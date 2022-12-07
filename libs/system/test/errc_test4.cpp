// Copyright 2022 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/system/errc.hpp>
#include <boost/assert/source_location.hpp>
#include <boost/core/lightweight_test.hpp>

int main()
{
    BOOST_STATIC_CONSTEXPR boost::source_location loc = BOOST_CURRENT_LOCATION;

    BOOST_TEST( make_error_code( boost::system::errc::no_such_file_or_directory, &loc ).has_location() );
    BOOST_TEST_EQ( make_error_code( boost::system::errc::no_such_file_or_directory, &loc ).location().to_string(), loc.to_string() );

    return boost::report_errors();
}
