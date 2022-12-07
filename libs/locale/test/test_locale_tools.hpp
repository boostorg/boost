//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_LOCLAE_TEST_LOCALE_TOOLS_HPP
#define BOOST_LOCLAE_TEST_LOCALE_TOOLS_HPP

#include <boost/locale/encoding.hpp>
#include <fstream>

#ifndef BOOST_LOCALE_NO_POSIX_BACKEND
#include "test_posix_tools.hpp"
#endif

#if defined(BOOST_MSVC) && BOOST_MSVC < 1700
#pragma warning(disable : 4428) // universal-character-name encountered in source
#endif

template<typename Char>
std::basic_string<Char> to_correct_string(std::string const &e,std::locale /*l*/)
{
    return boost::locale::conv::to_utf<Char>(e,"UTF-8");
}


template<>
inline std::string to_correct_string(std::string const &e,std::locale l)
{
    return boost::locale::conv::from_utf(e,l);
}

bool has_std_locale(std::string const &name)
{
    try {
        std::locale tmp(name.c_str());
        return true;
    }
    catch(...) {
        return false;
    }
}

inline bool test_std_supports_SJIS_codecvt(std::string const &locale_name)
{
    bool res = true;
    {
    // Japan in Shift JIS/cp932
        char const *japan_932 = "\x93\xfa\x96\x7b";
        std::ofstream f("test-siftjis.txt");
        f << japan_932;
        f.close();
    }
    try {
        std::wfstream test;
        test.imbue(std::locale(locale_name.c_str()));
        test.open("test-siftjis.txt");
        // Japan in Unicode
        std::wstring cmp = L"\u65e5\u672c";
        std::wstring ref;
        test >> ref;
        res = ref == cmp;
    }
    catch(std::exception const &)
    {
        res = false;
    }
    remove("test-siftjis.txt");
    return res;
}

std::string get_std_name(std::string const &name,std::string *real_name = 0)
{
    if(has_std_locale(name)) {
        if(real_name)
            *real_name = name;
        return name;
    }

    #ifdef BOOST_WINDOWS
    bool utf8=name.find("UTF-8")!=std::string::npos;

    if(name=="en_US.UTF-8" || name == "en_US.ISO8859-1") {
        if(has_std_locale("English_United States.1252")) {
            if(real_name)
                *real_name = "English_United States.1252";
            return utf8 ? name : "en_US.windows-1252";
        }
        return "";
    }
    else if(name=="he_IL.UTF-8" || name == "he_IL.ISO8859-8")  {
        if(has_std_locale("Hebrew_Israel.1255")) {
            if(real_name)
                *real_name = "Hebrew_Israel.1255";
            return utf8 ? name : "he_IL.windows-1255";
        }
    }
    else if(name=="ru_RU.UTF-8")  {
        if(has_std_locale("Russian_Russia.1251")) {
            if(real_name)
                *real_name = "Russian_Russia.1251";
            return name;
        }
    }
    else if(name == "tr_TR.UTF-8") {
        if(has_std_locale("Turkish_Turkey.1254")) {
            if(real_name)
                *real_name = "Turkish_Turkey.1254";
            return name;
        }
    }
    if(name == "ja_JP.SJIS") {
        if(has_std_locale("Japanese_Japan.932")) {
            if(real_name)
                *real_name = "Japanese_Japan.932";
            return name;
        }
        return "";
    }
    #endif
    return "";
}

char* make2(unsigned v)
{
    static unsigned char buf[3] = {0};
    buf[0] = static_cast<unsigned char>(0xC0 | (v >> 6));
    buf[1] = static_cast<unsigned char>(0x80 | (v & 0x3F));
    return reinterpret_cast<char*>(buf);
}

char* make3(unsigned v)
{
    static unsigned char buf[4] = {0};
    buf[0] = static_cast<unsigned char>(0xE0 | ((v >> 12)));
    buf[1] = static_cast<unsigned char>(0x80 | ((v >> 6) & 0x3F));
    buf[2] = static_cast<unsigned char>(0x80 | ((v >> 0) & 0x3F));
    return reinterpret_cast<char*>(buf);
}

char* make4(unsigned v)
{
    static unsigned char buf[5] = {0};
    buf[0] = static_cast<unsigned char>(0xF0 | ((v >> 18)));
    buf[1] = static_cast<unsigned char>(0x80 | ((v >> 12) & 0x3F));
    buf[2] = static_cast<unsigned char>(0x80 | ((v >> 6) & 0x3F));
    buf[3] = static_cast<unsigned char>(0x80 | ((v >> 0) & 0x3F));
    return reinterpret_cast<char*>(buf);
}

#endif
