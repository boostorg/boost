// Boost.Geometry

// Copyright (c) 2022 Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "geometry_test_common.hpp"

#include <boost/geometry/algorithms/buffer.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/io/wkt/read.hpp>
#include <boost/geometry/strategies/strategies.hpp>

using pt_t = bg::model::point<double, 2, bg::cs::cartesian>;
using ls_t = bg::model::linestring<pt_t>;
using po_t = bg::model::polygon<pt_t>;
using mpt_t = bg::model::multi_point<pt_t>;
using mls_t = bg::model::multi_linestring<ls_t>;
using mpo_t = bg::model::multi_polygon<po_t>;
using var_t = boost::variant<pt_t, ls_t, po_t, mpt_t, mls_t, mpo_t>;
using gc_t = bg::model::geometry_collection<var_t>;

void test_gc()
{
    gc_t gc;
    bg::read_wkt("GEOMETRYCOLLECTION("
                 "POLYGON((0 0,0 5,5 5,5 0,0 0)),"
                 "LINESTRING(2 2,9 9),"
                 "POINT(0 9)"
                 ")", gc);

    bg::strategy::buffer::distance_symmetric<double> distance(1.0);
    bg::strategy::buffer::side_straight side;
    bg::strategy::buffer::join_round join;
    bg::strategy::buffer::end_round end;
    bg::strategy::buffer::point_circle circle;
    
    mpo_t result_mpo;
    bg::buffer(gc, result_mpo, distance, side, join, end, circle);

    BOOST_CHECK(result_mpo.size() == 2);
    double area = bg::area(result_mpo);
    BOOST_CHECK_CLOSE(area, 62.548206613048151, 0.0001);

    gc_t result_gc;
    bg::buffer(gc, result_gc, distance, side, join, end, circle);

    BOOST_CHECK(result_gc.size() == 1);
    BOOST_CHECK(boost::get<mpo_t>(result_gc[0]).size() == 2);
    area = bg::area(boost::get<mpo_t>(result_gc[0]));
    BOOST_CHECK_CLOSE(area, 62.548206613048151, 0.0001);
}

int test_main(int, char* [])
{
    test_gc();

    return 0;
}
