// Boost.Geometry
// Unit Test

// Copyright (c) 2022 Barend Gehrels, Amsterdam, the Netherlands.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <test_geometries/const_point.hpp>

#include <boost/geometry/algorithms/detail/overlay/get_turns.hpp>

#include <boost/geometry/strategies/default_strategy.hpp>
#include <boost/geometry/strategies/relate/cartesian.hpp>
#include <boost/geometry/geometries/helper_geometry.hpp>

#include <geometry_test_common.hpp>


template <typename Ring1, typename Ring2>
void test_get_turns_on_const(Ring1 const &ring1, Ring2 const &ring2,
    const std::vector<const_point>& expectation)
{
    namespace bg = boost::geometry;

    using rescale_policy_type = bg::detail::no_rescale_policy;
    using strategy_type = typename bg::strategies::relate::services::default_strategy<Ring1, Ring2>::type;

    using point_type = typename bg::point_type<Ring1>::type;
    using writable_point_type = typename bg::helper_geometry<point_type>::type;

    using segment_ratio_type = typename bg::detail::segment_ratio_type
        <
            writable_point_type, rescale_policy_type
        >::type;
    using turn_info = bg::detail::overlay::turn_info
        <
            writable_point_type, segment_ratio_type
        >;

    std::vector<turn_info> turns;
    strategy_type strategy;
    bg::detail::get_turns::no_interrupt_policy policy;
    bg::get_turns
        <
            false, false, bg::detail::overlay::assign_null_policy
        >(ring1, ring2,  strategy, rescale_policy_type(), turns, policy);

    BOOST_CHECK_EQUAL(turns.size(), expectation.size());

    const double tolerance = 0.001;
    for (std::size_t i = 0; i < turns.size() && i < expectation.size(); i++) 
    {
        BOOST_CHECK_CLOSE(expectation[i].GetX(), bg::get<0>(turns[i].point), tolerance);
        BOOST_CHECK_CLOSE(expectation[i].GetY(), bg::get<1>(turns[i].point), tolerance);
    }

#ifdef BOOST_GEOMETRY_TEST_DEBUG
    std::cout << bg::wkt(ring1) << " " << bg::wkt(ring2) << std::endl;
    for (const auto& turn : turns) 
    {
        std::cout << bg::wkt(turn.point) << std::endl;
    }
#endif    
}

int test_main(int, char* [])
{
    // Construct two geometries with visually predictable intersections
    ring_of_const_point rectangle{{2, 2}, {2, 4}, {4, 4}, {4, 2}, {2, 2}};
    ring_of_const_point triangle{{1, 2}, {4, 5}, {5, 4}, {1, 2}};

    test_get_turns_on_const(rectangle, triangle, {{2, 3}, {2, 2.5}, {3, 4}, {4, 3.5}});

    return 0;
}

