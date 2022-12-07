// Boost.Geometry

// Copyright (c) 2022 Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "test_crosses.hpp"

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
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(LINESTRING(0 0, 10 10))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(LINESTRING(0 0, 10 0))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)))",
                              false);

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(LINESTRING(0 0, 10 0), POINT(1 1))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(LINESTRING(0 0, 10 0))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), POINT(9 0))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(LINESTRING(0 0, 10 0))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), POINT(10 0))",
                              false);

    test_geometry<ls_t, gc_t>("LINESTRING(0 0, 10 10)",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)))",
                              true);
    test_geometry<gc_t, po_t>("GEOMETRYCOLLECTION(LINESTRING(0 0, 10 0))",
                              "POLYGON((0 0,0 5,5 5,5 0,0 0))",
                              false);

    test_geometry<ls_t, gc_t>("LINESTRING(0 0,2 2,4 4)",
                              "GEOMETRYCOLLECTION(LINESTRING(0 1,2 1,3 1))",
                              true);
    test_geometry<gc_t, ls_t>("GEOMETRYCOLLECTION(LINESTRING(0 0,2 2,4 4))",
                              "LINESTRING(0 1,1 1,2 2,3 2)",
                              false);
}

int test_main(int , char* [])
{
    test_gc();

    return 0;
}
