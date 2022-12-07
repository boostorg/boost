//
// Copyright 2022 Mateusz Loskot <mateusz at loskot dot net>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#ifndef BOOST_GIL_TEST_CORE_VIRTUAL_2D_LOCATOR_TEST_FIXTURE_HPP
#define BOOST_GIL_TEST_CORE_VIRTUAL_2D_LOCATOR_TEST_FIXTURE_HPP

#include <boost/gil.hpp>
#include <boost/assert.hpp>

#include <stdexcept>

#include "core/pixel/test_fixture.hpp"

namespace boost { namespace gil { namespace test { namespace fixture {

template <typename Pixel>
struct noop_dereference_fn
{
    static constexpr bool is_mutable =false;

    using const_t = noop_dereference_fn;
    using value_type = Pixel;
    using reference = value_type;
    using const_reference = value_type;
    using argument_type = gil::point_t;
    using result_type = reference;

    static_assert(std::is_same<value_type, reference>::value, "value_type != reference");
    static_assert(std::is_same<value_type, result_type>::value, "value_type != result_type");

    result_type operator()(gil::point_t const&) const
    {
        return result_type{};
    }
};

template <typename Pixel>
struct locator_dereference_fn
{
    static constexpr bool is_mutable =false;

    using const_t = locator_dereference_fn;
    using value_type = Pixel;
    using reference = value_type;
    using const_reference = value_type;
    using argument_type = gil::point_t;
    using result_type = reference;

    static_assert(std::is_same<value_type, reference>::value, "value_type != reference");
    static_assert(std::is_same<value_type, result_type>::value, "value_type != result_type");

    locator_dereference_fn() = default;
    locator_dereference_fn(point_t const& dims) : _dimensions(dims) {}

    result_type operator()(gil::point_t const& position) const
    {
        if (position.x >= _dimensions.x)
            throw std::out_of_range("position x out of range");
        if (position.y >= _dimensions.y)
            throw std::out_of_range("position y out of range");

        auto pixel = pixel_generator<value_type>::random();
        return pixel;
    }

private:
    gil::point_t _dimensions;
};

}}}} // namespace boost::gil::test::fixture

#endif
