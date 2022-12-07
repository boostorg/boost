// Boost.Geometry

// Copyright (c) 2022 Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "test_within.hpp"

#include <boost/geometry/geometries/geometries.hpp>

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
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 4 4))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 4, 4 1))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 6 6))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 4, 4 1))",
                              false);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 4, 4 1))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 6 6))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 6 6))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(5 5, 6 6))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 6 6), POINT(0 6))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(5 5, 6 6))",
                              false);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(5 5, 6 6))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 6 6), POINT(0 6))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 6 6), POINT(0 6))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(5 5, 6 6), POINT(0 6))",
                              true);
    

    test_geometry<po_t, gc_t>("POLYGON((0 0,0 5,5 5,5 0,0 0))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 4 4))",
                              true);
    test_geometry<gc_t, po_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 6 6))",
                              "POLYGON((0 0,0 5,5 5,5 0,0 0))",
                              false);
}

int test_main(int , char* [])
{
    test_gc();

    return 0;
}
