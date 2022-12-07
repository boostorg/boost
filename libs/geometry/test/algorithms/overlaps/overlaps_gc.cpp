// Boost.Geometry

// Copyright (c) 2022 Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "test_overlaps.hpp"

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
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(MULTIPOINT(0 0,1 1,2 2))",
                              "GEOMETRYCOLLECTION(MULTIPOINT(1 1,3 3,4 4))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(MULTIPOINT(0 0,1 1,2 2))",
                              "GEOMETRYCOLLECTION(MULTIPOINT(1 1,2 2))",
                              false);

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POINT(0 0), POINT(1 1), POINT(2 2))",
                              "GEOMETRYCOLLECTION(POINT(1 1), POINT(3 3), POINT(4 4))",
                              true);
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POINT(0 0), POINT(1 1), POINT(2 2))",
                              "GEOMETRYCOLLECTION(POINT(1 1), POINT(2 2))",
                              false);

    test_geometry<mpt_t, gc_t>("MULTIPOINT(0 0,1 1,2 2)",
                               "GEOMETRYCOLLECTION(MULTIPOINT(1 1,3 3,4 4))",
                               true);
    test_geometry<gc_t, mpt_t>("GEOMETRYCOLLECTION(MULTIPOINT(0 0,1 1,2 2))",
                               "MULTIPOINT(1 1,2 2)",
                               false);

    test_geometry<gc_t, mpt_t>("GEOMETRYCOLLECTION(MULTIPOINT(0 0,1 1,2 2))",
                               "MULTIPOINT(1 1,3 3,4 4)",
                               true);
    test_geometry<mpt_t, gc_t>("MULTIPOINT(0 0,1 1,2 2)",
                               "GEOMETRYCOLLECTION(MULTIPOINT(1 1,2 2))",
                               false);
}

int test_main(int , char* [])
{
    test_gc();

    return 0;
}
