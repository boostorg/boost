// Boost.Geometry
// Unit Test

// Copyright (c) 2022 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <geometry_test_common.hpp>

#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/ring.hpp>
#include <boost/geometry/strategies/cartesian/buffer_join_round.hpp>
#include <boost/geometry/strategies/cartesian/buffer_join_round_by_divide.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/distance.hpp>
#include <boost/geometry/algorithms/equals.hpp>

template <typename JoinStrategy, typename P, typename T>
void test_join(JoinStrategy const& join,
               P const& ip, P const& vertex,
               P const& perp1, P const& perp2,
               T const& buffer_distance, std::size_t expected_size)
{
    // Use a deque to be able to use push_front
    bg::model::ring<P, true, true, std::deque> output_ring;

    auto const result = join.apply(ip, vertex, perp1, perp2, buffer_distance, output_ring);
    BOOST_CHECK_EQUAL(true, result);
    BOOST_CHECK_EQUAL(expected_size , output_ring.size());
    BOOST_CHECK(bg::equals(perp1, output_ring.front()));
    BOOST_CHECK(bg::equals(perp2, output_ring.back()));

    // All the generated points should be located
    // at or close to the specified buffer distance from the vertex
    for (const auto& p : output_ring)
    {
        auto const d = bg::distance(vertex, p);
        auto const fraction = d / buffer_distance;
        BOOST_CHECK_MESSAGE(fraction > 0.92 && fraction < 1.02,
            "Unexpected distance = " << d << " fraction=" << fraction);
    }

    // Start with the vertex itself, and close it,
    // to calculate the area of the buffered corner
    output_ring.push_front(vertex);
    output_ring.push_back(vertex);

    // Area should be around 0.25 * PI * sqr(specified buffer distance)
    // It is a bit less because there are less than infinite points in the circle
    double const area = bg::area(output_ring) ;
    double const expected = 0.25 * buffer_distance * buffer_distance * 3.1415;
    double const fraction_area = area / expected;
    BOOST_CHECK_MESSAGE(fraction_area > 0.95 && fraction_area < 1.05,
        "Unexpected area = " << area << " fraction=" << fraction_area);
}

template <typename P>
void test_case(typename bg::coordinate_type<P>::type c)
{
    // For round strategy (36 points per circle) we expect 10 points in a corner.
    // Using divide on level 3, we get 17
    test_join(bg::strategy::buffer::join_round(36), P{c, c}, P{0, 0}, P{0, c}, P{c, 0}, double(c), 10);
    test_join(bg::strategy::buffer::join_round_by_divide(3), P{c, c}, P{0, 0}, P{0, c}, P{c, 0}, double(c), 17);
}

template <typename P>
void test_all()
{
    // Also test for large coordinates.
    // For integer coordinates there was an overflow with them (now fixed)
    test_case<P>(100);
    test_case<P>(10000);
    test_case<P>(1000000);
    test_case<P>(100000000);
}

int test_main(int, char *[])
{
    test_all<bg::model::point<int, 2, bg::cs::cartesian>>();
    test_all<bg::model::point<float, 2, bg::cs::cartesian>>();
    test_all<bg::model::point<double, 2, bg::cs::cartesian>>();

    return 0;
}
