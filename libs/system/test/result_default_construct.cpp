// Copyright 2017, 2021 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/system/result.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/core/lightweight_test_trait.hpp>

using namespace boost::system;

struct X
{
};

struct Y
{
    Y( int );
};

int main()
{
    {
        result<int> r;

        BOOST_TEST( r.has_value() );
        BOOST_TEST( !r.has_error() );

        BOOST_TEST_EQ( r.value(), 0 );
    }

    {
        result<X> r;

        BOOST_TEST( r.has_value() );
        BOOST_TEST( !r.has_error() );
    }

    {
        result<void> r;

        BOOST_TEST( r.has_value() );
        BOOST_TEST( !r.has_error() );
    }

    {
        BOOST_TEST_TRAIT_TRUE((std::is_default_constructible<result<int>>));
        BOOST_TEST_TRAIT_TRUE((std::is_default_constructible<result<int, int>>));
        BOOST_TEST_TRAIT_TRUE((std::is_default_constructible<result<X>>));
        BOOST_TEST_TRAIT_TRUE((std::is_default_constructible<result<X, int>>));
        BOOST_TEST_TRAIT_TRUE((std::is_default_constructible<result<void>>));
        BOOST_TEST_TRAIT_TRUE((std::is_default_constructible<result<void, int>>));

        BOOST_TEST_TRAIT_FALSE((std::is_default_constructible<result<Y>>));
        BOOST_TEST_TRAIT_FALSE((std::is_default_constructible<result<Y, int>>));
    }

    return boost::report_errors();
}
