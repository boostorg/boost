//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <boost/locale.hpp>
#include <cstdlib>
#include <clocale>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <locale>
#include <stdexcept>

#ifdef BOOST_LOCALE_WITH_ICU
#include <unicode/uversion.h>
#endif

#include "test_locale_tools.hpp"

char const *env(char const *s)
{
    char const *r=getenv(s);
    if(r)
        return r;
    return "";
}

void check_locale(char const **names)
{
    std::cout << "  " << std::setw(32) << "locale" << std::setw(4) << "C" << std::setw(4) << "C++\n";
    while(*names) {
        char const *name = *names;
        std::cout << "  " << std::setw(32) << name << std::setw(4);
        if(setlocale(LC_ALL,name)!=0)
            std::cout << "Yes";
        else
            std::cout << "No";
        std::cout << std::setw(4);
        try {
            std::locale l(name);
            std::cout << "Yes";
        }
        catch(std::exception const &) {
            std::cout << "No";
        }
        std::cout << std::endl;
        names++;
    }
}

int main()
{
    std::cout << "- Backends: ";
    #ifdef BOOST_LOCALE_WITH_ICU
    std::cout << "icu:" << U_ICU_VERSION << " ";
    #endif
    #ifndef BOOST_LOCALE_NO_STD_BACKEND
    std::cout << "std ";
    #endif
    #ifndef BOOST_LOCALE_NO_POSIX_BACKEND
    std::cout << "posix ";
    #endif
    #ifndef BOOST_LOCALE_NO_WINAPI_BACKEND
    std::cout << "winapi";
    #endif
    std::cout << std::endl;
    #ifdef BOOST_LOCALE_WITH_ICONV
    std::cout << "- With iconv\n";
    #else
    std::cout << "- Without iconv\n";
    #endif
    std::cout << "- Environment \n";
    std::cout << "  LANG=" << env("LANG") << std::endl;
    std::cout << "  LC_ALL=" << env("LC_ALL") << std::endl;
    std::cout << "  LC_CTYPE=" << env("LC_CTYPE") << std::endl;
    std::cout << "  TZ=" << env("TZ") << std::endl;

    char const *clocale=setlocale(LC_ALL,"");
    if(!clocale)
        clocale= "undetected"; // LCOV_EXCL_LINE
    std::cout << "- C locale: " << clocale << std::endl;

    try {
        std::locale loc("");
#if defined(BOOST_CLANG) && BOOST_CLANG_VERSION < 30800
        std::cout << "- C++ locale: n/a on Clang < 3.8\n";
#else
        std::cout << "- C++ locale: " << loc.name() << std::endl;
#endif
    }
    catch(std::exception const &) {
        std::cout << "- C++ locale: is not supported\n"; // LCOV_EXCL_LINE
    }

    char const *locales_to_check[] = {
        "en_US.UTF-8", "en_US.ISO8859-1", "English_United States.1252",
        "he_IL.UTF-8", "he_IL.ISO8859-8", "Hebrew_Israel.1255",
        "ru_RU.UTF-8", "Russian_Russia.1251",
        "tr_TR.UTF-8", "Turkish_Turkey.1254",
        "ja_JP.UTF-8", "ja_JP.SJIS", "Japanese_Japan.932",
        0
    };
    std::cout << "- Testing locales availability on the operation system:" << std::endl;
    check_locale(locales_to_check);
    std::cout << "--- Testing Japanese_Japan.932 is working: " << test_std_supports_SJIS_codecvt("Japanese_Japan.932") << std::endl;

    std::cout << "- Testing timezone and time " << std::endl;
    {
        setlocale(LC_ALL,"C");
        time_t now = time(0);
        char buf[1024];
        strftime(buf,sizeof(buf),"%%c=%c; %%Z=%Z; %%z=%z",localtime(&now));
        std::cout << "  Local Time    :" << buf << std::endl;
        strftime(buf,sizeof(buf),"%%c=%c; %%Z=%Z; %%z=%z",gmtime(&now));
        std::cout << "  Universal Time:" << buf << std::endl;
    }
    std::cout << "- Boost.Locale's locale: ";
    try {
        boost::locale::generator gen;
        std::locale l = gen("");
        std::cout << std::use_facet<boost::locale::info>(l).name() << std::endl;
    }
    catch(std::exception const &) {
        std::cout << " undetected\n"; // LCOV_EXCL_LINE
        return EXIT_FAILURE;          // LCOV_EXCL_LINE
    }
    return EXIT_SUCCESS;
}

// boostinspect:noascii
