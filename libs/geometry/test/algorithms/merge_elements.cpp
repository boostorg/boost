// Boost.Geometry
// Unit Test

// Copyright (c) 2022, Oracle and/or its affiliates.

// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html


#include <geometry_test_common.hpp>

#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/length.hpp>
#include <boost/geometry/algorithms/merge_elements.hpp>
#include <boost/geometry/algorithms/perimeter.hpp>
#include <boost/geometry/geometries/adapted/boost_variant2.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>


template <typename P>
void test_all(std::size_t points_count, std::size_t linestrings_count, std::size_t polygons_count,
              double length, double perimeter, double area)
{
    using pt_t = P;
    using mpt_t = bg::model::multi_point<pt_t>;
    //using seg_t = bg::model::segment<pt_t>;
    using ls_t = bg::model::linestring<pt_t>;
    using mls_t = bg::model::multi_linestring<ls_t>;
    //using box_t = bg::model::box<pt_t>;
    using ring_t = bg::model::ring<pt_t>;
    using poly_t = bg::model::polygon<pt_t>;
    using mpoly_t = bg::model::multi_polygon<poly_t>;
    using var_t = boost::variant<pt_t, mpt_t/*, seg_t*/, ls_t, mls_t, /*box_t,*/ ring_t, poly_t, mpoly_t>;
    //using var_t = boost::variant2::variant<pt_t, mpt_t/*, seg_t*/, ls_t, mls_t, /*box_t,*/ ring_t, poly_t, mpoly_t>;
    using gc_t = bg::model::geometry_collection<var_t>;

    gc_t gc{
        poly_t{{{0, 0},{0, 10},{10, 10},{10, 0},{0, 0}}, {{1, 1},{5, 1},{5, 5},{1, 5},{1, 1}}},
        mpoly_t{{{{4, 4},{4, 6},{6, 6},{6, 4},{4, 4}}}, {{{10, 10},{10, 12},{12, 12},{12, 10},{10, 10}}}},
        ring_t{{11, 11},{11, 14},{14, 14},{14, 11},{11, 11}},
        //box_t{{13, 13}, {16, 16}},
        mls_t{{{0, 0},{20, 20}}, {{3, 3},{3, 6},{6, 6},{6, 3},{3, 3}}},
        ls_t{{3, 3},{3, 20}},
        mpt_t{{2, 2},{2, 2},{5, 5},{9, 9},{11, 11},{0, 20},{1, 20}},
        pt_t{0, 20}
    };

    gc_t result;
    bg::merge_elements(gc, result);

    BOOST_CHECK(boost::get<mpt_t>(result[0]).size() == points_count);
    BOOST_CHECK(boost::get<mls_t>(result[1]).size() == linestrings_count);
    BOOST_CHECK(boost::get<mpoly_t>(result[2]).size() == polygons_count);
    auto l = bg::length(result);
    auto a = bg::area(result);
    auto p = bg::perimeter(result);
    decltype(l) l_expected = length;
    decltype(p) p_expected = perimeter;
    decltype(a) a_expected = area;
    BOOST_CHECK_CLOSE(l, l_expected, 0.000001);
    BOOST_CHECK_CLOSE(p, p_expected, 0.000001);
    BOOST_CHECK_CLOSE(a, a_expected, 0.000001);    
}

int test_main(int, char* [])
{
    test_all<bg::model::point<double, 2, bg::cs::cartesian>>(2, 5, 2, std::sqrt(2.0) * (3 + 6) + 4 + 10, 72, 97);
    // TODO: right now the elements of multi geometries are not merged,
    //       only different geometries stored in a GC. Hence duplicated point in the result.
    test_all<bg::model::point<double, 2, bg::cs::spherical_equatorial<bg::degree>>>(4, 6, 2, 0.48141804683843953, 1.2506937915396685, 0.029392562222852522);
    test_all<bg::model::point<double, 2, bg::cs::geographic<bg::degree>>>(4, 6, 2, 3058383.6297531724, 7951118.1434133006, 1187967114570.5911);
    
    return 0;
}
