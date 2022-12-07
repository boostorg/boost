// Copyright 2022 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/throw_exception.hpp>
#include <boost/core/lightweight_test.hpp>

#if defined(_MSC_VER)
# pragma warning(disable: 4702) // unreachable code
#endif

class my_exception: public std::exception
{
};

int main()
{
    try
    {
        boost::throw_with_location( my_exception() );

        BOOST_ERROR( "boost::throw_with_location failed to throw" );
    }
    catch( std::exception const & x )
    {
        boost::source_location loc = boost::get_throw_location( x );

        // When not supplied explicitly, the source location is best effort.
        // It should be the location of the throw_with_location call on
        // recent compilers, but that's not guaranteed for every compiler.
        // So we can't be more specific in testing it.

        BOOST_TEST_CSTR_NE( loc.file_name(), "" );
        BOOST_TEST_NE( loc.line(), 0u );
    }
    catch( ... )
    {
        BOOST_ERROR( "boost::throw_with_location failed to throw std::exception" );
    }

    boost::source_location location = BOOST_CURRENT_LOCATION;

    try
    {
        boost::throw_with_location( my_exception(), location );

        BOOST_ERROR( "boost::throw_with_location failed to throw" );
    }
    catch( std::exception const & x )
    {
        boost::source_location loc = boost::get_throw_location( x );

        BOOST_TEST_CSTR_EQ( loc.file_name(), location.file_name() );
        BOOST_TEST_CSTR_EQ( loc.function_name(), location.function_name() );
        BOOST_TEST_EQ( loc.line(), location.line() );
        BOOST_TEST_EQ( loc.column(), location.column() );
    }
    catch( ... )
    {
        BOOST_ERROR( "boost::throw_with_location failed to throw std::exception" );
    }

    return boost::report_errors();
}
