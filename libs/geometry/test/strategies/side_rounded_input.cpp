// Boost.Geometry
// Unit Test

// Copyright (c) 2022 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <geometry_test_common.hpp>

#include <boost/geometry/geometries/point.hpp>

#include <boost/geometry/strategy/cartesian/side_robust.hpp>
#include <boost/geometry/strategy/cartesian/side_by_triangle.hpp>
#include <boost/geometry/strategies/cartesian/side_rounded_input.hpp>

#include <boost/geometry/util/math.hpp>

template <typename Point>
void test_side_both_ways(std::string const& /*case_id*/, Point const& p1, Point const& p2, Point const& p3,
                   int expected, int expected_rounded_input)
{
    int const side_cart = bg::strategy::side::services::default_strategy<bg::cartesian_tag>::type::apply(p1, p2, p3);
    int const side_ri = bg::strategy::side::side_rounded_input<>::apply(p1, p2, p3);

    BOOST_CHECK_EQUAL(side_cart, expected);
    BOOST_CHECK_EQUAL(side_ri, expected_rounded_input);
}

template <typename Point>
void test_side(std::string const& case_id, Point const& p1, Point const& p2, Point const& p3,
                   int expected, int expected_rounded_input = -99)
{
    int const eri = expected_rounded_input == -99 ? expected : expected_rounded_input;
    test_side_both_ways(case_id, p1, p2, p3, expected, eri);
    test_side_both_ways(case_id, p2, p1, p3, -expected, -eri);
}


template <typename Point>
void test_all()
{
    Point const p1(0.0, 0.0);
    Point const p2(10.0, 10.0);
    Point const left(5.0, 6.0);
    Point const right(5.0, 4.0);
    Point const on_segment(5.0, 5.0);

    // This point is just off. For double, side_rounded_input should say it's on top
    Point const just(5.0, 5.0 + 1.0e-15);

    test_side("left", p1, p2, left, 1);
    test_side("right", p1, p2, right, -1);
    test_side("colinear", p1, p2, on_segment, 0);
    test_side("just", p1, p2, just, 1, 0);
}

int test_main(int, char* [])
{
    test_all<bg::model::point<double, 2, bg::cs::cartesian> >();

    return 0;
}
