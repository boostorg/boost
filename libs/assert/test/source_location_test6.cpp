// Copyright 2019 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/assert/source_location.hpp>
#include <boost/core/lightweight_test.hpp>

int main()
{
    {
        boost::source_location loc, loc2;

        BOOST_TEST_EQ( loc, loc2 );

        boost::source_location loc3 = BOOST_CURRENT_LOCATION;
        boost::source_location loc4( loc3 );

        BOOST_TEST_EQ( loc3, loc4 );
        BOOST_TEST_NE( loc3, loc );
    }

    return boost::report_errors();
}
