//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#define BOOST_LOCALE_SOURCE
#include <boost/locale/encoding.hpp>
#include <boost/locale/encoding_errors.hpp>
#include <boost/locale/util.hpp>
#include <boost/locale/hold_ptr.hpp>
#include "boost/locale/encoding/conv.hpp"
#include "boost/locale/icu/all_generator.hpp"
#include "boost/locale/icu/uconv.hpp"
#include "boost/locale/icu/codecvt.hpp"
#include "boost/locale/icu/icu_util.hpp"
#include <unicode/ucnv.h>
#include <unicode/ucnv_err.h>

#ifdef BOOST_MSVC
#  pragma warning(disable : 4244) // loose data
#endif

namespace boost {
namespace locale {
namespace impl_icu {
    class uconv_converter : public util::base_converter {
    public:

        uconv_converter(std::string const &encoding) :
            encoding_(encoding)
        {
            UErrorCode err=U_ZERO_ERROR;

            // No need to check err each time, this
            // is how ICU works.
            cvt_ = ucnv_open(encoding.c_str(),&err);
            ucnv_setFromUCallBack(cvt_,UCNV_FROM_U_CALLBACK_STOP,0,0,0,&err);
            ucnv_setToUCallBack(cvt_,UCNV_TO_U_CALLBACK_STOP,0,0,0,&err);

            if(!cvt_ || U_FAILURE(err)) {
                if(cvt_)
                    ucnv_close(cvt_);
                throw conv::invalid_charset_error(encoding);
            }

            max_len_ = ucnv_getMaxCharSize(cvt_);
        }

        ~uconv_converter()
        {
            ucnv_close(cvt_);
        }

        bool is_thread_safe() const BOOST_OVERRIDE
        {
            return false;
        }

        uconv_converter *clone() const BOOST_OVERRIDE
        {
            return new uconv_converter(encoding_);
        }

        uint32_t to_unicode(char const *&begin,char const *end) BOOST_OVERRIDE
        {
            UErrorCode err=U_ZERO_ERROR;
            char const *tmp = begin;
            UChar32 c=ucnv_getNextUChar(cvt_,&tmp,end,&err);
            ucnv_reset(cvt_);
            if(err == U_TRUNCATED_CHAR_FOUND) {
                return incomplete;
            }
            if(U_FAILURE(err)) {
                return illegal;
            }

            begin = tmp;
            return c;
        }

        uint32_t from_unicode(uint32_t u,char *begin,char const *end) BOOST_OVERRIDE
        {
            UChar code_point[2]={0};
            int len;
            if(u<=0xFFFF) {
                if(0xD800 <=u && u<= 0xDFFF) // No surrogates
                    return illegal;
                code_point[0]=u;
                len=1;
            }
            else {
                u-=0x10000;
                code_point[0]=0xD800 | (u>>10);
                code_point[1]=0xDC00 | (u & 0x3FF);
                len=2;
            }
            UErrorCode err=U_ZERO_ERROR;
            int olen = ucnv_fromUChars(cvt_,begin,end-begin,code_point,len,&err);
            ucnv_reset(cvt_);
            if(err == U_BUFFER_OVERFLOW_ERROR)
                return incomplete;
            if(U_FAILURE(err))
                return illegal;
            return olen;
        }

        int max_len() const BOOST_OVERRIDE
        {
            return max_len_;
        }

    private:
        std::string encoding_;
        UConverter *cvt_;
        int max_len_;
    };

    util::base_converter *create_uconv_converter(std::string const &encoding)
    {
        hold_ptr<util::base_converter> cvt;
        try {
            cvt.reset(new uconv_converter(encoding));
        }
        catch(std::exception const &/*e*/)
        {
            // no encoding so we return empty pointer
        }
        return cvt.release();
    }

    std::locale create_codecvt(std::locale const &in,std::string const &encoding,character_facet_type type)
    {
        if(conv::impl::normalize_encoding(encoding.c_str())=="utf8")
            return util::create_utf8_codecvt(in,type);

        try {
            return util::create_simple_codecvt(in,encoding,type);
        }
        catch(boost::locale::conv::invalid_charset_error const &) {
            hold_ptr<util::base_converter> cvt;
            try {
                cvt.reset(create_uconv_converter(encoding));
            }
            catch(std::exception const &/*e*/)
            {
                cvt.reset(new util::base_converter());
            }
            return util::create_codecvt_from_pointer(in,cvt.release(),type);
        }
    }

} // impl_icu
} // locale
} // boost

