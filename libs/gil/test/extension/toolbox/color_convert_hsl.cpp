//
// Copyright 2013 Christian Henning
// Copyright 2020 Mateusz Loskot <mateusz@loskot.net>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/gil.hpp>
#include <boost/gil/extension/toolbox/color_spaces/hsl.hpp>

#include <boost/core/lightweight_test.hpp>

#include <cstdint>
#include <tuple>
#include <vector>

#include "test_utility_output_stream.hpp"
#include "test_utility_with_tolerance.hpp"

namespace gil = boost::gil;

void test_basic_hsl_to_rgb()
{
    gil::rgb8_pixel_t p(64, 0, 64);
    gil::hsl32f_pixel_t h;
    gil::color_convert(p, h);

    gil::rgb8_pixel_t b;
    gil::color_convert(h, b);
    BOOST_TEST_EQ(b, p);
}

void test_colors_hsl_to_rgb()
{
    using color_t = std::tuple<float, float, float, std::uint8_t, std::uint8_t, std::uint8_t>;
    std::vector<color_t> colors = {
        color_t{0, 0, 0, 0, 0, 0},               // black
        color_t{0, 0, 1, 255, 255, 255},         // white
        color_t{0, 1, 0.5, 255, 0, 0},           // red
        color_t{2 / 6.f, 1, 0.5, 0, 255, 0},     // lime
        color_t{4 / 6.f, 1, 0.5, 0, 0, 255},     // blue
        color_t{5 / 6.f, 1, 0.25, 128, 0, 128},  // purple
        color_t{3 / 6.f, 1, 0.25, 0, 128, 128},  // teal
        color_t{4 / 6.f, 1, 0.25, 0, 0, 128},    // navy
    };

    for (auto&& color : colors)
    {
        float h{0}, s{0}, l{0};
        std::uint8_t r{0}, g{0}, b{0};
        std::tie(h, s, l, r, g, b) = color;

        gil::hsl32f_pixel_t hsl(h, s, l);
        gil::rgb8_pixel_t rgb;
        gil::color_convert(hsl, rgb);

        float const abs_error = 1;
        BOOST_TEST_WITH(
            r, gil::get_color(rgb, gil::red_t()),
            gil::test::utility::with_tolerance<double>(abs_error));
        BOOST_TEST_WITH(
            g, gil::get_color(rgb, gil::green_t()),
            gil::test::utility::with_tolerance<double>(abs_error));
        BOOST_TEST_WITH(
            b, gil::get_color(rgb, gil::blue_t()),
            gil::test::utility::with_tolerance<double>(abs_error));
    }
}

void test_image_assign_hsl()
{
    std::ptrdiff_t const w = 320;
    std::ptrdiff_t const h = 240;
    gil::hsl32f_image_t hsl_img(w, h);

    for (std::ptrdiff_t y = 0; y < h; y++)
    {
        gil::hsl32f_view_t::x_iterator hsl_x_it = view(hsl_img).row_begin(y);
        float v                                 = static_cast<float>(h - y) / h;
        for (std::ptrdiff_t x = 0; x < w; x++)
        {
            float const hue = (x + 1.f) / w;
            gil::hsl32f_pixel_t const p(hue, 1.0, v);
            hsl_x_it[x] = p;
            BOOST_TEST_EQ(gil::view(hsl_img)(x, y), p);
        }
    }
}

int main()
{
    test_basic_hsl_to_rgb();
    test_colors_hsl_to_rgb();
    test_image_assign_hsl();

    return ::boost::report_errors();
}
