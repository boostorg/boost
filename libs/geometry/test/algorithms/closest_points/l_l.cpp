// Boost.Geometry
// Unit Test

// Copyright (c) 2021, Oracle and/or its affiliates.

// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html

#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE test_closest_points_linear_linear
#endif

#include "common.hpp"
#include "empty_geometry.hpp"

namespace bg = boost::geometry;

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_segment_segment(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "point/segment closest_points tests" << std::endl;
#endif

    typedef bg::model::segment<Point> Segment;
    typedef test_geometry<Segment, Segment, Segment> tester;

    tester::apply("SEGMENT(0 2,2 0)",
                  "SEGMENT(0 1,1 0)",
                  "SEGMENT(0.5 1.5,0 1)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies);

    //default strategies
    tester::apply("SEGMENT(0 2,2 0)",
                  "SEGMENT(0 1,1 0)",
                  "SEGMENT(0.5 1.5,0 1)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies, true, true);

    //geometries intersect
    tester::apply("SEGMENT(0 0,1 1)",
                  "SEGMENT(0 1,1 0)",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.5 0.500057,0.5 0.500057)",
                  strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_segment_linestring(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "segment/linestring closest_points tests" << std::endl;
#endif

    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::segment<Point> Segment;
    typedef test_geometry<Segment, Linestring, Segment> tester;

    tester::apply("SEGMENT(0 2,2 0)",
                  "LINESTRING(-1 0,0 1,1 0,-1 -2)",
                  "SEGMENT(0.5 1.5,0 1)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies);

    //default strategies
    tester::apply("SEGMENT(0 2,2 0)",
                  "LINESTRING(-1 0,0 1,1 0,-1 -2)",
                  "SEGMENT(0.5 1.5,0 1)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies, true, true);

    //geometries intersect
    tester::apply("SEGMENT(0 0,1 1)",
                  "LINESTRING(0 2,0 1,1 0,2 0)",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.5 0.500057,0.5 0.500057)",
                  strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_linestring_linestring(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "linestring/linestring closest_points tests" << std::endl;
#endif

    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::segment<Point> Segment;
    typedef test_geometry<Linestring, Linestring, Segment> tester;

    tester::apply("LINESTRING(0 2,2 0,3 0,4 0,5 0)",
                  "LINESTRING(-1 0,0 1,1 0,-1 -2)",
                  "SEGMENT(0.5 1.5,0 1)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies);

    //default strategies
    tester::apply("LINESTRING(0 2,2 0,3 0,4 0,5 0)",
                  "LINESTRING(-1 0,0 1,1 0,-1 -2)",
                  "SEGMENT(0.5 1.5,0 1)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies, true, true);

    //geometries intersect
    tester::apply("LINESTRING(0 0,1 1,2 2)",
                  "LINESTRING(0 2,0 1,1 0,2 0)",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.5 0.500057,0.5 0.500057)",
                  strategies);

    tester::apply("LINESTRING(0 0,1 1)",
                  "LINESTRING(0 2,0 1,1 0,2 0)",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.5 0.500057,0.5 0.500057)",
                   strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_segment_multi_linestring(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "segment/multi-linestring closest_points tests" << std::endl;
#endif

    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::multi_linestring<Linestring> MultiLinestring;
    typedef bg::model::segment<Point> Segment;
    typedef test_geometry<Segment, MultiLinestring, Segment> tester;

    tester::apply("SEGMENT(0 2,2 0)",
                  "MULTILINESTRING((-1 0,0 1,1 0,-1 -2)(0 4,4 0,5 0))",
                  "SEGMENT(0.5 1.5,0 1)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies);

    tester::apply("SEGMENT(0 2,2 0)",
                  "MULTILINESTRING((-1 0,0 1,1 0,-1 -2)(0 4,4 0,5 0))",
                  "SEGMENT(0.5 1.5,0 1)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies, true, true);

    //geometries intersect
    tester::apply("SEGMENT(0 0,1 1)",
                 "MULTILINESTRING((0 2,0 1,1 0,2 0)(1 0,3 0))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.5 0.500057,0.5 0.500057)",
                  strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_linestring_multi_linestring(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "linestring /multi-linestring closest_points tests" << std::endl;
#endif

    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::multi_linestring<Linestring> MultiLinestring;
    typedef bg::model::segment<Point> Segment;
    typedef test_geometry<Linestring, MultiLinestring, Segment> tester;

    tester::apply("LINESTRING(0 2,2 0,2 -1)",
                  "MULTILINESTRING((-1 0,0 1,1 0,-1 -2)(0 4,4 0,5 0))",
                  "SEGMENT(0.5 1.5,0 1)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies);

    tester::apply("LINESTRING(0 2,2 0,2 -1)",
                  "MULTILINESTRING((-1 0,0 1,1 0,-1 -2)(0 4,4 0,5 0))",
                  "SEGMENT(0.5 1.5,0 1)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies, true, true);

    //geometries intersect
    tester::apply("LINESTRING(0 0,1 1)",
                  "MULTILINESTRING((0 2,0 1,1 0,2 0)(1 0,3 0))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.5 0.500057,0.5 0.500057)",
                  strategies);

    tester::apply("LINESTRING(0 0,1 1,2 2)",
                  "MULTILINESTRING((0 2,0 1,1 0,2 0)(1 0,3 0))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.5 0.500057,0.5 0.500057)",
                  strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_multi_linestring_multi_linestring(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "multi-linestring /multi-linestring closest_points tests" << std::endl;
#endif

    typedef bg::model::linestring<Point> Linestring;
    typedef bg::model::multi_linestring<Linestring> MultiLinestring;
    typedef bg::model::segment<Point> Segment;
    typedef test_geometry<MultiLinestring, MultiLinestring, Segment> tester;

    tester::apply("MULTILINESTRING((0 2,2 0,2 -1)(0 2,-1 2))",
                  "MULTILINESTRING((-1 0,0 1,1 0,-1 -2)(0 4,4 0,5 0))",
                  "SEGMENT(0.5 1.5,0 1)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies);

    tester::apply("MULTILINESTRING((0 2,2 0,2 -1)(0 2,-1 2))",
                  "MULTILINESTRING((-1 0,0 1,1 0,-1 -2)(0 4,4 0,5 0))",
                  "SEGMENT(0.5 1.5,0 1)",
                  "SEGMENT(0.50019 1.50021,0 1)",
                  "SEGMENT(1.496909 0.503379,1 0)",
                  strategies, true, true);

    //geometries intersect
    tester::apply("MULTILINESTRING((0 0,1 1,2 2)(1 2,1 3))",
                  "MULTILINESTRING((0 2,0 1,1 0,2 0)(1 0,3 0))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.5 0.500057,0.5 0.500057)",
                  strategies);

    //multiple intersections
    tester::apply("MULTILINESTRING((0 0,1 1,2 2)(2 0,3 0))",
                  "MULTILINESTRING((0 2,0 1,1 0,2 0)(1 0,3 0))",
                  "SEGMENT(0.5 0.5,0.5 0.5)",
                  "SEGMENT(0.5 0.500057,0.5 0.500057)",
                  strategies);
}


//===========================================================================
//===========================================================================
//===========================================================================

template <typename Point, typename Strategies>
void test_all_l_l(Strategies strategies)
{
    test_closest_points_segment_segment<Point>(strategies);
    test_closest_points_segment_linestring<Point>(strategies);
    test_closest_points_linestring_linestring<Point>(strategies);

    test_closest_points_segment_multi_linestring<Point>(strategies);
    test_closest_points_linestring_multi_linestring<Point>(strategies);
    test_closest_points_multi_linestring_multi_linestring<Point>(strategies);

    test_more_empty_input_linear_linear<Point>(strategies);
}

BOOST_AUTO_TEST_CASE( test_all_linear_linear )
{
    test_all_l_l<car_point>(cartesian());

    test_all_l_l<sph_point>(spherical());
    test_all_l_l<sph_point>(spherical(
        bg::formula::mean_radius<double>(bg::srs::spheroid<double>())));

    test_all_l_l<geo_point>(andoyer());
    test_all_l_l<geo_point>(thomas());
    test_all_l_l<geo_point>(vincenty());
}
