///////////////////////////////////////////////////////////////////////////////
//  Copyright 2022 Matt Borland. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  See: https://github.com/boostorg/multiprecision/issues/464

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <locale>
#include "test.hpp"

template <typename T>
void test()
{
    auto a = boost::multiprecision::cpp_dec_float_50 {12345};

    auto d1 = a.convert_to<T>();

    std::locale::global(std::locale("de_DE"));
    auto d2 = a.convert_to<T>();

    BOOST_CHECK_EQUAL(d1, d2);
}

int main(void)
{
    test<double>();
    test<long double>();

    return boost::report_errors();
}
