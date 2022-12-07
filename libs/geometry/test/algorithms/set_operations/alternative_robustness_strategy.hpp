// Boost.Geometry (aka GGL, Generic Geometry Library)

// Copyright (c) 2021 Barend Gehrels, Amsterdam, the Netherlands.

// Licensed under the Boost Software License version 1.0.
// http://www.boost.org/users/license.html


#ifndef BOOST_GEOMETRY_TEST_INTERSECTION_ALTERNATIVE_ROBUSTNESS_STRATEGY_HPP
#define BOOST_GEOMETRY_TEST_INTERSECTION_ALTERNATIVE_ROBUSTNESS_STRATEGY_HPP

#include <boost/geometry/strategies/relate/cartesian.hpp>


template <typename SideStrategy>
class alternative_robustness_strategy
    : public bg::strategies::relate::cartesian<>
{
public:
    static auto side()
    {
        return SideStrategy();
    }
};


#endif // BOOST_GEOMETRY_TEST_INTERSECTION_ALTERNATIVE_ROBUSTNESS_STRATEGY_HPP
