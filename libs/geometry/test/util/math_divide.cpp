// Boost.Geometry
// Unit Test

// Copyright (c) 2022 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <geometry_test_common.hpp>

#include <boost/geometry/util/math.hpp>

namespace bgm = bg::math;

template <typename T>
void test_integer()
{
    BOOST_CHECK_EQUAL(0, bgm::divide<T>(0, 2));

    BOOST_CHECK_EQUAL(1, bgm::divide<T>(1, 2));
    BOOST_CHECK_EQUAL(-1, bgm::divide<T>(-1, 2));
    BOOST_CHECK_EQUAL(-1, bgm::divide<T>(1, -2));
    BOOST_CHECK_EQUAL(1, bgm::divide<T>(-1, -2));

    BOOST_CHECK_EQUAL(2, bgm::divide<T>(4, 2));
    BOOST_CHECK_EQUAL(3, bgm::divide<T>(5, 2));

    BOOST_CHECK_EQUAL(9, bgm::divide<T>(94, 10));
    BOOST_CHECK_EQUAL(10, bgm::divide<T>(95, 10));
    BOOST_CHECK_EQUAL(10, bgm::divide<T>(99, 10));

    BOOST_CHECK_EQUAL(-9, bgm::divide<T>(94, -10));
    BOOST_CHECK_EQUAL(-10, bgm::divide<T>(95, -10));
    BOOST_CHECK_EQUAL(-10, bgm::divide<T>(99, -10));

    BOOST_CHECK_EQUAL(-9, bgm::divide<T>(-94, 10));
    BOOST_CHECK_EQUAL(-10, bgm::divide<T>(-95, 10));
    BOOST_CHECK_EQUAL(-10, bgm::divide<T>(-99, 10));

    BOOST_CHECK_EQUAL(9, bgm::divide<T>(-94, -10));
    BOOST_CHECK_EQUAL(10, bgm::divide<T>(-95, -10));
    BOOST_CHECK_EQUAL(10, bgm::divide<T>(-99, -10));

    BOOST_CHECK_EQUAL(5, bgm::divide<T>(4567, 1000));
}

template <typename T>
void test_floating_point()
{
    constexpr double eps = 1.0e-6;
    BOOST_CHECK_CLOSE(4.567, bgm::divide<T>(4567, 1000), eps);
}

int test_main(int, char* [])
{
    test_integer<int>();
    test_floating_point<double>();
    
    return 0;
}
