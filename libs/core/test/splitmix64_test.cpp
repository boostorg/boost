// Test for boost/core/detail/splitmix64.hpp
//
// Copyright 2022 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/core/detail/splitmix64.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/cstdint.hpp>

int main()
{
    {
        boost::detail::splitmix64 rng;

        boost::uint64_t r1 = rng(), x1 = ( boost::uint64_t(0xe220a839u) << 32 ) + 0x7b1dcdafu;
        BOOST_TEST_EQ( r1, x1 );

        for( int i = 0; i < 1000; ++i ) rng();

        boost::uint64_t r2 = rng(), x2 = ( boost::uint64_t(0xdcad513cu) << 32 ) + 0x211f942du;
        BOOST_TEST_EQ( r2, x2 );
    }

    {
        boost::detail::splitmix64 rng( 12345 );

        boost::uint64_t r1 = rng(), x1 = ( boost::uint64_t(0x22118258u) << 32 ) + 0xa9d111a0u;
        BOOST_TEST_EQ( r1, x1 );

        for( int i = 0; i < 1000; ++i ) rng();

        boost::uint64_t r2 = rng(), x2 = ( boost::uint64_t(0xaaefb4b6u) << 32 ) + 0x5c15e958u;
        BOOST_TEST_EQ( r2, x2 );
    }

    return boost::report_errors();
}
