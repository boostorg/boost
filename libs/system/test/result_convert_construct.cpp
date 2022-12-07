// Copyright 2017, 2021, 2022 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/system/result.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/core/lightweight_test_trait.hpp>

using namespace boost::system;

struct X
{
    static int instances;

    int v_;

    X(): v_() { ++instances; }

    X( int v ): v_( v ) { ++instances; }

    X( X const& r ): v_( r.v_ ) { ++instances; }

    X& operator=( X const& ) = delete;

    ~X() { --instances; }
};

bool operator==( X const & x1, X const & x2 )
{
    return x1.v_ == x2.v_;
}

std::ostream& operator<<( std::ostream& os, X const & x )
{
    os << "X:" << x.v_;
    return os;
}

int X::instances = 0;

int main()
{
    {
        result<int> r( 5 );
        result<long> r2 = r;

        BOOST_TEST( r2 ) && BOOST_TEST_EQ( *r2, 5 );
    }

    {
        result<int> const r( 6 );
        result<long> r2 = r;

        BOOST_TEST( r2 ) && BOOST_TEST_EQ( *r2, 6 );
    }

    {
        result<long> r2 = result<int>( 7 );

        BOOST_TEST( r2 ) && BOOST_TEST_EQ( *r2, 7 );
    }

    BOOST_TEST_EQ( X::instances, 0 );

    {
        result<int> r( 5 );
        result<X> r2 = r;

        BOOST_TEST( r2 ) && BOOST_TEST_EQ( *r2, 5 );
        BOOST_TEST_EQ( X::instances, 1 );
    }

    BOOST_TEST_EQ( X::instances, 0 );

    {
        result<int> const r( 6 );
        result<X> r2 = r;

        BOOST_TEST( r2 ) && BOOST_TEST_EQ( *r2, 6 );
        BOOST_TEST_EQ( X::instances, 1 );
    }

    BOOST_TEST_EQ( X::instances, 0 );

    {
        result<X> r2 = result<int>( 7 );

        BOOST_TEST( r2 ) && BOOST_TEST_EQ( *r2, 7 );
        BOOST_TEST_EQ( X::instances, 1 );
    }

    BOOST_TEST_EQ( X::instances, 0 );

    {
        auto ec = make_error_code( errc::invalid_argument );

        result<int> r( ec );
        result<long> r2 = r;

        BOOST_TEST( !r2 );
        BOOST_TEST_EQ( r2.error(), ec );
    }

    {
        auto ec = make_error_code( errc::invalid_argument );

        result<int> r( ec );
        result<long> r2 = r;

        BOOST_TEST( !r2 );
        BOOST_TEST_EQ( r2.error(), ec );
    }

    {
        auto ec = make_error_code( errc::invalid_argument );

        result<long> r2 = result<int>( ec );

        BOOST_TEST( !r2 );
        BOOST_TEST_EQ( r2.error(), ec );
    }

    BOOST_TEST_EQ( X::instances, 0 );

    {
        result<char const*, int> r( "test" );
        result<std::string, X> r2 = r;

        BOOST_TEST( r2 ) && BOOST_TEST_EQ( *r2, std::string( "test" ) );
        BOOST_TEST_EQ( X::instances, 0 );
    }

    BOOST_TEST_EQ( X::instances, 0 );

    {
        result<char const*, int> const r( "test" );
        result<std::string, X> r2 = r;

        BOOST_TEST( r2 ) && BOOST_TEST_EQ( *r2, std::string( "test" ) );
        BOOST_TEST_EQ( X::instances, 0 );
    }

    BOOST_TEST_EQ( X::instances, 0 );

    {
        result<std::string, X> r2 = result<char const*, int>( "test" );

        BOOST_TEST( r2 ) && BOOST_TEST_EQ( *r2, std::string( "test" ) );
        BOOST_TEST_EQ( X::instances, 0 );
    }

    BOOST_TEST_EQ( X::instances, 0 );

    {
        result<char const*, int> r( 5 );
        result<std::string, X> r2 = r;

        BOOST_TEST( !r2 );
        BOOST_TEST_EQ( r2.error(), X(5) );
        BOOST_TEST_EQ( X::instances, 1 );
    }

    BOOST_TEST_EQ( X::instances, 0 );

    {
        result<char const*, int> const r( 6 );
        result<std::string, X> r2 = r;

        BOOST_TEST( !r2 );
        BOOST_TEST_EQ( r2.error(), X(6) );
        BOOST_TEST_EQ( X::instances, 1 );
    }

    {
        result<std::string, X> r2 = result<char const*, int>( 7 );

        BOOST_TEST( !r2 );
        BOOST_TEST_EQ( r2.error(), X(7) );
        BOOST_TEST_EQ( X::instances, 1 );
    }

    BOOST_TEST_EQ( X::instances, 0 );

    {
        BOOST_TEST_TRAIT_TRUE((std::is_constructible<result<long>, result<int>>));
        BOOST_TEST_TRAIT_TRUE((std::is_convertible<result<int>, result<long>>));

        BOOST_TEST_TRAIT_FALSE((std::is_constructible<result<int>, result<void*>>));
        BOOST_TEST_TRAIT_FALSE((std::is_convertible<result<void*>, result<int>>));

        BOOST_TEST_TRAIT_FALSE((std::is_constructible<result<int>, result<void>>));
        BOOST_TEST_TRAIT_FALSE((std::is_convertible<result<void>, result<int>>));

        BOOST_TEST_TRAIT_FALSE((std::is_constructible<result<void>, result<int>>));
        BOOST_TEST_TRAIT_FALSE((std::is_convertible<result<int>, result<void>>));

        BOOST_TEST_TRAIT_FALSE((std::is_constructible<result<int, void*>, result<int, int>>));
        BOOST_TEST_TRAIT_FALSE((std::is_convertible<result<int, int>, result<int, void*>>));
    }

    return boost::report_errors();
}
