// Boost.Geometry
// Unit Test

// Copyright (c) 2022 Oracle and/or its affiliates.
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <geometry_test_common.hpp>

#include <boost/variant.hpp>

#include <boost/geometry/views/detail/random_access_view.hpp>
#include <boost/geometry/geometries/geometries.hpp>

using point_t = bg::model::point<double, 2, bg::cs::cartesian>;
using linestring_t = bg::model::linestring<point_t>;
using polygon_t = bg::model::polygon<point_t>;
using variant_t = boost::variant<point_t, linestring_t, polygon_t>;
using gc_t = bg::model::geometry_collection<variant_t>;
using nra_gc_t = bg::model::geometry_collection<variant_t, std::list>;

struct rec_gc_t;
using rec_var_t = boost::variant<point_t, linestring_t, polygon_t, rec_gc_t>;
struct rec_gc_t : std::vector<rec_var_t>
{
    rec_gc_t() = default;
    rec_gc_t(std::initializer_list<rec_var_t> l) : std::vector<rec_var_t>(l) {}
};

struct rec_nra_gc_t;
using rec_nra_var_t = boost::variant<point_t, linestring_t, polygon_t, rec_nra_gc_t>;
struct rec_nra_gc_t : std::list<rec_nra_var_t>
{
    rec_nra_gc_t() = default;
    rec_nra_gc_t(std::initializer_list<rec_nra_var_t> l) : std::list<rec_nra_var_t>(l) {}
};

namespace boost { namespace geometry { namespace traits {
    template<> struct tag<rec_gc_t> { typedef geometry_collection_tag type; };
    template<> struct geometry_types<rec_gc_t> { typedef util::type_sequence<point_t, linestring_t, rec_gc_t, polygon_t> type; };
}}}

namespace boost { namespace geometry { namespace traits {
    template<> struct tag<rec_nra_gc_t> { typedef geometry_collection_tag type; };
    template<> struct geometry_types<rec_nra_gc_t> { typedef util::type_sequence<point_t, linestring_t, rec_nra_gc_t, polygon_t> type; };
}}}

template <typename GC>
GC make_gc()
{
    return GC{
        point_t{0, 0},
        linestring_t{{1, 1}, {2, 2}},
        polygon_t{{{3, 3}, {3, 4}, {4, 4}, {4, 3}, {3, 3}}},
        polygon_t{{{5, 5}, {5, 6}, {6, 6}, {6, 5}, {5, 5}}}
    };
}

template <typename GC>
GC make_rec_gc()
{
    return GC{
        point_t{0, 0},
        GC {
            // This should be put at the end because GC is processed in a BFS manner.
            // So both GCs should be equivalent.
            polygon_t{{{5, 5}, {5, 6}, {6, 6}, {6, 5}, {5, 5}}}
        },
        linestring_t{{1, 1}, {2, 2}},
        polygon_t{{{3, 3}, {3, 4}, {4, 4}, {4, 3}, {3, 3}}},
    };
}

template <typename GC, typename Make>
void test_all(Make make)
{
    using view_t = bg::detail::random_access_view<GC>;

    GC gc = make();
    view_t rav{gc};

    using non_gc_types = bg::util::type_sequence<point_t, linestring_t, polygon_t>;
    using view_types = typename bg::traits::geometry_types<view_t>::type;
    BOOST_STATIC_ASSERT((std::is_same<non_gc_types, view_types>::value));
    BOOST_STATIC_ASSERT(bg::detail::is_random_access_range<view_t>::value);
    BOOST_STATIC_ASSERT(! bg::detail::is_geometry_collection_recursive<view_t>::value);
    
    size_t s = boost::size(gc);
    for (size_t i = 0 ; i < s ; ++i)
    {
        bg::traits::iter_visit<bg::detail::random_access_view<GC>>::apply([&](auto&& g)
        {
            using geom_t = bg::util::remove_cref_t<decltype(g)>;
            if (i == 0) { BOOST_CHECK(bg::util::is_point<geom_t>::value); }
            else if (i == 1) { BOOST_CHECK(bg::util::is_linestring<geom_t>::value); }
            else { BOOST_CHECK(bg::util::is_polygon<geom_t>::value); }
        }, rav.begin() + i);
    }
}

int test_main(int, char* [])
{
    BOOST_STATIC_ASSERT(bg::detail::is_random_access_range<gc_t>::value);
    BOOST_STATIC_ASSERT(! bg::detail::is_random_access_range<nra_gc_t>::value);
    BOOST_STATIC_ASSERT(bg::detail::is_random_access_range<rec_gc_t>::value);
    BOOST_STATIC_ASSERT(! bg::detail::is_random_access_range<rec_nra_gc_t>::value);

    BOOST_STATIC_ASSERT(! bg::detail::is_geometry_collection_recursive<gc_t>::value);
    BOOST_STATIC_ASSERT(! bg::detail::is_geometry_collection_recursive<nra_gc_t>::value);
    BOOST_STATIC_ASSERT(bg::detail::is_geometry_collection_recursive<rec_gc_t>::value);
    BOOST_STATIC_ASSERT(bg::detail::is_geometry_collection_recursive<rec_nra_gc_t>::value);

    test_all<gc_t>(make_gc<gc_t>);
    test_all<nra_gc_t>(make_gc<nra_gc_t>);
    test_all<rec_gc_t>(make_gc<rec_gc_t>);
    test_all<rec_nra_gc_t>(make_gc<rec_nra_gc_t>);

    return 0;
}
