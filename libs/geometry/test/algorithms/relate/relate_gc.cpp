// Boost.Geometry

// Copyright (c) 2022 Oracle and/or its affiliates.

// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include "test_relate.hpp"

#include <boost/geometry/geometries/adapted/boost_variant2.hpp>
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

void test_gc_gc()
{
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 10,10 10,10 0,0 0)), LINESTRING(5 10, 10 11, 15 10))",
                              "GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(15 5))",
                              "FF2F11212");
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 10,10 10,10 0,0 0)), LINESTRING(5 10, 10 11, 15 10))",
                              "GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(5 0))",
                              "FF2011212");
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 10,9 10,9 0,0 0)), LINESTRING(5 10, 10 11, 15 10))",
                              "GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(15 5))",
                              "FF2F01212");
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 10,9 10,9 0,0 0)), LINESTRING(5 10, 10 11, 15 10))",
                              "GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(5 0))",
                              "FF2001212");
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 10,9 10,9 0,0 0)), LINESTRING(5 10, 10 11, 15 10, 15 5))",
                              "GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(15 5))",
                              "1020F1212");

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 10,0 20,10 20,10 10,0 10)), POLYGON((10 0,10 10,20 10,20 0,10 0)),"
                                                " POLYGON((20 10,20 20,30 20,30 10,20 10)), POLYGON((10 20,10 30,20 30,20 20,10 20)))",
                              "GEOMETRYCOLLECTION(POLYGON((10 10,10 20,20 20,20 10,10 10)), LINESTRING(11 11,19 19))",
                              "FF2F112F2");

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POINT(1 1))",
                              "GEOMETRYCOLLECTION(LINESTRING(0 0, 1 1), LINESTRING(1 1, 2 2))",
                              "0FFFFF102");

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 6 6))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(5 5, 6 6))",
                              "2FFF1FFF2");

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(LINESTRING(4 4, 6 6))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 7 7))",
                              "10F0FF212");

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 4, 4 1))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 6 6))",
                              "2FFF1F102");

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(LINESTRING(0 0,5 5))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 6 6))",
                              "1FFF0F212");

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(LINESTRING(0 0,5 5), POINT(6 6))",
                              "GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), POINT(6 6))",
                              "1FFF0F212");

    // Error in detail::relate::linear_linear
#ifdef BOOST_GEOMETRY_TEST_ENABLE_FAILING
    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(LINESTRING(0 0,5 5))",
                              "GEOMETRYCOLLECTION(LINESTRING(0 0,5 5), LINESTRING(5 5, 6 6))",
                              "1FF00F102");
#endif

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(LINESTRING(0 0,5 5))",
                              "GEOMETRYCOLLECTION(LINESTRING(0 0,6 6))",
                              "1FF00F102");

    test_geometry<gc_t, gc_t>("GEOMETRYCOLLECTION(POLYGON((0 0,0 10,9 10,9 0,0 0)), LINESTRING(5 10, 10 11, 15 10))",
                              "GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(15 5))",
                              "FF2F01212");

    {
        // Test case causing II, IE and EI not to be calculated with static_mask without aa_handler_wrapper
        gc_t gc1, gc2;
        bg::read_wkt("GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 4, 4 1))", gc1);
        bg::read_wkt("GEOMETRYCOLLECTION(POLYGON((0 0,0 5,5 5,5 0,0 0)), LINESTRING(1 1, 6 6))", gc2);
        bool b1 = bg::relate(gc1, gc2, bg::de9im::mask("*F*F1F*02"));
        bool b2 = bg::relate(gc1, gc2, bg::de9im::static_mask<'*','F','*','F','1','F','*','0','2'>());
        BOOST_CHECK(b1);
        BOOST_CHECK(b2);
    }
}

void test_g_gc()
{
    test_geometry<po_t, gc_t>("POLYGON((0 0,0 10,10 10,10 0,0 0))",
                              "GEOMETRYCOLLECTION(POLYGON((10 0,10 10,20 10,20 0,10 0)), POINT(15 5))",
                              "FF2F11212");
}

int test_main( int , char* [] )
{
    test_gc_gc();
    test_g_gc();
    
    return 0;
}
