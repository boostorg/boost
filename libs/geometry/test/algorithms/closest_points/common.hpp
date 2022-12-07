// Boost.Geometry
// Unit Test

// Copyright (c) 2021-2022, Oracle and/or its affiliates.

// Contributed and/or modified by Vissarion Fysikopoulos, on behalf of Oracle
// Contributed and/or modified by Adam Wulkiewicz, on behalf of Oracle

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html

#ifndef BOOST_GEOMETRY_TEST_ALGORITHMS_CLOSEST_POINTS_COMMON_HPP
#define BOOST_GEOMETRY_TEST_ALGORITHMS_CLOSEST_POINTS_COMMON_HPP

#include <boost/geometry/core/tags.hpp>

#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/algorithms/closest_points.hpp>
#include <boost/geometry/strategies/strategies.hpp>

#include <boost/test/included/unit_test.hpp>

#include <from_wkt.hpp>

namespace bg = boost::geometry;

//===========================================================================
// point types

using car_point = bg::model::point<double, 2, bg::cs::cartesian>;

using sph_point = bg::model::point
        <
            double, 2,
            bg::cs::spherical_equatorial<bg::degree>
        >;

using geo_point = bg::model::point
        <
            double, 2,
            bg::cs::geographic<bg::degree>
        >;

//===========================================================================

using cartesian = bg::strategies::closest_points::cartesian<double>;

using spherical = bg::strategies::closest_points::spherical<double>;

using andoyer = bg::strategies::closest_points::geographic<bg::strategy::andoyer>;
using thomas = bg::strategies::closest_points::geographic<bg::strategy::andoyer>;
using vincenty = bg::strategies::closest_points::geographic<bg::strategy::andoyer>;

//===========================================================================

template <typename Segment>
static inline Segment swap(Segment const& s)
{
    Segment swapped;

    bg::set<0, 0>(swapped, bg::get<1, 0>(s));
    bg::set<0, 1>(swapped, bg::get<1, 1>(s));
    bg::set<1, 0>(swapped, bg::get<0, 0>(s));
    bg::set<1, 1>(swapped, bg::get<0, 1>(s));

    return swapped;
}

template <int i, int j, typename Segment>
void compare_result_with_expected(Segment const& expected_resulting_segment,
                                  Segment const& resulting_segment)
{
    double expected = bg::get<i, j>(expected_resulting_segment);
    double resulting = bg::get<i, j>(resulting_segment);
    BOOST_CHECK_CLOSE(expected, resulting, 0.01);
}

template <typename Segment>
void compare_result_with_expected(Segment const& exp_resulting_segment,
                                  Segment const& resulting_segment)
{
    compare_result_with_expected<0,0>(exp_resulting_segment, resulting_segment);
    compare_result_with_expected<1,0>(exp_resulting_segment, resulting_segment);
    compare_result_with_expected<0,1>(exp_resulting_segment, resulting_segment);
    compare_result_with_expected<1,1>(exp_resulting_segment, resulting_segment);
}

//==============================================================================

template
<
    typename Geometry1,
    typename Geometry2,
    typename Segment,
    typename Strategy
>
void compute_result(Geometry1 const& geometry1,
                    Geometry2 const& geometry2,
                    Segment const& exp_resulting_segment,
                    Strategy const& strategy,
                    bool default_strategy)
{
#ifdef BOOST_GEOMETRY_TEST_DEBUG_CLOSEST_POINTS
    //std::cout << "CS: " << typeid(typename bg::cs_tag<Geometry1>::type).name()
    //          << std::endl;
    std::cout << bg::wkt(geometry1) << " --- " << bg::wkt(geometry2)
              << std::endl;
#endif
    Segment resulting_segment;
    if (default_strategy)
    {
        bg::closest_points(geometry1, geometry2, resulting_segment);
    }
    else
    {
        bg::closest_points(geometry1, geometry2, resulting_segment, strategy);
    }
#ifdef BOOST_GEOMETRY_TEST_DEBUG_CLOSEST_POINTS
    std::cout << "closest_points : " << bg::wkt(resulting_segment)
              << std::endl << std::endl;
#endif
    compare_result_with_expected(exp_resulting_segment, resulting_segment);
}

template
<
    typename Geometry1,
    typename Geometry2,
    typename Segment,
    typename Strategy
>
void compute_result(Geometry1 const& geometry1,
                    Geometry2 const& geometry2,
                    Segment const& exp_resulting_segment,
                    Strategy const& strategy,
                    bool swap_geometries,
                    bool default_strategy)
{
    compute_result(geometry1, geometry2, exp_resulting_segment, strategy,
                   default_strategy);
    if (swap_geometries)
    {
        // swap input geometries and expected segment
        compute_result(geometry2, geometry1,
                       swap(exp_resulting_segment), strategy,
                       default_strategy);
    }
}

//==============================================================================

template <typename CS_tag>
struct test_closest_points_dispatch
{};

template <>
struct test_closest_points_dispatch<bg::cartesian_tag>
{
    template
    <
        typename OutputSegment,
        typename Geometry1,
        typename Geometry2,
        typename Strategy
    >
    void static apply(Geometry1 const& geometry1,
                      Geometry2 const& geometry2,
                      std::string const& expected_resulting_segment_car,
                      std::string const& expected_resulting_segment_sph,
                      std::string const& expected_resulting_segment_geo,
                      Strategy const& strategy,
                      bool swap_geometries,
                      bool default_strategy)
    {
        boost::ignore_unused(expected_resulting_segment_sph);
        boost::ignore_unused(expected_resulting_segment_geo);

        OutputSegment expected_resulting_segment;
        bg::read_wkt(expected_resulting_segment_car,
                     expected_resulting_segment);
        compute_result(geometry1,
                       geometry2,
                       expected_resulting_segment,
                       strategy,
                       swap_geometries,
                       default_strategy);
    }
};

template <>
struct test_closest_points_dispatch<bg::spherical_equatorial_tag>
{
    template
    <
        typename OutputSegment,
        typename Geometry1,
        typename Geometry2,
        typename Strategy
    >
    void static apply(Geometry1 const& geometry1,
                      Geometry2 const& geometry2,
                      std::string const& expected_resulting_segment_car,
                      std::string const& expected_resulting_segment_sph,
                      std::string const& expected_resulting_segment_geo,
                      Strategy const& strategy,
                      bool swap_geometries,
                      bool default_strategy)
    {
        boost::ignore_unused(expected_resulting_segment_car);
        boost::ignore_unused(expected_resulting_segment_geo);

        OutputSegment expected_resulting_segment;
        bg::read_wkt(expected_resulting_segment_sph,
                     expected_resulting_segment);
        compute_result(geometry1,
                       geometry2,
                       expected_resulting_segment,
                       strategy,
                       swap_geometries,
                       default_strategy);
    }
};


template <>
struct test_closest_points_dispatch<bg::geographic_tag>
{
    template
    <
        typename OutputSegment,
        typename Geometry1,
        typename Geometry2,
        typename Strategy
    >
    void static apply(Geometry1 const& geometry1,
                      Geometry2 const& geometry2,
                      std::string const& expected_resulting_segment_car,
                      std::string const& expected_resulting_segment_sph,
                      std::string const& expected_resulting_segment_geo,
                      Strategy const& strategy,
                      bool swap_geometries,
                      bool default_strategy)
    {
        boost::ignore_unused(expected_resulting_segment_car);
        boost::ignore_unused(expected_resulting_segment_sph);

        OutputSegment expected_resulting_segment;
        bg::read_wkt(expected_resulting_segment_geo,
                     expected_resulting_segment);
        compute_result(geometry1,
                       geometry2,
                       expected_resulting_segment,
                       strategy,
                       swap_geometries,
                       default_strategy);
    }
};

//==============================================================================


template <typename Geometry1, typename Geometry2, typename OutputSegment>
struct test_geometry
{
    template <typename Strategy>
    inline static void apply(std::string const& wkt1,
                             std::string const& wkt2,
                             std::string const& expected_resulting_segment_car,
                             std::string const& expected_resulting_segment_sph,
                             std::string const& expected_resulting_segment_geo,
                             Strategy const& strategy,
                             bool swap_geometries = true,
                             bool default_strategy = false)
    {
        using CS_tag = typename bg::cs_tag<Geometry1>::type;

        Geometry1 geometry1;
        bg::read_wkt(wkt1, geometry1);
        Geometry2 geometry2;
        bg::read_wkt(wkt2, geometry2);

        test_closest_points_dispatch<CS_tag>
               ::template apply<OutputSegment>(geometry1,
                                               geometry2,
                                               expected_resulting_segment_car,
                                               expected_resulting_segment_sph,
                                               expected_resulting_segment_geo,
                                               strategy,
                                               swap_geometries,
                                               default_strategy);
    }

    template <typename Strategy>
    inline static void apply(std::string const& wkt1,
                             std::string const& wkt2,
                             std::string const& expected_resulting_segment_car,
                             std::string const& expected_resulting_segment_sph_geo,
                             Strategy const& strategy,
                             bool swap_geometries = true,
                             bool default_strategy = false)
    {
        apply(wkt1,
              wkt2,
              expected_resulting_segment_car,
              expected_resulting_segment_sph_geo,
              expected_resulting_segment_sph_geo,
              strategy,
              swap_geometries,
              default_strategy);
    }

    template <typename Strategy>
    inline static void apply(std::string const& wkt1,
                             std::string const& wkt2,
                             std::string const& expected_resulting_segment,
                             Strategy const& strategy,
                             bool swap_geometries = true,
                             bool default_strategy = false)
    {
        apply(wkt1,
              wkt2,
              expected_resulting_segment,
              expected_resulting_segment,
              expected_resulting_segment,
              strategy,
              swap_geometries,
              default_strategy);
    }
};

#endif // BOOST_GEOMETRY_TEST_ALGORITHMS_CLOSEST_POINTS_COMMON_HPP
