//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/locale.hpp>
#include <iostream>

#include <ctime>

int main()
{
    using namespace boost::locale;

    // Create system default locale
    generator gen;
    std::locale loc=gen("");
    std::locale::global(loc);
    std::wcout.imbue(loc);

    // This is needed to prevent C library to
    // convert strings to narrow
    // instead of C++ on some platforms
    std::ios_base::sync_with_stdio(false);


    std::wcout << wformat(L"Today {1,date} at {1,time} we had run our first localization example") % time(0)
               << std::endl;

    std::wcout << L"This is how we show numbers in this locale " << as::number << 103.34 << std::endl;
    std::wcout << L"This is how we show currency in this locale " << as::currency << 103.34 << std::endl;
    std::wcout << L"This is typical date in the locale " << as::date << std::time(0) << std::endl;
    std::wcout << L"This is typical time in the locale " << as::time << std::time(0) << std::endl;
    std::wcout << L"This is upper case " << to_upper(L"Hello World!") << std::endl;
    std::wcout << L"This is lower case " << to_lower(L"Hello World!") << std::endl;
    std::wcout << L"This is title case " << to_title(L"Hello World!") << std::endl;
    std::wcout << L"This is fold case " << fold_case(L"Hello World!") << std::endl;

}


