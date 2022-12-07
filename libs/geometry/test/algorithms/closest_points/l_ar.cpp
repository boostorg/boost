// Boost.Geometry
// Unit Test

// Copyright (c) 2021, Oracle and/or its affiliates.

// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html

#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE test_closest_points_linear_areal
#endif

#define BOOST_GEOMETRY_TEST_DEBUG

#include "common.hpp"
#include "empty_geometry.hpp"

namespace bg = boost::geometry;

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_segment_polygon_or_ring(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "segment/polygon or ring closest_points tests" << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::ring<Point> Ring;
    typedef bg::model::polygon<Point> Polygon;
    typedef test_geometry<Segment, Ring, Segment> tester;

    tester::apply("SEGMENT(2 0,0 2)",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(1.5 0.5,1 0)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies);

    //geometries intersect
    //intersect boundary of ring
    tester::apply("SEGMENT(1 1,0.1 0.1)",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                  strategies);

    //intersect interior of ring
    tester::apply("SEGMENT(0.2 0.2,0.1 0.1)",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(0.2 0.2,0.2 0.2)",
                  strategies);

    typedef test_geometry<Segment, Polygon, Segment> tester2;

    tester2::apply("SEGMENT(2 0,0 2)",
                   "POLYGON((0 0,1 0,0 1,0 0))",
                   "SEGMENT(1.5 0.5,1 0)",
                   "SEGMENT(0.50019 1.50021,0 1)",
                   "SEGMENT(1.496909 0.503379,1 0)",
                   strategies);

    tester2::apply("SEGMENT(2 0,0 2)",
                   "POLYGON((0 0,1 0,0 1,0 0)(0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4))",
                   "SEGMENT(1.5 0.5,1 0)",
                   "SEGMENT(0.50019 1.50021,0 1)",
                   "SEGMENT(1.496909 0.503379,1 0)",
                   strategies);

    tester2::apply("SEGMENT(2 0,0 2)",
                   "POLYGON((0 0,1 0,0 1,0 0)(0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4))",
                   "SEGMENT(1.5 0.5,1 0)",
                   "SEGMENT(0.50019 1.50021,0 1)",
                   "SEGMENT(1.496909 0.503379,1 0)",
                   strategies, true, true);

    //geometries intersect
    //intersect boundary of ring
    tester2::apply("SEGMENT(1 1,0.1 0.1)",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                  strategies);

    //intersect interior of ring
    tester2::apply("SEGMENT(0.2 0.2,0.1 0.1)",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(0.2 0.2,0.2 0.2)",
                  strategies);

    //intersect boundary of interior ring
    tester2::apply("SEGMENT(0.9 0.9,0.1 0.1)",
                  "POLYGON((-1 -1,2 0,0 2,-1 -1)(0 0,0 1,1 0,0 0))",
                   "SEGMENT(0.5 0.5,0.5 0.5)",
                   "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                  strategies);

    //intersect boundary of exterior ring
    tester2::apply("SEGMENT(0.9 0.9,2 2)",
                   "POLYGON((-1 -1,2 0,0 2,-1 -1)(0 0,0 1,1 0,0 0))",
                   "SEGMENT(1 1,1 1)",
                   "SEGMENT(1.0002 1.00026,1.0002 1.00026)",
                  strategies);

    //intersect interior
    tester2::apply("SEGMENT(0 -0.5,1 -0.5)",
                   "POLYGON((-1 -1,0 2,2 0,-1 -1)(0 0,1 0,0 1,0 0))",
                   "SEGMENT(0.5 -0.5,0.5 -0.5)",
                   "SEGMENT(0.500575 -0.500019,0.500575 -0.500019)",
                  strategies);

}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_segment_multi_polygon(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "segment/multi-polygon or ring closest_points tests" << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::polygon<Point> Polygon;
    typedef bg::model::multi_polygon<Polygon> MultiPolygon;
    typedef test_geometry<Segment, MultiPolygon, Segment> tester;

    tester::apply("SEGMENT(2 0,0 2)",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(1.5 0.5,1 0)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies);

    tester::apply("SEGMENT(2 0,0 2)",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(1.5 0.5,1 0)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies, true, true);

    //geometries intersect
    //intersect boundary of ring
    tester::apply("SEGMENT(1 1,0.1 0.1)",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                  strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_segment_box(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "segment/box closest_points tests" << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::box<Point> Box;
    typedef test_geometry<Segment, Box, Segment> tester;

    tester::apply("SEGMENT(2 0,0 2)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(1 1,10 10)",
                  "SEGMENT(0.922834 1.07763,10 10)",
                  "SEGMENT(0.983761 1.0167,10 10)",
                  strategies);

    tester::apply("SEGMENT(5 0,5 15)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(5 15,10 15)",
                  "SEGMENT(5 15,10 15.0547)",
                  "SEGMENT(5 15,10 15.055)",
                  strategies);

    tester::apply("SEGMENT(0 5,15 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15 5,15 10)",
                  "SEGMENT(9.98098 5.03848,10 10)",
                  strategies);

    tester::apply("SEGMENT(0 5,20 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20 5,20 10)",
                  "SEGMENT(10 5.07673,10 10)",
                  "SEGMENT(10 5.07725,10 10)",
                  strategies);

    tester::apply("SEGMENT(10 5, 20 6)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20 6,20 10)",
                  strategies);

    tester::apply("SEGMENT(0 0, 0 10)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(0 10,10 10)",
                  "SEGMENT(0 10,10 10.1511)",
                  "SEGMENT(0 10,10 10.1521)",
                  strategies);

    tester::apply("SEGMENT(0 5, 125 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20 5,20 10)",
                  "SEGMENT(20.2581 7.98472,20 10)",
                  "SEGMENT(20.2533 8.02139,20 10)",
                  strategies);

    // Test segments above box
    tester::apply("SEGMENT(0 25, 9 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(9 25,10 20)",
                  strategies);
    tester::apply("SEGMENT(0 25, 10 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(10 25,10 20)",
                  strategies);
    tester::apply("SEGMENT(0 25, 11 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(11 25,11 20)",
                  strategies);
    tester::apply("SEGMENT(0 25, 22 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(10 25,10 20)",
                  "SEGMENT(20.3808 25.1117,20 20)",
                  "SEGMENT(20.3787 25.1124,20 20)",
                  strategies);

    // Segments left-right of box
    tester::apply("SEGMENT(0 5,9 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(9 5,10 10)",
                  strategies);
    tester::apply("SEGMENT(21 10, 21 15)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(21 15,20 15)",
                  "SEGMENT(21 15,20 15.0022)",
                  strategies);


    //Segments on corners of box
    //left-top corner
    //generic
    tester::apply("SEGMENT(9 19.5, 11 21)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(9.88 20.16,10 20)",
                  "SEGMENT(9.8677 20.1571,10 20)",
                  strategies);
    //degenerate for cartesian
    tester::apply("SEGMENT(9 19, 11 21)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(10 20,10 20)",
                  "SEGMENT(9.99514 20.0043,10 20)",
                  strategies);
    //left-bottom corner
    //generic
    tester::apply("SEGMENT(8.5 11, 11 9)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(9.90244 9.87805,10 10)",
                  "SEGMENT(9.90395 9.88218,10 10)",
                  strategies);
    //right-top corner
    //generic
    tester::apply("SEGMENT(19 21, 21 19.5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20.12 20.16,20 20)",
                  "SEGMENT(20.1323 20.1571,20 20)",
                  strategies);

    //right-bottom corner
    //generic
    tester::apply("SEGMENT(19 9, 21 10.5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20.12 9.84,20 10)",
                  "SEGMENT(20.1197 9.84293,20 10)",
                  strategies);
    //Segment and box on different hemispheres (for non-cartesian)
    tester::apply("SEGMENT(10 20, 15 30)",
                  "BOX(10 -20,20 -10)",
                  "SEGMENT(10 20,10 -10)",
                  strategies);
    tester::apply("SEGMENT(0 10, 12 10)",
                  "BOX(10 -20,20 -10)",
                  "SEGMENT(12 10,12 -10)",
                  strategies);
    tester::apply("SEGMENT(10 10, 20 10)",
                  "BOX(10 -20,20 -10)",
                  "SEGMENT(10 10,10 -10)",
                  strategies);
    tester::apply("SEGMENT(0 -10, 12 -10)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(12 -10,12 10)",
                  strategies);
    tester::apply("SEGMENT(10 -10, 20 -10)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20 -10,20 10)",
                  "SEGMENT(10 -10,10 10)",
                  strategies);
    //Box crossing equator
    tester::apply("SEGMENT(10 20, 15 30)",
                  "BOX(10 -10,20 10)",
                  "SEGMENT(10 20,10 10)",
                  strategies);
    tester::apply("SEGMENT(10 -20, 15 -30)",
                  "BOX(10 -10,20 10)",
                  "SEGMENT(10 -20,10 -10)",
                  strategies);

    //Box crossing prime meridian
    tester::apply("SEGMENT(-5 25, 10 30)",
                  "BOX(-10 10,15 20)",
                  "SEGMENT(-5 25,-5 20)",
                  strategies);
    tester::apply("SEGMENT(-5 5, 10 7)",
                  "BOX(-10 10,15 20)",
                  "SEGMENT(10 7,10 10)",
                  strategies);
    tester::apply("SEGMENT(-5 5, 10 5)",
                  "BOX(-10 10,15 20)",
                  "SEGMENT(10 5,10 10)",
                  "SEGMENT(10 5.04321,10 10)",
                  strategies);
    //Geometries in south hemisphere
    tester::apply("SEGMENT(10 -30, 15 -30)",
                  "BOX(10 -20,20 -10)",
                  "SEGMENT(15 -30,15 -20)",
                  "SEGMENT(10 -30,10 -20)",
                  strategies);
    //Segments in boxes corner
    tester::apply("SEGMENT(17 21, 25 20)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20.0769 20.6154,20 20)",
                  "SEGMENT(20.0886 20.6593,20 20)",
                  strategies);
    tester::apply("SEGMENT(17 21, 0 20)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(9.96552 20.5862,10 20)",
                  "SEGMENT(9.9552 20.7908,10 20)",
                  strategies);
    tester::apply("SEGMENT(17 5, 0 10)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(9.20382 7.29299,10 10)",
                  "SEGMENT(9.20532 7.38795,10 10)",
                  "SEGMENT(9.21495 7.38567,10 10)",
                  strategies);
    tester::apply("SEGMENT(17 5, 25 9)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(21.4 7.2,20 10)",
                  "SEGMENT(21.4147 7.23283,20 10)",
                  "SEGMENT(21.3999 7.22557,20 10)",
                  strategies);

    tester::apply("SEGMENT(17 5, 25 9)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(21.4 7.2,20 10)",
                  "SEGMENT(21.4147 7.23283,20 10)",
                  "SEGMENT(21.3999 7.22557,20 10)",
                  strategies, true, true);

    tester::apply("SEGMENT(50 50,50 50)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(50 50,20 20)",
                  strategies);

    //intersection
    tester::apply("SEGMENT(15 15,15 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15 15,15 15)",
                  strategies);

    //intersection by crossing box
    tester::apply("SEGMENT(15 21, 21 15)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20 16,20 16)",
                  "SEGMENT(20 16.0403,20 16.0403)",
                  "SEGMENT(20 16.0405,20 16.0405)",
                  strategies);

    tester::apply("SEGMENT(15 21, 16 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15.6875 10,15.6875 10)",
                  "SEGMENT(15.7003 10,15.7003 10)",
                  strategies);

    tester::apply("SEGMENT(16 5,15 21)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15.6875 10,15.6875 10)",
                  "SEGMENT(15.7003 10,15.7003 10)",
                  strategies);

    tester::apply("SEGMENT(16 21,15 6)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15.2667 10,15.2667 10)",
                  "SEGMENT(15.2554 10,15.2554 10)",
                  strategies);

    tester::apply("SEGMENT(15 6,16 21)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15.2667 10,15.2667 10)",
                  "SEGMENT(15.2554 10,15.2554 10)",
                  strategies);

    tester::apply("SEGMENT(15 5, 16 21)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15.3125 10,15.3125 10)",
                  "SEGMENT(15.2997 10,15.2997 10)",
                  "SEGMENT(15.2996 10,15.2996 10)",
                  strategies);

    tester::apply("SEGMENT(0 0,100 3)",
                  "BOX(0 1,100 2)",
                  "SEGMENT(33.3333 1,33.3333 1)",
                  "SEGMENT(19.1476 1,19.1476 1)",
                  "SEGMENT(19.0629 1,19.0629 1)",
                  strategies);

    tester::apply("SEGMENT(0 0,3 70)",
                  "BOX(1 0,2 70)",
                  "SEGMENT(1 23.3333,1 23.3333)",
                  "SEGMENT(1 42.4958,1 42.4958)",
                  "SEGMENT(1 42.5308,1 42.5308)",
                  strategies);

    tester::apply("SEGMENT(0 1,100 1)",
                  "BOX(80 1.1,100 1.2)",
                  "SEGMENT(100 1,100 1.1)",
                  "SEGMENT(95.0021 1.1,95.0021 1.1)",
                  "SEGMENT(95.0509 1.1,95.0509 1.1)",
                  strategies);

    tester::apply("SEGMENT(0 1,100 1)",
                  "BOX(0 1.1,100 1.2)",
                  "SEGMENT(100 1,100 1.1)",
                  "SEGMENT(4.99795 1.1,4.99795 1.1)",
                  "SEGMENT(4.9493 1.1,4.9493 1.1)",
                  strategies);

    tester::apply("SEGMENT(0 1,100 1)",
                  "BOX(0 1.1,100 1.2)",
                  "SEGMENT(100 1,100 1.1)",
                  "SEGMENT(4.99795 1.1,4.99795 1.1)",
                  "SEGMENT(4.9493 1.1,4.9493 1.1)",
                  strategies);

    //Case A: box intersects without containing the vertex
    tester::apply("SEGMENT(0 5,120 5)",
                  "BOX(0 6,120 7)",
                  "SEGMENT(120 5,120 6)",
                  "SEGMENT(6.91815 6,6.91815 6)",
                  "SEGMENT(6.83635 6,6.83635 6)",
                  strategies);
    tester::apply("SEGMENT(0 -5,120 -5)",
                  "BOX(0 -7,120 -6)",
                  "SEGMENT(0 -5,0 -6)",
                  "SEGMENT(105.435 -7,105.435 -7)",
                  "SEGMENT(105.719 -7,105.719 -7)",
                  strategies);

    //Case B: box intersects and contains the vertex
    tester::apply("SEGMENT(0 5,120 5)",
                  "BOX(0 9,120 10)",
                  "SEGMENT(120 5,120 9)",
                  "SEGMENT(34.8464 9,34.8464 9)",
                  "SEGMENT(34.2138 9,34.2138 9)",
                  strategies);
    tester::apply("SEGMENT(0 -5,120 -5)",
                  "BOX(0 -10,120 -9)",
                  "SEGMENT(0 -5,0 -9)",
                  "SEGMENT(85.1536 -9,85.1536 -9)",
                  "SEGMENT(85.7839 -9,85.7839 -9)",
                  strategies);

    tester::apply("SEGMENT(15 15,15 15)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15 15,15 15)",
                  strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_linestring_polygon_or_ring(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "linestring/polygon or ring closest_points tests" << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::ring<Point> Ring;
    typedef bg::model::polygon<Point> Polygon;
    typedef test_geometry<Linestring, Ring, Segment> tester;

    tester::apply("LINESTRING(2 0,0 2)",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(1.5 0.5,1 0)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies);

    tester::apply("LINESTRING(2 0,0 2)",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(1.5 0.5,1 0)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies, true, true);

    //geometries intersect
    //intersect boundary of ring
    tester::apply("LINESTRING(1 1,0.1 0.1,0.2 0.1)",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                  strategies);

    //intersect interior of ring
    tester::apply("LINESTRING(0.2 0.2,0.1 0.1,0.2 0.1)",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(0.2 0.2,0.2 0.2)",
                  strategies);

    typedef test_geometry<Linestring, Polygon, Segment> tester2;

    tester2::apply("LINESTRING(2 0,0 2)",
                   "POLYGON((0 0,1 0,0 1,0 0))",
                   "SEGMENT(1.5 0.5,1 0)",
                   "SEGMENT(0.50019 1.50021,0 1)",
                   "SEGMENT(1.496909 0.503379,1 0)",
                   strategies);

    tester2::apply("LINESTRING(2 0,0 2)",
                   "POLYGON((0 0,1 0,0 1,0 0)(0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4))",
                   "SEGMENT(1.5 0.5,1 0)",
                   "SEGMENT(0.50019 1.50021,0 1)",
                   "SEGMENT(1.496909 0.503379,1 0)",
                   strategies);

    //geometries intersect
    //intersect boundary of ring
    tester2::apply("LINESTRING(1 1,0.1 0.1,0.1 0.2)",
                   "POLYGON((0 0,1 0,0 1,0 0))",
                   "SEGMENT(0.5 0.5,0.5 0.5)",
                   "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                   strategies);

    //intersect interior of ring
    tester2::apply("LINESTRING(0.2 0.2,0.1 0.1,0.1 0.2)",
                   "POLYGON((0 0,1 0,0 1,0 0))",
                   "SEGMENT(0.2 0.2,0.2 0.2)",
                   strategies);

    //intersect boundary of interior ring
    tester2::apply("LINESTRING(0.9 0.9,0.1 0.1,0.1 0.2)",
                   "POLYGON((-1 -1,2 0,0 2,-1 -1)(0 0,0 1,1 0,0 0))",
                   "SEGMENT(0.5 0.5,0.5 0.5)",
                   "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                   strategies);

    //intersect boundary of exterior ring
    tester2::apply("LINESTRING(0.9 0.9,2 2,1 2)",
                   "POLYGON((-1 -1,2 0,0 2,-1 -1)(0 0,0 1,1 0,0 0))",
                   "SEGMENT(1 1,1 1)",
                   "SEGMENT(1.0002 1.00026,1.0002 1.00026)",
                   strategies);

    //intersect interior of polygon
    tester2::apply("LINESTRING(0 -0.5,1 -0.5,1.1 -0.5)",
                   "POLYGON((-1 -1,0 2,2 0,-1 -1)(0 0,1 0,0 1,0 0))",
                   "SEGMENT(0.5 -0.5,0.5 -0.5)",
                   "SEGMENT(0.500575 -0.500019,0.500575 -0.500019)",
                   strategies);
}


//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_linestring_multi_polygon(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "linestring/multi-polygon closest_points tests" << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::polygon<Point> Polygon;
    typedef bg::model::multi_polygon<Polygon> MultiPolygon;
    typedef test_geometry<Linestring, MultiPolygon, Segment> tester;

    tester::apply("LINESTRING(2 0,0 2)",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(1.5 0.5,1 0)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies);

    tester::apply("LINESTRING(2 0,0 2)",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(1.5 0.5,1 0)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies, true, true);

    //geometries intersect
    //intersect boundary of ring
    tester::apply("LINESTRING(1 1,0.1 0.1,0.1 0.2)",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                  strategies);

    //intersect boundary of ring
    tester::apply("LINESTRING(1 1,0.1 0.1,0.1 0.2)",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                                ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                  strategies);

    //intersect interior of polygon
    tester::apply("LINESTRING(0 -0.5,1 -0.5,1.1 -0.5)",
                  "MULTIPOLYGON(((3 0,3 3,4 3,3 0)),\
                                ((-1 -1,0 2,2 0,-1 -1)(0 0,1 0,0 1,0 0)))",
                  "SEGMENT(0.5 -0.5,0.5 -0.5)",
                  "SEGMENT(0.500575 -0.500019,0.500575 -0.500019)",
                  strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_linestring_box(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "linestring/box closest_points tests" << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::box<Point> Box;
    typedef test_geometry<Linestring, Box, Segment> tester;

    tester::apply("LINESTRING(2 0,0 2,0 0,-1 -1)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(1 1,10 10)",
                  "SEGMENT(0.922834 1.07763,10 10)",
                  "SEGMENT(0.983761 1.0167,10 10)",
                  strategies);

    tester::apply("LINESTRING(2 0,0 2,0 0,-1 -1)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(1 1,10 10)",
                  "SEGMENT(0.922834 1.07763,10 10)",
                  "SEGMENT(0.983761 1.0167,10 10)",
                  strategies, true, true);

    tester::apply("LINESTRING(50 50)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(50 50,20 20)",
                  strategies);

    //intersection
    tester::apply("LINESTRING(15 15,15 25,20 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15 15,15 15)",
                  strategies);
    tester::apply("LINESTRING(15 21,21 15,22 25)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20 16,20 16)",
                  "SEGMENT(20 16.0403,20 16.0403)",
                  "SEGMENT(20 16.0405,20 16.0405)",
                  strategies);
    tester::apply("LINESTRING(15 21,16 5,25 5)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15.6875 10,15.6875 10)",
                  "SEGMENT(15.7003 10,15.7003 10)",
                  strategies);
    tester::apply("LINESTRING(15 21,16 5,17 15)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15.6875 10,15.6875 10)",
                  "SEGMENT(15.7003 10,15.7003 10)",
                  strategies);

    tester::apply("LINESTRING(0 0,100 3,100 4)",
                  "BOX(0 1,100 2)",
                  "SEGMENT(33.3333 1,33.3333 1)",
                  "SEGMENT(19.1476 1,19.1476 1)",
                  "SEGMENT(19.0629 1,19.0629 1)",
                  strategies);

    tester::apply("LINESTRING(15 15)",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15 15,15 15)",
                  strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_multi_linestring_polygon_or_ring(Strategies
                                                          const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "multi-linestring/polygon or ring closest_points tests"
              << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::multi_linestring<Linestring> MultiLinestring;
    typedef bg::model::ring<Point> Ring;
    typedef bg::model::polygon<Point> Polygon;
    typedef test_geometry<MultiLinestring, Ring, Segment> tester;

    tester::apply("MULTILINESTRING((2 0,0 2)(0 3,3 0))",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(1.5 0.5,1 0)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies);

    tester::apply("MULTILINESTRING((2 0,0 2)(0 3,3 0))",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(1.5 0.5,1 0)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies, true, true);

    //geometries intersect
    //intersect boundary of ring
    tester::apply("MULTILINESTRING((1 1,0.1 0.1,0.2 0.1)(1 1,2 2,3 3))",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                  strategies);

    //intersect interior of ring
    tester::apply("MULTILINESTRING((0.2 0.2,0.1 0.1,0.2 0.1)(1 1,2 2,3 3))",
                  "POLYGON((0 0,1 0,0 1,0 0))",
                  "SEGMENT(0.2 0.2,0.2 0.2)",
                  strategies);

    typedef test_geometry<MultiLinestring, Polygon, Segment> tester2;

    tester2::apply("MULTILINESTRING((2 0,0 2)(0 3,3 0))",
                   "POLYGON((0 0,1 0,0 1,0 0))",
                   "SEGMENT(1.5 0.5,1 0)",
                   "SEGMENT(0.50019 1.50021,0 1)",
                   "SEGMENT(1.496909 0.503379,1 0)",
                   strategies);

    tester2::apply("MULTILINESTRING((2 0,0 2)(0 3,3 0))",
                   "POLYGON((0 0,1 0,0 1,0 0)(0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4))",
                   "SEGMENT(1.5 0.5,1 0)",
                   "SEGMENT(0.50019 1.50021,0 1)",
                   "SEGMENT(1.496909 0.503379,1 0)",
                   strategies);

    //geometries intersect
    //intersect boundary of ring
    tester2::apply("MULTILINESTRING((1 1,2 2,3 4)(1 1,0.1 0.1,0.1 0.2))",
                   "POLYGON((0 0,1 0,0 1,0 0))",
                   "SEGMENT(0.5 0.5,0.5 0.5)",
                   "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                   strategies);

    //intersect interior of ring
    tester2::apply("MULTILINESTRING((1 1,2 2,3 4)(0.2 0.2,0.1 0.1,0.1 0.2))",
                   "POLYGON((0 0,1 0,0 1,0 0))",
                   "SEGMENT(0.2 0.2,0.2 0.2)",
                   strategies);

    //intersect boundary of interior ring
    tester2::apply("MULTILINESTRING((2 2,3 4,4 4)(0.9 0.9,0.1 0.1,0.1 0.2))",
                   "POLYGON((-1 -1,2 0,0 2,-1 -1)(0 0,0 1,1 0,0 0))",
                   "SEGMENT(0.5 0.5,0.5 0.5)",
                   "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                   strategies);

    //intersect boundary of exterior ring
    tester2::apply("MULTILINESTRING((2 2,3 4,4 4)(0.9 0.9,2 2,1 2))",
                   "POLYGON((-1 -1,2 0,0 2,-1 -1)(0 0,0 1,1 0,0 0))",
                   "SEGMENT(1 1,1 1)",
                   "SEGMENT(1.0002 1.00026,1.0002 1.00026)",
                   strategies);

    //intersect interior of polygon
    tester2::apply("MULTILINESTRING((2 2,3 4,4 4)(0 -0.5,1 -0.5,1.1 -0.5))",
                   "POLYGON((-1 -1,0 2,2 0,-1 -1)(0 0,1 0,0 1,0 0))",
                   "SEGMENT(0.5 -0.5,0.5 -0.5)",
                   "SEGMENT(0.500575 -0.500019,0.500575 -0.500019)",
                   strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_multi_linestring_multi_polygon(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "multi-linestring/polygon or ring closest_points tests"
              << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::multi_linestring<Linestring> MultiLinestring;
    typedef bg::model::polygon<Point> Polygon;
    typedef bg::model::multi_polygon<Polygon> MultiPolygon;

    typedef test_geometry<MultiLinestring, MultiPolygon, Segment> tester;

    tester::apply("MULTILINESTRING((2 0,0 2)(0 3,3 0))",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                  ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(1.5 0.5,1 0)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies);

    tester::apply("MULTILINESTRING((2 0,0 2)(0 3,3 0))",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),\
                  ((0.4 0.4,0.4 0.1,0.1 0.4,0.4 0.4)))",
                  "SEGMENT(1.5 0.5,1 0)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies, true, true);

    //geometries intersect
    //intersect boundary of ring
    tester::apply("MULTILINESTRING((1 1,2 2,3 4)(1 1,0.1 0.1,0.1 0.2))",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),((0 0,-1 0,0 -1,0 0)))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                  strategies);

    //intersect interior of ring
    tester::apply("MULTILINESTRING((1 1,2 2,3 4)(0.2 0.2,0.1 0.1,0.1 0.2))",
                  "MULTIPOLYGON(((0 0,1 0,0 1,0 0)),((0 0,-1 0,0 -1,0 0)))",
                  "SEGMENT(0.2 0.2,0.2 0.2)",
                  strategies);

    //intersect boundary of interior ring
    tester::apply("MULTILINESTRING((2 2,3 4,4 4)(0.9 0.9,0.1 0.1,0.1 0.2))",
                  "MULTIPOLYGON(((-1 -1,2 0,0 2,-1 -1)(0 0,0 1,1 0,0 0)),\
                                ((-1 -1,-2 -1,-1 -2,-1 -1)))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.500004 0.500053,0.500004 0.500053)",
                  strategies);

    //intersect boundary of exterior ring
    tester::apply("MULTILINESTRING((2 2,3 4,4 4)(0.9 0.9,2 2,1 2))",
                  "MULTIPOLYGON(((-1 -1,2 0,0 2,-1 -1)(0 0,0 1,1 0,0 0)),\
                                ((-1 -1,-2 -1,-1 -2,-1 -1)))",
                  "SEGMENT(1 1,1 1)",
                  "SEGMENT(1.0002 1.00026,1.0002 1.00026)",
                  strategies);

    //intersect interior of polygon
    tester::apply("MULTILINESTRING((2 2,3 4,4 4)(0 -0.5,1 -0.5,1.1 -0.5))",
                  "MULTIPOLYGON(((-1 -1,0 2,2 0,-1 -1)(0 0,1 0,0 1,0 0)),\
                                ((-1 -1,-2 -1,-1 -2,-1 -1)))",
                  "SEGMENT(0.5 -0.5,0.5 -0.5)",
                  "SEGMENT(0.500575 -0.500019,0.500575 -0.500019)",
                  strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_multi_linestring_box(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "multi-linestring/polygon or ring closest_points tests"
              << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::multi_linestring<Linestring> MultiLinestring;
    typedef bg::model::box<Point> Box;

    typedef test_geometry<MultiLinestring, Box, Segment> tester;

    tester::apply("MULTILINESTRING((2 0,0 2,0 0,-1 -1)(-1 0,-1 2,0 5))",
                  "BOX(10 10,20 20)",
                  "SEGMENT(0 5,10 10)",
                  strategies);

    tester::apply("MULTILINESTRING((2 0,0 2,0 0,-1 -1)(-1 0,-1 2,0 5))",
                  "BOX(10 10,20 20)",
                  "SEGMENT(0 5,10 10)",
                  strategies, true, true);

    //intersection
    tester::apply("MULTILINESTRING((2 0,0 2,0 0,-1 -1)(-1 0,-1 2,0 5))",
                  "BOX(10 10,20 20)",
                  "SEGMENT(0 5,10 10)",
                  strategies);
    tester::apply("MULTILINESTRING((15 15,15 25,20 25)(5 5,5 10))",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15 15,15 15)",
                  strategies);
    tester::apply("MULTILINESTRING((15 21,21 15,22 25)(5 5,5 10))",
                  "BOX(10 10,20 20)",
                  "SEGMENT(20 16,20 16)",
                  "SEGMENT(20 16.0403,20 16.0403)",
                  "SEGMENT(20 16.0405,20 16.0405)",
                  strategies);
    tester::apply("MULTILINESTRING((15 21,16 5,25 5)(5 5,5 10))",
                  "BOX(10 10,20 20)",
                  "SEGMENT(15.6875 10,15.6875 10)",
                  "SEGMENT(15.7003 10,15.7003 10)",
                  strategies);
    tester::apply("MULTILINESTRING((0 0,100 3,100 4)(0 0,10 0))",
                  "BOX(0 1,100 2)",
                  "SEGMENT(33.3333 1,33.3333 1)",
                  "SEGMENT(19.1476 1,19.1476 1)",
                  "SEGMENT(19.0629 1,19.0629 1)",
                  strategies);

}

//===========================================================================
//===========================================================================
//===========================================================================

template <typename Point, typename Strategies>
void test_all_l_ar(Strategies strategies)
{
    test_closest_points_segment_polygon_or_ring<Point>(strategies);
    test_closest_points_segment_multi_polygon<Point>(strategies);
    //test_closest_points_segment_box<Point>(strategies);

    test_closest_points_linestring_polygon_or_ring<Point>(strategies);
    test_closest_points_linestring_multi_polygon<Point>(strategies);
    //test_closest_points_linestring_box<Point>(strategies);

    test_closest_points_multi_linestring_polygon_or_ring<Point>(strategies);
    test_closest_points_multi_linestring_multi_polygon<Point>(strategies);
    //test_closest_points_multi_linestring_box<Point>(strategies);

    test_more_empty_input_pointlike_areal<Point>(strategies);
}

BOOST_AUTO_TEST_CASE( test_all_linear_areal )
{
    test_all_l_ar<car_point>(cartesian());

    test_all_l_ar<sph_point>(spherical());
    test_all_l_ar<sph_point>(spherical(
        bg::formula::mean_radius<double>(bg::srs::spheroid<double>())));
    
    test_all_l_ar<geo_point>(andoyer());
    test_all_l_ar<geo_point>(thomas());
    test_all_l_ar<geo_point>(vincenty());
}
