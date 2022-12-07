// Boost.Geometry
// Unit Test

// Copyright (c) 2022 Oracle and/or its affiliates.

// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <limits>
#include <type_traits>

#include <geometry_test_common.hpp>

#include <boost/geometry/util/condition.hpp>
#include <boost/geometry/util/math.hpp>
#include <boost/geometry/util/normalize_spheroidal_box_coordinates.hpp>
#include <boost/geometry/util/normalize_spheroidal_coordinates.hpp>


namespace bgm = bg::math;

template <typename T, std::enable_if_t<std::numeric_limits<T>::has_quiet_NaN, int> = 0>
void test_equals(T const& v, T const& r)
{
    bool const check = bg::math::equals(v, r) || (std::isnan(v) && std::isnan(r));
    BOOST_CHECK_MESSAGE(check, v << " different than " << r);
}

template <typename T, std::enable_if_t<! std::numeric_limits<T>::has_quiet_NaN, int> = 0>
void test_equals(T const& v, T const& r)
{
    bool const check = bg::math::equals(v, r);
    BOOST_CHECK_MESSAGE(check, v << " different than " << r);
}

template <typename T>
void test_one(T lon, T lat, T const& res_lon, T const& res_lat)
{
    bg::math::normalize_spheroidal_coordinates<bg::degree>(lon, lat);
    test_equals(lon, res_lon);
    test_equals(lat, res_lat);
}

template <typename T>
void test_box(T lon1, T lat1, T lon2, T lat2,
              T const& res_lon1, T const& res_lat1, T const& res_lon2, T const& res_lat2)
{
    bg::math::normalize_spheroidal_box_coordinates<bg::degree>(lon1, lat1, lon2, lat2);
    test_equals(lon1, res_lon1);
    test_equals(lat1, res_lat1);
    test_equals(lon2, res_lon2);
    test_equals(lat2, res_lat2);
}

template <typename T>
void test_all()
{
    test_one<T>(270, 0, -90, 0);

    test_one<T>(-180, 10, 180, 10);
    test_one<T>(-180, -90, 0, -90);

    if (BOOST_GEOMETRY_CONDITION(std::numeric_limits<T>::has_quiet_NaN))
    {
        T nan = std::numeric_limits<T>::quiet_NaN();
        test_one<T>(nan, 10, nan, 10);
        test_one<T>(10, nan, 10, nan);
        test_one<T>(nan, nan, nan, nan);

        test_box<T>(nan, 10, 20, 20, nan, 10, 20, 20);
        test_box<T>(10, nan, 20, 20, 10, nan, 20, 20);
        test_box<T>(10, 10, nan, 20, 10, 10, nan, 20);
        test_box<T>(10, 10, 20, nan, 10, 10, 20, nan);

        test_box<T>(nan, 10, 270, 20, nan, 10, -90, 20);
        test_box<T>(270, 10, nan, 20, -90, 10, nan, 20);
    }
}

int test_main(int, char* [])
{
    test_all<int>();
    test_all<float>();
    test_all<double>();
    
    return 0;
}
