// Copyright 2021 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/core/detail/string_view.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/core/lightweight_test.hpp>
#include <string>

boost::core::string_view f( boost::core::string_view const& str )
{
    return str;
}

int main()
{
    {
        std::string s1( "123" );
        std::string s2 = f( s1 );

        BOOST_TEST_EQ( s1, s2 );
    }

    {
        boost::string_view s1( "123" );
        boost::string_view s2 = f( s1 );

        BOOST_TEST_EQ( s1, s2 );
    }

    return boost::report_errors();
}
