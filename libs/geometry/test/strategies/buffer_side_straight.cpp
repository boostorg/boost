// Boost.Geometry
// Unit Test

// Copyright (c) 2022 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <geometry_test_common.hpp>

#include <boost/geometry/geometries/point.hpp>

#include <boost/geometry/strategies/cartesian/buffer_side_straight.hpp>
#include <boost/geometry/strategies/agnostic/buffer_distance_symmetric.hpp>

#include <boost/geometry/algorithms/distance.hpp>

template <typename P>
void test_side_straight(P const &input_p1, P const &input_p2)
{
    using bg::strategy::buffer::side_straight;

    bg::strategy::buffer::distance_symmetric<typename bg::coordinate_type<P>::type> ds(1.0);

    std::vector<P> output_range;

    auto const result = side_straight::apply(input_p1, input_p2, bg::strategy::buffer::buffer_side_right, ds, output_range);

    BOOST_CHECK_EQUAL(bg::strategy::buffer::result_normal, result);
    BOOST_CHECK_EQUAL(2u, output_range.size());

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    if (output_range.size() == 2)
    {
        std::cout << bg::distance(output_range.front(), output_range.back()) << std::endl;
    }
#endif
}

template <typename P>
void test_all()
{
    // Also test for large coordinates.
    // For integer coordinates there was an overflow with them (now fixed)
    test_side_straight(P{0, 0}, P{1, 1});
    test_side_straight(P{0, 0}, P{100, 100});
    test_side_straight(P{0, 0}, P{10000, 10000});
    test_side_straight(P{0, 0}, P{1000000, 1000000});
    test_side_straight(P{0, 0}, P{100000000, 100000000});
}

int test_main(int, char *[])
{
    test_all<bg::model::point<int, 2, bg::cs::cartesian>>();
    test_all<bg::model::point<double, 2, bg::cs::cartesian>>();

    return 0;
}
