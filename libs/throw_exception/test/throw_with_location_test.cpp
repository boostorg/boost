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
    BOOST_TEST_THROWS( boost::throw_with_location( my_exception() ), my_exception );
    BOOST_TEST_THROWS( boost::throw_with_location( my_exception() ), std::exception );

    return boost::report_errors();
}
