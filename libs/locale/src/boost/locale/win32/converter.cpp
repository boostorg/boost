//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#define BOOST_LOCALE_SOURCE

#include <boost/locale/generator.hpp>
#include <boost/locale/conversion.hpp>
#include <boost/locale/encoding.hpp>
#include <cstring>
#include <locale>
#include <stdexcept>
#include "boost/locale/win32/api.hpp"
#include "boost/locale/win32/all_generator.hpp"

namespace boost {
namespace locale {
namespace impl_win {

class utf16_converter : public converter<wchar_t>
{
public:
    utf16_converter(winlocale const &lc,size_t refs = 0) :
        converter<wchar_t>(refs),
        lc_(lc)
    {
    }
    std::wstring convert(converter_base::conversion_type how,wchar_t const *begin,wchar_t const *end,int flags = 0) const BOOST_OVERRIDE
    {
        switch(how) {
        case converter_base::upper_case:
            return towupper_l(begin,end,lc_);
        case converter_base::lower_case:
            return towlower_l(begin,end,lc_);
        case converter_base::case_folding:
            return wcsfold(begin,end);
        case converter_base::normalization:
            return wcsnormalize(static_cast<norm_type>(flags),begin,end);
        default:
            return std::wstring(begin,end-begin);
        }
    }
private:
    winlocale lc_;
};

class utf8_converter : public converter<char> {
public:
    utf8_converter(winlocale const &lc,size_t refs = 0) :
        converter<char>(refs),
        lc_(lc)
    {
    }
    std::string convert(converter_base::conversion_type how,char const *begin,char const *end,int flags = 0) const BOOST_OVERRIDE
    {
        std::wstring tmp = conv::to_utf<wchar_t>(begin,end,"UTF-8");
        wchar_t const *wb=tmp.c_str();
        wchar_t const *we=wb+tmp.size();

        std::wstring res;

        switch(how) {
        case upper_case:
            res = towupper_l(wb,we,lc_);
            break;
        case lower_case:
            res = towlower_l(wb,we,lc_);
            break;
        case case_folding:
            res = wcsfold(wb,we);
            break;
        case normalization:
            res = wcsnormalize(static_cast<norm_type>(flags),wb,we);
            break;
        default:
            res = tmp; // make gcc happy
        }
        return conv::from_utf(res,"UTF-8");
    }
private:
    winlocale lc_;
};

std::locale create_convert( std::locale const &in,
                            winlocale const &lc,
                            character_facet_type type)
{
        switch(type) {
        case char_facet:
            return std::locale(in,new utf8_converter(lc));
        case wchar_t_facet:
            return std::locale(in,new utf16_converter(lc));
        default:
            return in;
        }
}


} // namespace impl_win32
} // locale
} // boost
