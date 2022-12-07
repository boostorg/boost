//
// Copyright 2020 Olzhas Zhumabek <anonymous.from.applecity@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/extension/rasterization/line.hpp>

namespace gil = boost::gil;

// Demonstrates the use of a rasterizer to generate an image of a line
// The various rasterizers available are defined in include/boost/gil/extension/rasterization/circle.hpp,
// include/boost/gil/extension/rasterization/ellipse.hpp and include/boost/gil/extension/rasterization/line.hpp
// The rasterizer used implements the Bresenham's line algorithm.
// Multiple images are created, all of the same size, but with areas of different sizes passed to the rasterizer, resulting in different lines.
// See also:
// rasterizer_circle.cpp - Demonstrates the use of a rasterizer to generate an image of a circle
// rasterizer_ellipse.cpp - Demonstrates the use of a rasterizer to generate an image of an ellipse


const std::ptrdiff_t size = 256;

void line_bresenham(std::ptrdiff_t width, std::ptrdiff_t height, const std::string& output_name)
{
    const auto rasterizer = gil::bresenham_line_rasterizer{{0, 0}, {width - 1, height - 1}};

    gil::gray8_image_t image(size, size);
    auto view = gil::view(image);
    gil::apply_rasterizer(view, rasterizer, gil::gray8_pixel_t{255});
    gil::write_view(output_name, view, gil::png_tag{});
}

int main()
{
    line_bresenham(256, 256, "line-bresenham-256-256.png");
    line_bresenham(256, 128, "line-bresenham-256-128.png");
    line_bresenham(256, 1, "line-bresenham-256-1.png");
    line_bresenham(1, 256, "line-bresenham-1-256.png");
}
