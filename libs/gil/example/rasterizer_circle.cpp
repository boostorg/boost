//
// Copyright 2020 Olzhas Zhumabek <anonymous.from.applecity@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/rasterization/circle.hpp>

namespace gil = boost::gil;

// Demonstrates the use of a rasterizer to generate an image of a circle
// The various rasterizers available are defined in include/boost/gil/extension/rasterization/circle.hpp,
// include/boost/gil/extension/rasterization/ellipse.hpp and include/boost/gil/extension/rasterization/line.hpp
// This example uses a trigonometric rasterizer; GIL also offers the rasterizer midpoint_circle_rasterizer,
// which implements the Midpoint algorithm.
// See also:
// rasterizer_ellipse.cpp - Demonstrates the use of a rasterizer to generate an image of an ellipse
// rasterizer_line.cpp - Demonstrates the use of a rasterizer to generate an image of a line

int main()
{
    const std::ptrdiff_t size = 256;
    gil::gray8_image_t buffer_image(size, size);
    auto buffer = gil::view(buffer_image);

    const gil::point_t center = {128, 128};
    const std::ptrdiff_t radius = 64;
    const auto rasterizer = gil::trigonometric_circle_rasterizer{center, radius};
    gil::apply_rasterizer(buffer, rasterizer, gil::gray8_pixel_t{255});

    gil::write_view("circle.png", buffer, gil::png_tag{});
}
