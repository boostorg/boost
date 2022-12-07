// Boost.Geometry
// Unit Test

// Copyright (c) 2022 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <test_geometries/const_point.hpp>

#include <boost/geometry/algorithms/covered_by.hpp>
#include <boost/geometry/algorithms/crosses.hpp>
#include <boost/geometry/algorithms/disjoint.hpp>
#include <boost/geometry/algorithms/intersects.hpp>
#include <boost/geometry/algorithms/overlaps.hpp>
#include <boost/geometry/algorithms/relate.hpp>
#include <boost/geometry/algorithms/touches.hpp>
#include <boost/geometry/algorithms/within.hpp>


#include <geometry_test_common.hpp>


template <typename Geometry1, typename Geometry2>
void test_relate_const(Geometry1 const &geometry1, Geometry2 const &geometry2,
    bool exp_cov, bool exp_crosses, bool exp_disjoint, bool exp_intersects,
    bool exp_overlaps, bool exp_touches, bool exp_relate, bool exp_within)
{
    namespace bg = boost::geometry;

    BOOST_CHECK_EQUAL(exp_cov, bg::covered_by(geometry1, geometry2));
    BOOST_CHECK_EQUAL(exp_crosses, bg::crosses(geometry1, geometry2));
    BOOST_CHECK_EQUAL(exp_disjoint, bg::disjoint(geometry1, geometry2));
    BOOST_CHECK_EQUAL(exp_intersects, bg::intersects(geometry1, geometry2));
    BOOST_CHECK_EQUAL(exp_overlaps, bg::overlaps(geometry1, geometry2));
    BOOST_CHECK_EQUAL(exp_touches, bg::touches(geometry1, geometry2));
    BOOST_CHECK_EQUAL(exp_relate, bg::relate(geometry1, geometry2, bg::de9im::mask("F0F******")));
    BOOST_CHECK_EQUAL(exp_within, bg::within(geometry1, geometry2));
}


int test_main(int, char* [])
{
    ring_of_const_point const rectangle{{2, 2}, {2, 4}, {4, 4}, {4, 2}, {2, 2}};
    ring_of_const_point const triangle{{1, 2}, {4, 5}, {5, 4}, {1, 2}};
    linestring_of_const_point const diagonal{{0, 0}, {6, 6}};
    linestring_of_const_point const horizontal{{0, 3}, {6, 3}};

    // areal/areal
    test_relate_const(rectangle, triangle, false, false, false, true, true, false, false, false);

    // point/areal
    test_relate_const(rectangle.front(), triangle, false, false, true, false, false, false, false, false);

    // point/linear
    test_relate_const(rectangle.front(), diagonal, true, false, false, true, false, false, false, true);

    // linear/linear
    test_relate_const(horizontal, diagonal, false, true, false, true, false, false, false, false);

    return 0;
}
