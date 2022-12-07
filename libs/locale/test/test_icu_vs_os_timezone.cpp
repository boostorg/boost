//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_LOCALE_WITH_ICU
#include <iostream>
int main()
{
        std::cout << "ICU is not build... Skipping\n";
}
#else

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
// Disable this "security crap"
#endif

#include <boost/locale/format.hpp>
#include <boost/locale/formatting.hpp>
#include <boost/locale/generator.hpp>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <sstream>

#include "test_locale.hpp"
#include "test_locale_tools.hpp"

void test_main(int /*argc*/, char** /*argv*/)
{
    time_t now=time(0);
    boost::locale::generator gen;
    std::locale::global(gen("en_US.UTF-8"));

    for(int i=0;i<366;i++) {
        time_t point = now + i * 24 * 3600;
        std::stringstream ss;
        ss << boost::locale::format("{1,ftime='%H %M %S'}") % point;
        int icu_hour = 0,icu_min = 0,icu_sec = 0;
        ss >> icu_hour >> icu_min >> icu_sec;
        std::tm *tm=localtime(&point);
        TEST(icu_hour == tm->tm_hour);
        TEST(icu_min == tm->tm_min);
        TEST(icu_sec == tm->tm_sec);

    }
}

#endif // NO ICU

// boostinspect:noascii
