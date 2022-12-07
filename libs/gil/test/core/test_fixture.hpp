//
// Copyright 2019 Mateusz Loskot <mateusz at loskot dot net>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#ifndef BOOST_GIL_TEST_CORE_TEST_FIXTURE_HPP
#define BOOST_GIL_TEST_CORE_TEST_FIXTURE_HPP

#include <boost/assert.hpp>
#include <boost/gil/promote_integral.hpp>

#include <cstdint>
#include <initializer_list>
#include <limits>
#include <random>
#include <tuple>
#include <type_traits>

namespace boost { namespace gil { namespace test { namespace fixture {

template <typename T>
struct consecutive_value
{
    consecutive_value(T start) : current_(start)
    {
        BOOST_ASSERT(static_cast<int>(current_) >= 0);
    }

    T operator()()
    {
        BOOST_ASSERT(static_cast<int>(current_) + 1 > 0);
        current_++;
        return current_;
    }

    T current_;
};

template <typename T>
struct reverse_consecutive_value
{
    reverse_consecutive_value(T start) : current_(start)
    {
        BOOST_ASSERT(static_cast<int>(current_) > 0);
    }

    T operator()()
    {
        BOOST_ASSERT(static_cast<int>(current_) + 1 >= 0);
        current_--;
        return current_;
    }

    T current_;
};

template <typename T>
struct random_value
{
    static_assert(std::is_integral<T>::value, "T must be integral type");

    random_value(T range_min = std::numeric_limits<T>::min(),
                 T range_max = std::numeric_limits<T>::max())
        : uid_(range_min, range_max)
    {}

    random_value(std::uint32_t seed, T minimum, T maximum) : rng_(seed), uid_(minimum, maximum)
    {}

    T operator()()
    {
        return uid_(rng_);
    }

    T range_min() const noexcept
    {
        return uid_.a();
    }

    T range_max() const noexcept
    {
        return uid_.b();
    }

    std::mt19937 rng_;
    std::uniform_int_distribution<typename gil::promote_integral<T>::type> uid_;
};

}}}} // namespace boost::gil::test::fixture

#endif
