//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/locale.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <iostream>

#include <ctime>



int main()
{
    using namespace boost::locale;
    // Create system default locale
    generator gen;
    std::locale loc=gen("");
    std::locale::global(loc);
    std::cout.imbue(loc);


    std::cout << "Correct case conversion can't be done by simple, character by character conversion\n";
    std::cout << "because case conversion is context sensitive and not 1-to-1 conversion\n";
    std::cout << "For example:\n";
    std::cout << "   German grüßen correctly converted to "
              << to_upper("grüßen") << ", instead of incorrect "
              << boost::to_upper_copy(std::string("grüßen")) << std::endl;
    std::cout << "     where ß is replaced with SS\n";
    std::cout << "   Greek ὈΔΥΣΣΕΎΣ is correctly converted to "
              << to_lower("ὈΔΥΣΣΕΎΣ") << ", instead of incorrect "
              << boost::to_lower_copy(std::string("ὈΔΥΣΣΕΎΣ")) << std::endl;
    std::cout << "     where Σ is converted to σ or to ς, according to position in the word\n";
    std::cout << "Such type of conversion just can't be done using std::toupper that work on character base, also std::toupper is\n";
    std::cout << "not even applicable when working with variable character length like in UTF-8 or UTF-16 limiting the correct\n";
    std::cout << "behavior to unicode subset BMP or ASCII only\n";
}

// boostinspect:noascii
