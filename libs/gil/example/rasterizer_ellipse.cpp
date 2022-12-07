//
// Copyright 2021 Prathamesh Tagore <prathameshtagore@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil.hpp>
#include <boost/gil/extension/io/jpeg.hpp>

namespace gil = boost::gil;

// Demonstrates the use of a rasterizer to generate an image of an ellipse
// The various rasterizers available are defined in include/boost/gil/extension/rasterization/circle.hpp,
// include/boost/gil/extension/rasterization/ellipse.hpp and include/boost/gil/extension/rasterization/line.hpp
// The rasterizer used is a generalisation of the midpoint algorithm often used for drawing circle.
// This examples also shows how to create images with various pixel depth, as well as the behaviour
// in case of the rasterization of a curve that doesn't fit in a view.
// See also:
// rasterizer_circle.cpp - Demonstrates the use of a rasterizer to generate an image of a circle
// rasterizer_line.cpp - Demonstrates the use of a rasterizer to generate an image of a line


int main()
{
    // Syntax for usage :-
    // auto rasterizer = gil::midpoint_ellipse_rasterizer{};
    // rasterizer(img_view, pixel, center, semi-axes_length);
    // Where
    // img_view : gil view of the image on which ellipse is to be drawn.
    // pixel : Pixel value for the elliptical curve to be drawn. Pixel's type must be compatible to the
    // pixel type of the image view.
    // center : Point containing positive integer x co-ordinate and y co-ordinate of the center
    // respectively.
    // semi-axes_length : Point containing positive integer lengths of horizontal semi-axis
    // and vertical semi-axis respectively.

    gil::gray8_image_t gray_buffer_image(256, 256);
    auto gray_ellipse_rasterizer = gil::midpoint_ellipse_rasterizer{{128, 128}, {100, 50}};
    gil::apply_rasterizer(view(gray_buffer_image), gray_ellipse_rasterizer, gil::gray8_pixel_t{128});

    gil::rgb8_image_t rgb_buffer_image(256, 256);
    auto rgb_ellipse_rasterizer = gil::midpoint_ellipse_rasterizer{{128, 128}, {50, 100}};
    gil::apply_rasterizer(view(rgb_buffer_image), rgb_ellipse_rasterizer, gil::rgb8_pixel_t{0, 0, 255});

    gil::rgb8_image_t rgb_buffer_image_out_of_bound(256, 256);
    auto rgb_ellipse_rasterizer_out_of_bound = gil::midpoint_ellipse_rasterizer{{100, 100}, {160, 160}};
    apply_rasterizer(view(rgb_buffer_image_out_of_bound), rgb_ellipse_rasterizer_out_of_bound, gil::rgb8_pixel_t{255, 0, 0});

    gil::write_view("rasterized_ellipse_gray.jpg", view(gray_buffer_image), gil::jpeg_tag{});
    gil::write_view("rasterized_ellipse_rgb.jpg", view(rgb_buffer_image), gil::jpeg_tag{});
    gil::write_view("rasterized_ellipse_rgb_out_of_bound.jpg", view(rgb_buffer_image_out_of_bound),
        gil::jpeg_tag{});
}
