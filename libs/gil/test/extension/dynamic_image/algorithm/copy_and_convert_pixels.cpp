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

struct test_copy_and_convert_pixels
{
    template <typename ImageLhs, typename ImageRhs>
    void operator()(std::pair<ImageLhs, ImageRhs> const&)
    {
        using image_lhs_t = ImageLhs;
        using image_rhs_t = ImageRhs;
        using image_view_lhs_t = typename image_lhs_t::view_t;
        using image_view_rhs_t = typename image_rhs_t::view_t;

        gil::default_color_converter cc_default;
        image_lhs_t image_lhs1(fixture::create_image<image_lhs_t>(2, 2, 128));
        image_rhs_t image_rhs1(fixture::create_image<image_rhs_t>(2, 2, 128));
        fixture::dynamic_image dyn_image_lhs1(image_lhs1);
        fixture::dynamic_image dyn_image_rhs1(image_rhs1);
        {
            // dynamic_image -> image
            image_lhs_t image_lhs2(2, 2);
            image_lhs_t image_lhs3(2, 2);
            image_rhs_t image_rhs2(2, 2);
            image_rhs_t image_rhs3(2, 2);

            // lhs, lhs
            BOOST_TEST_NO_THROW(
                gil::copy_and_convert_pixels(
                    gil::const_view(dyn_image_lhs1),
                    gil::view(image_lhs2)));
            BOOST_TEST(
                gil::equal_pixels(
                    gil::const_view(dyn_image_lhs1),
                    gil::const_view(image_lhs2)));

            BOOST_TEST_NO_THROW(
                gil::copy_and_convert_pixels(
                    gil::const_view(dyn_image_lhs1),
                    gil::view(image_lhs3), cc_default));
            BOOST_TEST(
                gil::equal_pixels(
                    gil::const_view(dyn_image_lhs1),
                    gil::const_view(image_lhs3)));


            // lhs, rhs
            BOOST_TEST_NO_THROW(
                gil::copy_and_convert_pixels(
                    gil::const_view(dyn_image_lhs1),
                    gil::view(image_rhs2)));
            BOOST_TEST_NO_THROW(
                gil::copy_and_convert_pixels(
                    gil::const_view(dyn_image_lhs1),
                    gil::view(image_rhs3), cc_default));

           if (gil::views_are_compatible<image_view_lhs_t, image_view_rhs_t>::value)
           {
                BOOST_TEST(
                    gil::equal_pixels(
                        gil::const_view(dyn_image_rhs1),
                        gil::const_view(image_rhs2)));
                BOOST_TEST(
                    gil::equal_pixels(
                        gil::const_view(dyn_image_rhs1),
                        gil::const_view(image_rhs3)));
           }
        }
        {
            // image -> dynamic_image
            fixture::dynamic_image dyn_image_lhs2(image_lhs_t(2, 2));
            fixture::dynamic_image dyn_image_lhs3(image_lhs_t(2, 2));
            fixture::dynamic_image dyn_image_rhs2(image_rhs_t(2, 2));
            fixture::dynamic_image dyn_image_rhs3(image_rhs_t(2, 2));

            // lhs, lhs
            BOOST_TEST_NO_THROW(
                gil::copy_and_convert_pixels(
                    gil::const_view(image_lhs1),
                    gil::view(dyn_image_lhs2)));
            BOOST_TEST(
                gil::equal_pixels(
                    gil::const_view(image_lhs1),
                    gil::const_view(dyn_image_lhs2)));

            BOOST_TEST_NO_THROW(
                gil::copy_and_convert_pixels(
                    gil::const_view(image_lhs1),
                    gil::view(dyn_image_lhs3), cc_default));
            BOOST_TEST(
                gil::equal_pixels(
                    gil::const_view(image_lhs1),
                    gil::const_view(dyn_image_lhs3)));

            // lhs, rhs
            BOOST_TEST_NO_THROW(
                gil::copy_and_convert_pixels(
                    gil::const_view(image_lhs1),
                    gil::view(dyn_image_rhs2)));
            BOOST_TEST_NO_THROW(
                gil::copy_and_convert_pixels(
                    gil::const_view(image_lhs1),
                    gil::view(dyn_image_rhs3), cc_default));

           if (gil::views_are_compatible<image_view_lhs_t, image_view_rhs_t>::value)
           {
                BOOST_TEST(
                    gil::equal_pixels(
                        gil::const_view(image_lhs1),
                        gil::const_view(dyn_image_rhs2)));

                BOOST_TEST(
                    gil::equal_pixels(
                        gil::const_view(image_lhs1),
                        gil::const_view(dyn_image_rhs3)));
           }
        }
        {
            // dynamic_image -> dynamic_image
            fixture::dynamic_image dyn_image_lhs2(image_lhs_t(2, 2));
            fixture::dynamic_image dyn_image_lhs3(image_lhs_t(2, 2));
            fixture::dynamic_image dyn_image_rhs2(image_rhs_t(2, 2));
            fixture::dynamic_image dyn_image_rhs3(image_rhs_t(2, 2));

            // lhs, lhs
            BOOST_TEST_NO_THROW(
                gil::copy_and_convert_pixels(
                    gil::const_view(dyn_image_lhs1),
                    gil::view(dyn_image_lhs2)));
            BOOST_TEST(
                gil::equal_pixels(
                    gil::const_view(dyn_image_lhs1),
                    gil::const_view(dyn_image_lhs2)));

            BOOST_TEST_NO_THROW(
                gil::copy_and_convert_pixels(
                    gil::const_view(dyn_image_lhs1),
                    gil::view(dyn_image_lhs3), cc_default));
            BOOST_TEST(
                gil::equal_pixels(
                    gil::const_view(dyn_image_lhs1),
                    gil::const_view(dyn_image_lhs3)));

            // lhs, rhs
            BOOST_TEST_NO_THROW(
                gil::copy_and_convert_pixels(
                    gil::const_view(dyn_image_lhs1),
                    gil::view(dyn_image_rhs2)));
            BOOST_TEST_NO_THROW(
                gil::copy_and_convert_pixels(
                    gil::const_view(dyn_image_lhs1),
                    gil::view(dyn_image_rhs3), cc_default));

           if (gil::views_are_compatible<image_view_lhs_t, image_view_rhs_t>::value)
           {
                BOOST_TEST(
                    gil::equal_pixels(
                        gil::const_view(dyn_image_lhs1),
                        gil::const_view(dyn_image_rhs2)));

                BOOST_TEST(
                    gil::equal_pixels(
                        gil::const_view(dyn_image_lhs1),
                        gil::const_view(dyn_image_rhs3)));
           }
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
        >(test_copy_and_convert_pixels{});
    }
};

int main()
{
    test_copy_and_convert_pixels::run();

    return ::boost::report_errors();
}
