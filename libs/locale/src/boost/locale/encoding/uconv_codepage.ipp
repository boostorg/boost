//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_LOCALE_IMPL_UCONV_CODEPAGE_HPP
#define BOOST_LOCALE_IMPL_UCONV_CODEPAGE_HPP
#include <boost/locale/encoding.hpp>
#include <boost/locale/hold_ptr.hpp>
#include <unicode/ucnv.h>
#include <unicode/ucnv_err.h>
#include "boost/locale/encoding/conv.hpp"
#include "boost/locale/icu/icu_util.hpp"
#include "boost/locale/icu/uconv.hpp"

namespace boost {
namespace locale {
namespace conv {
namespace impl {
    template<typename CharType>
    class uconv_to_utf : public converter_to_utf<CharType> {
    public:
        typedef CharType char_type;

        typedef std::basic_string<char_type> string_type;

        bool open(char const *charset,method_type how) BOOST_OVERRIDE
        {
            close();
            try {
                cvt_from_.reset(new from_type(charset,how == skip ? impl_icu::cvt_skip : impl_icu::cvt_stop));
                cvt_to_.reset(new to_type("UTF-8",how == skip ? impl_icu::cvt_skip : impl_icu::cvt_stop));
            }
            catch(std::exception const &/*e*/) {
                close();
                return false;
            }
            return true;
        }
        void close()
        {
            cvt_from_.reset();
            cvt_to_.reset();
        }

        string_type convert(char const *begin,char const *end) BOOST_OVERRIDE
        {
            try {
                return cvt_to_->std(cvt_from_->icu_checked(begin,end));
            }
            catch(std::exception const &/*e*/) {
                throw conversion_error();
            }
        }

    private:

        typedef impl_icu::icu_std_converter<char> from_type;
        typedef impl_icu::icu_std_converter<CharType> to_type;

        hold_ptr<from_type> cvt_from_;
        hold_ptr<to_type> cvt_to_;

    };


    template<typename CharType>
    class uconv_from_utf : public converter_from_utf<CharType> {
    public:
        typedef CharType char_type;
        bool open(char const *charset,method_type how) BOOST_OVERRIDE
        {
            close();
            try {
                cvt_from_.reset(new from_type("UTF-8",how == skip ? impl_icu::cvt_skip : impl_icu::cvt_stop));
                cvt_to_.reset(new to_type(charset,how == skip ? impl_icu::cvt_skip : impl_icu::cvt_stop));
            }
            catch(std::exception const &/*e*/) {
                close();
                return false;
            }
            return true;
        }
        void close()
        {
            cvt_from_.reset();
            cvt_to_.reset();
        }

        std::string convert(CharType const *begin,CharType const *end) BOOST_OVERRIDE
        {
            try {
                return cvt_to_->std(cvt_from_->icu_checked(begin,end));
            }
            catch(std::exception const &/*e*/) {
                throw conversion_error();
            }
        }

    private:

        typedef impl_icu::icu_std_converter<CharType> from_type;
        typedef impl_icu::icu_std_converter<char> to_type;

        hold_ptr<from_type> cvt_from_;
        hold_ptr<to_type> cvt_to_;

    };

    class uconv_between : public converter_between {
    public:
        bool open(char const *to_charset,char const *from_charset,method_type how) BOOST_OVERRIDE
        {
            close();
            try {
                cvt_from_.reset(new from_type(from_charset,how == skip ? impl_icu::cvt_skip : impl_icu::cvt_stop));
                cvt_to_.reset(new to_type(to_charset,how == skip ? impl_icu::cvt_skip : impl_icu::cvt_stop));
            }
            catch(std::exception const &/*e*/) {
                close();
                return false;
            }
            return true;
        }
        void close()
        {
            cvt_from_.reset();
            cvt_to_.reset();
        }

        std::string convert(char const *begin,char const *end) BOOST_OVERRIDE
        {
            try {
                return cvt_to_->std(cvt_from_->icu(begin,end));
            }
            catch(std::exception const &/*e*/) {
                throw conversion_error();
            }
        }

    private:

        typedef impl_icu::icu_std_converter<char> from_type;
        typedef impl_icu::icu_std_converter<char> to_type;

        hold_ptr<from_type> cvt_from_;
        hold_ptr<to_type> cvt_to_;

    };


} // impl
} // conv
} // locale
} // boost


#endif
