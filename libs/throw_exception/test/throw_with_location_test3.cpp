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
        throw my_exception();
    }
    catch( std::exception const & x )
    {
        boost::source_location loc = boost::get_throw_location( x );

        BOOST_TEST_CSTR_EQ( loc.file_name(), "" );
        BOOST_TEST_CSTR_EQ( loc.function_name(), "" );
        BOOST_TEST_EQ( loc.line(), 0u );
        BOOST_TEST_EQ( loc.column(), 0u );
    }

    return boost::report_errors();
}
