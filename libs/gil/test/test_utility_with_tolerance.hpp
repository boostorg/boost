//
// Copyright 2020 Samuel Debionne
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#ifndef BOOST_GIL_TEST_TEST_UTILITY_WITH_TOLERANCE_HPP
#define BOOST_GIL_TEST_TEST_UTILITY_WITH_TOLERANCE_HPP

#include <cstdint>
#include <ostream>
#include <type_traits>

namespace boost { namespace gil {

namespace test { namespace utility {

// Tolerance predicate for floating point comparison to use with BOOST_TEST_WITH.
// See https://github.com/boostorg/core/pull/77 for details.
template <typename T>
struct with_tolerance
{
    with_tolerance(T tolerance) : tolerance(tolerance)
    {
    }

    bool operator()(T lhs, T rhs)
    {
        return (std::abs(lhs - rhs) <= tolerance);
    }

private:
    T tolerance;
};

}} // namespace test::utility

}} // namespace boost::gil

#endif
