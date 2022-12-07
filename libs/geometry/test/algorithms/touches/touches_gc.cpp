// Boost.Geometry

// Copyright (c) 2022 Adam Wulkiewicz, Lodz, Poland.

// Copyright (c) 2022 Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "test_touches.hpp"

using pt_t = bg::model::point<double, 2, bg::cs::cartesian>;
using ls_t = bg::model::linestring<pt_t>;
using po_t = bg::model::polygon<pt_t>;
using mpt_t = bg::model::multi_point<pt_t>;
using mls_t = bg::model::multi_linestring<ls_t>;
using mpo_t = bg::model::multi_polygon<po_t>;
using var_t = boost::variant<pt_t, ls_t, po_t, mpt_t, mls_t, mpo_t>;
//using var_t = boost::variant2::variant<pt_t, ls_t, po_t, mpt_t, mls_t, mpo_t>;
using gc_t = bg::model::geometry_collection<var_t>;

void test_gc()
{
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 10,10 10,10 0,0 0)), LINESTRING(5 10, 10 11, 15 10))",
                              "GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(15 5))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 10,10 10,10 0,0 0)), LINESTRING(5 10, 10 11, 15 10))",
                              "GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(5 0))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 10,9 10,9 0,0 0)), LINESTRING(5 10, 10 11, 15 10))",
                              "GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(15 5))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 10,9 10,9 0,0 0)), LINESTRING(5 10, 10 11, 15 10))",
                              "GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(5 0))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 10,9 10,9 0,0 0)), LINESTRING(5 10, 10 11, 15 10, 15 5))",
                              "GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(15 5))",
                              false);

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 10,0 20,10 20,10 10,0 10)), POLYGON((10 0,10 10,20 10,20 0,10 0)),"
                                                " POLYGON((20 10,20 20,30 20,30 10,20 10)), POLYGON((10 20,10 30,20 30,20 20,10 20)))",
                              "GEOMETRYCOLLECTION(POLYGON((10 10,10 20,20 20,20 10,10 10)), LINESTRING(11 11,19 19))",
                              true);

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POINT(0 0), POINT(2 2))",
                              "GEOMETRYCOLLECTION(POINT(0 0), POINT(1 1))",
                              false);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POINT(0 0))",
                              "GEOMETRYCOLLECTION(POINT(0 0))",
                              false);

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POINT(2 2))",
                              "GEOMETRYCOLLECTION(LINESTRING(0 0, 1 1), LINESTRING(1 1, 2 2))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POINT(1 1))",
                              "GEOMETRYCOLLECTION(LINESTRING(0 0, 1 1), LINESTRING(1 1, 2 2))",
                              false);

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 6 6))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(5 5, 6 6))",
                              false);

    test_geometry<po_t, gc_t>("POLYGON((0 0,0 10,10 10,10 0,0 0))",
                              "GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(15 5))",
                              true);

    test_geometry<gc_t, po_t>("GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(15 5))",
                              "POLYGON((0 0,0 10,10 10,10 0,0 0))",
                              true);
}

int test_main(int , char* [])
{
    test_gc();

    return 0;
}
