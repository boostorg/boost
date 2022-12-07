// Copyright 2022 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/unordered_map.hpp>
#include <boost/core/lightweight_test.hpp>
#include <string>

int main()
{
    boost::unordered_map<std::string, int> map;

    map[ "2" ] = 2;

    BOOST_TEST_EQ( map[ "1" ], 0 );
    BOOST_TEST_EQ( map[ "2" ], 2 );

    return boost::report_errors();
}
