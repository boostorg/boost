//  Boost integer_log2.hpp test program  --------------------------------------//

//  (C) Copyright Andrey Semashev 2021.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  https://www.boost.org/LICENSE_1_0.txt)

//  See https://www.boost.org for most recent version including documentation.

#include <boost/config.hpp>
#include <boost/cstdint.hpp>
#include <boost/integer/integer_log2.hpp>

#include <boost/core/lightweight_test.hpp>

#include <iostream>

#include "multiprecision_config.hpp"

#if !defined(DISABLE_MP_TESTS)
#include <boost/multiprecision/cpp_int.hpp>
#endif

// Macros to compact code
#define PRIVATE_LB_TEST( v, e )  BOOST_TEST( ::boost::integer_log2((v)) == e )

#define PRIVATE_PRINT_LB( v )    ::std::cout << "boost::integer_log2(" << (v) \
 << ") = " << ::boost::integer_log2((v)) << '.' << ::std::endl

// Control to check for a compile-time error
#ifndef CONTROL_LB_0_TEST
#define PRIVATE_LB_0_TEST
#else
#define PRIVATE_LB_0_TEST  PRIVATE_PRINT_LB( 0 )
#endif

// Main testing function
int main()
{
    std::cout << "Doing tests on integer_log2." << std::endl;

    PRIVATE_LB_0_TEST;

    PRIVATE_LB_TEST( (unsigned char)1, 0 );
    PRIVATE_LB_TEST( (unsigned char)2, 1 );

    PRIVATE_LB_TEST( 1, 0 );

    PRIVATE_LB_TEST( 2, 1 );
    PRIVATE_LB_TEST( 3, 1 );

    PRIVATE_LB_TEST( 4, 2 );
    PRIVATE_LB_TEST( 5, 2 );
    PRIVATE_LB_TEST( 6, 2 );
    PRIVATE_LB_TEST( 7, 2 );

    PRIVATE_LB_TEST(  8, 3 );
    PRIVATE_LB_TEST(  9, 3 );
    PRIVATE_LB_TEST( 10, 3 );
    PRIVATE_LB_TEST( 11, 3 );
    PRIVATE_LB_TEST( 12, 3 );
    PRIVATE_LB_TEST( 13, 3 );
    PRIVATE_LB_TEST( 14, 3 );
    PRIVATE_LB_TEST( 15, 3 );

    PRIVATE_LB_TEST( 16, 4 );
    PRIVATE_LB_TEST( 17, 4 );
    PRIVATE_LB_TEST( 18, 4 );
    PRIVATE_LB_TEST( 19, 4 );
    PRIVATE_LB_TEST( 20, 4 );
    PRIVATE_LB_TEST( 21, 4 );
    PRIVATE_LB_TEST( 22, 4 );
    PRIVATE_LB_TEST( 23, 4 );
    PRIVATE_LB_TEST( 24, 4 );
    PRIVATE_LB_TEST( 25, 4 );
    PRIVATE_LB_TEST( 26, 4 );
    PRIVATE_LB_TEST( 27, 4 );
    PRIVATE_LB_TEST( 28, 4 );
    PRIVATE_LB_TEST( 29, 4 );
    PRIVATE_LB_TEST( 30, 4 );
    PRIVATE_LB_TEST( 31, 4 );

    PRIVATE_LB_TEST( 32, 5 );
    PRIVATE_LB_TEST( 33, 5 );
    PRIVATE_LB_TEST( 34, 5 );
    PRIVATE_LB_TEST( 35, 5 );
    PRIVATE_LB_TEST( 36, 5 );
    PRIVATE_LB_TEST( 37, 5 );
    PRIVATE_LB_TEST( 38, 5 );
    PRIVATE_LB_TEST( 39, 5 );
    PRIVATE_LB_TEST( 40, 5 );

    PRIVATE_LB_TEST( 63, 5 );

    PRIVATE_LB_TEST( 64, 6 );
    PRIVATE_LB_TEST( 65, 6 );

    PRIVATE_LB_TEST( 127, 6 );

    PRIVATE_LB_TEST( 128, 7 );
    PRIVATE_LB_TEST( 129, 7 );

    PRIVATE_LB_TEST( 255, 7 );

    PRIVATE_LB_TEST( 256, 8 );
    PRIVATE_LB_TEST( 257, 8 );

    PRIVATE_LB_TEST( 511, 8 );

    PRIVATE_LB_TEST( 512, 9 );
    PRIVATE_LB_TEST( 513, 9 );

    PRIVATE_LB_TEST( 1023, 9 );

    PRIVATE_LB_TEST( 1024, 10 );
    PRIVATE_LB_TEST( 1025, 10 );

    PRIVATE_LB_TEST( 2047, 10 );

    PRIVATE_LB_TEST( 2048, 11 );
    PRIVATE_LB_TEST( 2049, 11 );

    PRIVATE_LB_TEST( 4095, 11 );

    PRIVATE_LB_TEST( 4096, 12 );
    PRIVATE_LB_TEST( 4097, 12 );

    PRIVATE_LB_TEST( 8191, 12 );

    PRIVATE_LB_TEST( 8192, 13 );
    PRIVATE_LB_TEST( 8193, 13 );

    PRIVATE_LB_TEST( 16383, 13 );

    PRIVATE_LB_TEST( 16384, 14 );
    PRIVATE_LB_TEST( 16385, 14 );

    PRIVATE_LB_TEST( 32767, 14 );

    PRIVATE_LB_TEST( 32768, 15 );
    PRIVATE_LB_TEST( 32769, 15 );

    PRIVATE_LB_TEST( 65535, 15 );

    PRIVATE_LB_TEST( 65536, 16 );
    PRIVATE_LB_TEST( 65537, 16 );

#if defined(UINT32_C)
    PRIVATE_LB_TEST( UINT32_C(4294967295), 31 );
#endif

#if defined(UINT64_C) && !defined(BOOST_NO_INT64_T)
    PRIVATE_LB_TEST( UINT64_C(4294967296), 32 );
    PRIVATE_LB_TEST( UINT64_C(4294967297), 32 );

    PRIVATE_LB_TEST( UINT64_C(18446744073709551615), 63 );
#endif

#if defined(BOOST_HAS_INT128)
    PRIVATE_LB_TEST( boost::uint128_type(1u) << 64u, 64 );
    PRIVATE_LB_TEST( (boost::uint128_type(1u) << 64u) + 1u, 64 );

    PRIVATE_LB_TEST( ~boost::uint128_type(0u), 127 );
#endif

#if !defined(DISABLE_MP_TESTS)
    PRIVATE_LB_TEST( boost::multiprecision::cpp_int(1), 0 );
    PRIVATE_LB_TEST( boost::multiprecision::cpp_int(2), 1 );
    PRIVATE_LB_TEST( boost::multiprecision::cpp_int(3), 1 );
    PRIVATE_LB_TEST( boost::multiprecision::cpp_int(65535), 15 );
    PRIVATE_LB_TEST( boost::multiprecision::cpp_int(65536), 16 );

    PRIVATE_LB_TEST( boost::multiprecision::int1024_t(1), 0 );
    PRIVATE_LB_TEST( boost::multiprecision::int1024_t(2), 1 );
    PRIVATE_LB_TEST( boost::multiprecision::int1024_t(3), 1 );
    PRIVATE_LB_TEST( boost::multiprecision::int1024_t(65535), 15 );
    PRIVATE_LB_TEST( boost::multiprecision::int1024_t(65536), 16 );

    PRIVATE_LB_TEST( boost::multiprecision::uint1024_t(1), 0 );
    PRIVATE_LB_TEST( boost::multiprecision::uint1024_t(2), 1 );
    PRIVATE_LB_TEST( boost::multiprecision::uint1024_t(3), 1 );
    PRIVATE_LB_TEST( boost::multiprecision::uint1024_t(65535), 15 );
    PRIVATE_LB_TEST( boost::multiprecision::uint1024_t(65536), 16 );
#endif

    return boost::report_errors();
}
