// Copyright 2009-2011 Artyom Beilis (Tonkikh)
// Copyright 2021 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

#include <boost/locale.hpp>
#include <iostream>
#include <ctime>

int main()
{
    boost::locale::generator gen;

    std::locale loc = gen( "en_US.UTF-8" ); 

    std::cout.imbue( loc );

    std::cout << boost::locale::format( "Today {1,date} at {1,time} we had run our first localization example" ) % std::time( 0 ) << std::endl;

    std::cout << "This is how we show numbers in this locale " << boost::locale::as::number << 103.34 << std::endl; 
    std::cout << "This is how we show currency in this locale " << boost::locale::as::currency << 103.34 << std::endl; 

    std::cout << "This is typical date in the locale " << boost::locale::as::date << std::time( 0 ) << std::endl;
    std::cout << "This is typical time in the locale " << boost::locale::as::time << std::time( 0 ) << std::endl;

    std::string str( "Hello World!" );

    std::cout << "This is upper case " << boost::locale::to_upper( str, loc ) << std::endl;
    std::cout << "This is lower case " << boost::locale::to_lower( str, loc ) << std::endl;
    std::cout << "This is title case " << boost::locale::to_title( str, loc ) << std::endl;
    std::cout << "This is fold case " << boost::locale::fold_case( str, loc ) << std::endl;
}
