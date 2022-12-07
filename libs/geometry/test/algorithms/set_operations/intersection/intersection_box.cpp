// Boost.Geometry

// Copyright (c) 2007-2015 Barend Gehrels, Amsterdam, the Netherlands.
// Copyright (c) 2008-2015 Bruno Lalande, Paris, France.
// Copyright (c) 2009-2015 Mateusz Loskot, London, UK.

// This file was modified by Oracle on 2015-2022.
// Modifications copyright (c) 2015-2022, Oracle and/or its affiliates.
// Contributed and/or modified by Menelaos Karavelas, on behalf of Oracle
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Parts of Boost.Geometry are redesigned from Geodan's Geographic Library
// (geolib/GGL), copyright (c) 1995-2010 Geodan, Amsterdam, the Netherlands.

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html


#include <string>

#include "test_intersection.hpp"


template <typename Box>
void test_boxes(std::string const& wkt1, std::string const& wkt2, double expected_area, bool expected_result)
{
    Box box1, box2;
    bg::read_wkt(wkt1, box1);
    bg::read_wkt(wkt2, box2);

    Box box_out;
    bg::assign_zero(box_out);
    bool detected = bg::intersection(box1, box2, box_out);
    typename bg::default_area_result<Box>::type area = bg::area(box_out);

    BOOST_CHECK_EQUAL(detected, expected_result);
    if (detected && expected_result)
    {
        BOOST_CHECK_CLOSE(area, expected_area, 0.01);
    }
}

template <typename P>
void test_boxes_per_d(P const& min1, P const& max1, P const& min2, P const& max2, bool expected_result)
{
    using box = bg::model::box<P>;
    
    box box_out;
    bool detected = bg::intersection(box(min1, max1), box(min2, max2), box_out);

    BOOST_CHECK_EQUAL(detected, expected_result);
    if (detected && expected_result)
    {
        BOOST_CHECK(bg::equals(box_out, box(min2,max1)));
    }
}

template <typename CoordinateType>
void test_boxes_nd()
{
    using p1 = bg::model::point<CoordinateType, 1, bg::cs::cartesian>;
    using p2 = bg::model::point<CoordinateType, 2, bg::cs::cartesian>;
    using p3 = bg::model::point<CoordinateType, 3, bg::cs::cartesian>;

    test_boxes_per_d(p1(0), p1(5), p1(3), p1(6), true);
    test_boxes_per_d(p2(0,0), p2(5,5), p2(3,3), p2(6,6), true);
    test_boxes_per_d(p3(0,0,0), p3(5,5,5), p3(3,3,3), p3(6,6,6), true);
}

template <typename CoordinateType>
void test_box_geo(std::string const& wkt1, std::string const& wkt2, std::string const& wkt_result)
{
    using point_deg = bg::model::point<CoordinateType, 2, bg::cs::geographic<bg::degree> >;
    using point_rad = bg::model::point<CoordinateType, 2, bg::cs::geographic<bg::radian> >;
    using box_deg = bg::model::box<point_deg>;
    using box_rad = bg::model::box<point_rad>;

    box_deg box1, box2;
    bg::read_wkt(wkt1, box1);
    bg::read_wkt(wkt2, box2);
    box_rad box3{{bg::get_as_radian<0, 0>(box1), bg::get_as_radian<0, 1>(box1)},
                 {bg::get_as_radian<1, 0>(box1), bg::get_as_radian<1, 1>(box1)}};
    box_rad box4{{bg::get_as_radian<0, 0>(box2), bg::get_as_radian<0, 1>(box2)},
                 {bg::get_as_radian<1, 0>(box2), bg::get_as_radian<1, 1>(box2)}};

    box_deg box5;
    box_rad box6;
    bool result1 = bg::intersection(box1, box2, box5);
    bool result2 = bg::intersection(box3, box4, box6);
    if (wkt_result.empty())
    {
        BOOST_CHECK(! result1 && ! result2);
    }
    else
    {
        box_deg box7;
        bg::read_wkt(wkt_result, box7);
        box_rad box8{{bg::get_as_radian<0, 0>(box7), bg::get_as_radian<0, 1>(box7)},
                     {bg::get_as_radian<1, 0>(box7), bg::get_as_radian<1, 1>(box7)}};
        BOOST_CHECK(result1 && result2);
        BOOST_CHECK(bg::equals(box5, box7));
        BOOST_CHECK(bg::equals(box6, box8));
    }
}

template <typename T>
void test_boxes_geo()
{
    test_box_geo<T>("box(0 0, 50 50)", "box(10 10, 40 40)", "box(10 10, 40 40)");
    test_box_geo<T>("box(0 0, 50 50)", "box(10 10, 60 60)", "box(10 10, 50 50)");

    test_box_geo<T>("box(100 -90, 270 90)", "box(10 10, 150 50)", "box(100 10, 150 50)");

    test_box_geo<T>("box(180.0 -90, 180.00000000000034 90.000000000000085)", "box(1 1, 4 4)", "");
}

template <typename T>
void test_boxes_car()
{
    using point = bg::model::d2::point_xy<T>;
    using box = bg::model::box<point>;

    test_boxes<box>("box(2 2,8 8)", "box(4 4,10 10)", 16, true);
    test_boxes<box>("box(2 2,8 7)", "box(4 4,10 10)", 12, true);
    test_boxes<box>("box(2 2,8 7)", "box(14 4,20 10)", 0, false);
    test_boxes<box>("box(2 2,4 4)", "box(4 4,8 8)", 0, true);
}

int test_main(int, char*[])
{
    test_boxes_car<default_test_type>();
    test_boxes_geo<double>();

#ifndef BOOST_GEOMETRY_TEST_ONLY_ONE_TYPE
    test_boxes_car<float>();
    test_boxes_nd<double>();
#endif

    return 0;
}
