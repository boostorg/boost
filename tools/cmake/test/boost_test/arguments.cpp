// Copyright 2018 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

#include <boost/core/lightweight_test.hpp>
#include <string.h>

int main( int ac, char const* av[] )
{
    BOOST_TEST_EQ( ac, 2 );

    if( ac >= 2 )
    {
        BOOST_TEST_CSTR_EQ( av[1], "pumpkin" );
    }

    return boost::report_errors();
}
