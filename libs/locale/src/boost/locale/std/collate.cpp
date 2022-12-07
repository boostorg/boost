//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#define BOOST_LOCALE_SOURCE
#include <boost/locale/encoding.hpp>
#include <locale>
#include <string>
#include <ios>
#include "boost/locale/std/all_generator.hpp"

namespace boost {
namespace locale {
namespace impl_std {

class utf8_collator_from_wide : public std::collate<char> {
public:
    typedef std::collate<wchar_t> wfacet;
    utf8_collator_from_wide(std::locale const &base,size_t refs = 0) :
        std::collate<char>(refs),
        base_(base)
    {
    }
    int do_compare(char const *lb,char const *le,char const *rb,char const *re) const BOOST_OVERRIDE
    {
        std::wstring l=conv::to_utf<wchar_t>(lb,le,"UTF-8");
        std::wstring r=conv::to_utf<wchar_t>(rb,re,"UTF-8");
        return std::use_facet<wfacet>(base_).compare(   l.c_str(),l.c_str()+l.size(),
                                                        r.c_str(),r.c_str()+r.size());
    }
    long do_hash(char const *b,char const *e) const BOOST_OVERRIDE
    {
        std::wstring tmp=conv::to_utf<wchar_t>(b,e,"UTF-8");
        return std::use_facet<wfacet>(base_).hash(tmp.c_str(),tmp.c_str()+tmp.size());
    }
    std::string do_transform(char const *b,char const *e) const BOOST_OVERRIDE
    {
        std::wstring tmp=conv::to_utf<wchar_t>(b,e,"UTF-8");
        std::wstring wkey =
            std::use_facet<wfacet>(base_).transform(tmp.c_str(),tmp.c_str()+tmp.size());
        std::string key;
BOOST_LOCALE_START_CONST_CONDITION
        if(sizeof(wchar_t)==2)
            key.reserve(wkey.size()*2);
        else
            key.reserve(wkey.size()*3);
        for(unsigned i=0;i<wkey.size();i++) {
            if(sizeof(wchar_t)==2) {
                uint16_t tv = static_cast<uint16_t>(wkey[i]);
                key += char(tv >> 8);
                key += char(tv & 0xFF);
            }
            else { // 4
                uint32_t tv = static_cast<uint32_t>(wkey[i]);
                // 21 bit
                key += char((tv >> 16) & 0xFF);
                key += char((tv >> 8) & 0xFF);
                key += char(tv & 0xFF);
            }
        }
BOOST_LOCALE_END_CONST_CONDITION
        return key;
    }
private:
    std::locale base_;
};

std::locale create_collate( std::locale const &in,
                            std::string const &locale_name,
                            character_facet_type type,
                            utf8_support utf)
{
    switch(type) {
    case char_facet:
        {
            if(utf == utf8_from_wide) {
                std::locale base=
                    std::locale(std::locale::classic(),
                                new std::collate_byname<wchar_t>(locale_name.c_str()));
                return std::locale(in,new utf8_collator_from_wide(base));
            }
            else
            {
                return std::locale(in,new std::collate_byname<char>(locale_name.c_str()));
            }
        }

    case wchar_t_facet:
        return std::locale(in,new std::collate_byname<wchar_t>(locale_name.c_str()));

    #ifdef BOOST_LOCALE_ENABLE_CHAR16_T
    case char16_t_facet:
        return std::locale(in,new std::collate_byname<char16_t>(locale_name.c_str()));
    #endif

    #ifdef BOOST_LOCALE_ENABLE_CHAR32_T
    case char32_t_facet:
        return std::locale(in,new std::collate_byname<char32_t>(locale_name.c_str()));
    #endif
    default:
        return in;
    }
}


} // impl_std
} // locale
} //boost



