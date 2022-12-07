// Boost.Geometry
// Unit Test

// Copyright (c) 2021 Oracle and/or its affiliates.

// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html

#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE test_algorithms_closest_points_pointlike_areal
#endif

#include "common.hpp"
#include "empty_geometry.hpp"

namespace bg = boost::geometry;

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_point_polygon_or_ring(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "point/polygon or ring closest_points tests" << std::endl;
#endif

    using Segment = bg::model::segment<Point>;
    using Ring = bg::model::ring<Point>;
    using Polygon = bg::model::polygon<Point>;

    using tester = test_geometry<Point, Ring, Segment>;

    tester::apply("POINT(1 1)",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(1 1,0.5 0.5)",
                  "SEGMENT(1 1,0.499962 0.500095)",
                  "SEGMENT(1 1,0.503314 0.496737)",
                  strategies);

    tester::apply("POINT(0.4 0.4)",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(0.4 0.4,0.4 0.4)",
                  strategies);

    using tester2 = test_geometry<Point, Polygon, Segment>;

    tester2::apply("POINT(1 1)",
                   "POLYGON((0 0,1 0,0 1,0 0))",
                   "SEGMENT(1 1,0.5 0.5)",
                   "SEGMENT(1 1,0.499962 0.500095)",
                   "SEGMENT(1 1,0.503314 0.496737)",
                   strategies);

    //default strategies
    tester2::apply("POINT(1 1)",
                   "POLYGON((0 0,1 0,0 1,0 0))",
                   "SEGMENT(1 1,0.5 0.5)",
                   "SEGMENT(1 1,0.499962 0.500095)",
                   "SEGMENT(1 1,0.503314 0.496737)",
                   strategies, true, true);

    tester2::apply("POINT(1 1)",
                   "POLYGON((0 0,1 0,0 1,0 0)(0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4))",
                   "SEGMENT(1 1,0.5 0.5)",
                   "SEGMENT(1 1,0.499962 0.500095)",
                   "SEGMENT(1 1,0.503314 0.496737)",
                   strategies);

    tester2::apply("POINT(1 1)",
                   "POLYGON((0 0,1 0,0 1,0 0)(0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4))",
                   "SEGMENT(1 1,0.5 0.5)",
                   "SEGMENT(1 1,0.499962 0.500095)",
                   "SEGMENT(1 1,0.503314 0.496737)",
                   strategies);

    tester2::apply("POINT(0.01 0.01)",
                   "POLYGON((0 0,1 0,0 1,0 0)(0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4))",
                   "SEGMENT(0.01 0.01,0.01 0.01)",
                   strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_point_box(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "point/box closest_points tests" << std::endl;
#endif

    using Segment = bg::model::segment<Point>;
    using Box = bg::model::box<Point>;
    using tester = test_geometry<Point, Box, Segment>;

    //all 8 possible placements of point around box
    tester::apply("POINT(5 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(5 25,10 20)",
                  strategies);

    tester::apply("POINT(15 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15 25,15 20)",
                  strategies);

    tester::apply("POINT(25 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(25 25,20 20)",
                  strategies);

    tester::apply("POINT(5 15)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(5 15,10 15)",
                  "SEGMENT(5 15,10 15.055045985544346)",
                  strategies);

    tester::apply("POINT(25 15)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(25 15,20 15)",
                  "SEGMENT(25 15,20 15.055045985544346)",
                  strategies);

    tester::apply("POINT(5 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(5 5,10 10)",
                  strategies);

    tester::apply("POINT(15 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15 5,15 10)",
                  strategies);

    tester::apply("POINT(25 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(25 5,20 10)",
                  strategies);

    //default strategies
    tester::apply("POINT(25 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(25 5,20 10)",
                  strategies, true, true);

    //inside
    tester::apply("POINT(15 15)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15 15,15 15)",
                  strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_point_multi_polygon(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "point/multi-polygon closest_points tests" << std::endl;
#endif

    using Segment = bg::model::segment<Point>;
    using Polygon = bg::model::polygon<Point>;
    using MultiPolygon = bg::model::multi_polygon<Polygon>;
    using tester = test_geometry<Point, MultiPolygon, Segment>;

    tester::apply("POINT(1 1)",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(1 1,0.5 0.5)",
                  "SEGMENT(1 1,0.499962 0.500095)",
                  "SEGMENT(1 1,0.503314 0.496737)",
                  strategies);

    tester::apply("POINT(1 1)",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(1 1,0.5 0.5)",
                  "SEGMENT(1 1,0.499962 0.500095)",
                  "SEGMENT(1 1,0.503314 0.496737)",
                  strategies, true, true);

    tester::apply("POINT(0.01 0.01)",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(0.01 0.01,0.01 0.01)",
                  strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_multi_point_polygon_or_ring(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "multi-point/polygon or ring closest_points tests" << std::endl;
#endif

    using Segment = bg::model::segment<Point>;
    using MultiPoint = bg::model::multi_point<Point>;
    using Ring = bg::model::ring<Point>;
    using Polygon = bg::model::polygon<Point>;
    using tester = test_geometry<MultiPoint, Ring, Segment>;

    tester::apply("MULTIPOINT((2 1),(1 1))",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(1 1,0.5 0.5)",
                  "SEGMENT(1 1,0.499962 0.500095)",
                  "SEGMENT(1 1,0.503314 0.496737)",
                  strategies);

    tester::apply("MULTIPOINT((2 1),(1 1))",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(1 1,0.5 0.5)",
                  "SEGMENT(1 1,0.499962 0.500095)",
                  "SEGMENT(1 1,0.503314 0.496737)",
                  strategies, true, true);

    tester::apply("MULTIPOINT((1 1),(0.01 0.01))",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(0.01 0.01,0.01 0.01)",
                  strategies);

    using tester2 = test_geometry<MultiPoint, Polygon, Segment>;

    tester2::apply("MULTIPOINT((2 1),(1 1))",
                   "POLYGON((0 0,1 0,0 1,0 0))",
                   "SEGMENT(1 1,0.5 0.5)",
                   "SEGMENT(1 1,0.499962 0.500095)",
                   "SEGMENT(1 1,0.503314 0.496737)",
                   strategies);

    tester2::apply("MULTIPOINT((2 1),(1 1))",
                   "POLYGON((0 0,1 0,0 1,0 0)(0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4))",
                   "SEGMENT(1 1,0.5 0.5)",
                   "SEGMENT(1 1,0.499962 0.500095)",
                   "SEGMENT(1 1,0.503314 0.496737)",
                   strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_multi_point_multi_polygon(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "multi-point/multi-polygon closest_points tests" << std::endl;
#endif

    using Segment = bg::model::segment<Point>;
    using MultiPoint = bg::model::multi_point<Point>;
    using Polygon = bg::model::polygon<Point>;
    using MultiPolygon = bg::model::multi_polygon<Polygon>;
    using tester = test_geometry<MultiPoint, MultiPolygon, Segment>;

    tester::apply("MULTIPOINT((2 1),(1 1))",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(1 1,0.5 0.5)",
                  "SEGMENT(1 1,0.499962 0.500095)",
                  "SEGMENT(1 1,0.503314 0.496737)",
                  strategies);

    tester::apply("MULTIPOINT((2 1),(0.01 0.01))",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(0.01 0.01,0.01 0.01)",
                  strategies);

    tester::apply("MULTIPOINT((2 1),(0.01 0.01))",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(0.01 0.01,0.01 0.01)",
                  strategies, true, true);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_multi_point_box(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "multi-point/box closest_points tests" << std::endl;
#endif

    using Segment = bg::model::segment<Point>;
    using Box = bg::model::box<Point>;
    using MultiPoint = bg::model::multi_point<Point>;
    using tester = test_geometry<MultiPoint, Box, Segment>;

    tester::apply("MULTIPOINT(5 25,0 0,0 20)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(5 25,10 20)",
                  strategies);

    tester::apply("MULTIPOINT(5 25,0 0,0 20)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(5 25,10 20)",
                  strategies, true, true);

    tester::apply("MULTIPOINT(5 25,0 0,15 15)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15 15,15 15)",
                  strategies);
}

//===========================================================================
//===========================================================================
//===========================================================================

template <typename Point, typename Strategies>
void test_all_pl_ar(Strategies strategies)
{
    test_closest_points_point_polygon_or_ring<Point>(strategies);
    test_closest_points_point_multi_polygon<Point>(strategies);
    //test_closest_points_point_box<Point>(strategies);

    test_closest_points_multi_point_polygon_or_ring<Point>(strategies);
    test_closest_points_multi_point_multi_polygon<Point>(strategies);
    //test_closest_points_multi_point_box<Point>(strategies);

    test_more_empty_input_pointlike_areal<Point>(strategies);
}

BOOST_AUTO_TEST_CASE( test_all_pointlike_areal )
{
    test_all_pl_ar<car_point>(cartesian());

    test_all_pl_ar<sph_point>(spherical());
    test_all_pl_ar<sph_point>(spherical(
        bg::formula::mean_radius<double>(bg::srs::spheroid<double>())));

    test_all_pl_ar<geo_point>(andoyer());
    test_all_pl_ar<geo_point>(thomas());
    test_all_pl_ar<geo_point>(vincenty());
}

