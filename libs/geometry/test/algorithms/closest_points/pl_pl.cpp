// Boost.Geometry
// Unit Test

// Copyright (c) 2021, Oracle and/or its affiliates.

// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html

#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE test_closest_points_pointlike_pointlike
#endif

#include "common.hpp"
#include "empty_geometry.hpp"

namespace bg = boost::geometry;

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_point_point(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "point/point closest_points tests" << std::endl;
#endif

    using Segment = bg::model::segment<Point>;
    using tester = test_geometry<Point, Point, Segment>;

    tester::apply("POINT(1 1)",
                  "POINT(0 0)",
                  "SEGMENT(1 1,0 0)",
                  strategies);

    tester::apply("POINT(1 1)",
                  "POINT(1 1)",
                  "SEGMENT(1 1,1 1)",
                  strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_point_multi_point(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "point/multi-point closest_points tests" << std::endl;
#endif

    using Segment = bg::model::segment<Point>;
    using MultiPoint = bg::model::multi_point<Point>;
    using tester = test_geometry<Point, MultiPoint, Segment>;

    tester::apply("POINT(0 0)",
                  "MULTIPOINT((1 1),(2 0),(0 1),(2 1))",
                  "SEGMENT(0 0,0 1)",
                  strategies);

    tester::apply("POINT(2 0)",
                  "MULTIPOINT((1 1),(2 0),(0 1),(2 1))",
                  "SEGMENT(2 0,2 0)",
                  strategies);
}

//===========================================================================

template <typename Point, typename Strategies>
void test_closest_points_multi_point_multi_point(Strategies const& strategies)
{

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << std::endl;
    std::cout << "multi-point/multi-point closest_points tests" << std::endl;
#endif

    using Segment = bg::model::segment<Point>;
    using MultiPoint = bg::model::multi_point<Point>;
    using tester = test_geometry<MultiPoint, MultiPoint, Segment>;

    tester::apply("MULTIPOINT((-1 -1),(0 0))",
                  "MULTIPOINT((1 1),(2 0),(0 1),(2 1))",
                  "SEGMENT(0 0,0 1)",
                  strategies);

    tester::apply("MULTIPOINT((1 1),(0 0))",
                  "MULTIPOINT((1 1),(2 0),(0 1),(2 1))",
                  "SEGMENT(1 1,1 1)",
                  strategies);
}

template <typename Point, typename Strategies>
void test_variant(Strategies const& strategies)
{
    using point_type = Point;
    using segment_type = bg::model::segment<point_type>;
    using mp_type = bg::model::multi_point<point_type>;
    using variant_type = boost::variant<point_type, mp_type>;

    point_type point1;
    bg::read_wkt("POINT(1 3)", point1);

    point_type point2;
    bg::read_wkt("POINT(2 3)", point2);

    mp_type mpoint;
    bg::read_wkt("MULTIPOINT((1 1),(2 0),(0 1),(2 1))", mpoint);

    variant_type v1, v2;

    v1 = point1;
    v2 = point2;
    segment_type res;
    bg::read_wkt("SEGMENT(1 3,2 3)", res);
    compute_result(v1, v2, res, strategies, false);

    v1 = point1;
    v2 = mpoint;
    bg::read_wkt("SEGMENT(1 3,1 1)", res);
    compute_result(v1, v2, res, strategies, false);

}


//===========================================================================
//===========================================================================
//===========================================================================

template <typename Point, typename Strategies>
void test_all_pl_pl(Strategies strategies)
{
    test_closest_points_point_point<Point>(strategies);
    test_closest_points_point_multi_point<Point>(strategies);
    test_closest_points_multi_point_multi_point<Point>(strategies);

    //test_variant<Point>(strategies);

    test_more_empty_input_pointlike_pointlike<Point>(strategies);
}

BOOST_AUTO_TEST_CASE( test_all_pointlike_pointlike )
{
    test_all_pl_pl<car_point>(cartesian());

    test_all_pl_pl<sph_point>(spherical());
    test_all_pl_pl<sph_point>(spherical(
        bg::formula::mean_radius<double>(bg::srs::spheroid<double>())));

    test_all_pl_pl<geo_point>(andoyer());
    test_all_pl_pl<geo_point>(thomas());
    test_all_pl_pl<geo_point>(vincenty());
}
