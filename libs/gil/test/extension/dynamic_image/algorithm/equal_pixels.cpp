//
// Copyright 2022 Marco Langer <langer.m86 at gmail dot com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/gil/extension/dynamic_image/algorithm.hpp>
#include <boost/gil/extension/dynamic_image/any_image.hpp>

#include <boost/core/lightweight_test.hpp>

#include <utility>

#include "core/image/test_fixture.hpp"
#include "extension/dynamic_image/test_fixture.hpp"

namespace gil = boost::gil;
namespace fixture = boost::gil::test::fixture;

struct test_equal_pixels
{
    template <typename ImageLhs, typename ImageRhs>
    void operator()(std::pair<ImageLhs, ImageRhs> const&)
    {
        using image_lhs_t = ImageLhs;
        using image_rhs_t = ImageRhs;
        using image_view_lhs_t = typename image_lhs_t::view_t;
        using image_view_rhs_t = typename image_rhs_t::view_t;

        image_lhs_t image_lhs = fixture::create_image<image_lhs_t>(2, 2, 128);
        image_rhs_t image_rhs = fixture::create_image<image_rhs_t>(2, 2, 128);

        fixture::dynamic_image dyn_image_lhs(image_lhs);
        fixture::dynamic_image dyn_image_rhs(image_rhs);

        // lhs, lhs
        BOOST_TEST(
            gil::equal_pixels(
                gil::const_view(dyn_image_lhs),
                gil::const_view(image_lhs)));
        BOOST_TEST(
            gil::equal_pixels(
                gil::const_view(image_lhs),
                gil::const_view(dyn_image_lhs)));
        BOOST_TEST(
            gil::equal_pixels(
                gil::const_view(dyn_image_lhs),
                gil::const_view(dyn_image_lhs)));

        // lhs, rhs
        if (gil::views_are_compatible<image_view_lhs_t, image_view_rhs_t>::value)
        {
            BOOST_TEST(
                gil::equal_pixels(
                    gil::const_view(dyn_image_lhs),
                    gil::const_view(image_rhs)));
            BOOST_TEST(
                gil::equal_pixels(
                    gil::const_view(image_lhs),
                    gil::const_view(dyn_image_rhs)));
            BOOST_TEST(
                gil::equal_pixels(
                    gil::const_view(dyn_image_lhs),
                    gil::const_view(dyn_image_rhs)));
        }
        else
        {
            BOOST_TEST_THROWS(
                gil::equal_pixels(
                    gil::const_view(dyn_image_lhs),
                    gil::const_view(image_rhs)),
                std::bad_cast);
            BOOST_TEST_THROWS(
                gil::equal_pixels(
                    gil::const_view(image_lhs),
                    gil::const_view(dyn_image_rhs)),
                std::bad_cast);
            BOOST_TEST_THROWS(
                gil::equal_pixels(
                    gil::const_view(dyn_image_lhs),
                    gil::const_view(dyn_image_rhs)),
                std::bad_cast);
        }
    }

    static void run()
    {
        boost::mp11::mp_for_each
        <
            boost::mp11::mp_pairwise_fold
            <
                fixture::image_types, std::pair
            >
        >(test_equal_pixels{});
    }
};

int main()
{
    test_equal_pixels::run();

    return ::boost::report_errors();
}
