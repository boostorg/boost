// Copyright 2014 Renato Tegon Forti, Antony Polukhin.
// Copyright Antony Polukhin, 2015-2022.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// MinGW related workaround
#define BOOST_DLL_FORCE_ALIAS_INSTANTIATION

//[plugcpp_tutorial7_library1
#include <boost/dll/alias.hpp> // for BOOST_DLL_ALIAS_SECTIONED
#include <iostream>
#include <string>

void print(const std::string& s) {
    std::cout << "Hello, " << s << '!' << std::endl;
}

BOOST_DLL_ALIAS_SECTIONED(print, print_hello, Anna)
//]
