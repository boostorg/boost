//
// Copyright 2019 Miral Shah <miralshah2211@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <iostream>

using namespace boost::gil;

// Demonstrates Adaptive Thresholding

// threshold_adaptive works following either the mean or the gaussian method, and accepts also a direction
// The direction indicates whether the pixels are assigned the max value when their values are greater
// than the threshold (regular), or when they are less than the threshold (inverse)
// threshold_adaptive is defined in include/boost/gil/image_processing/threshold.hpp
// See also:
// threshold.cpp - Simple thresholding

int main()
{
    gray8_image_t img;
    read_image("test_adaptive.png", img, png_tag{});
    gray8_image_t img_out(img.dimensions());

    boost::gil::threshold_adaptive(const_view(img), view(img_out), 11, threshold_adaptive_method::mean, threshold_direction::regular, 2);
    write_view("out-threshold-adaptive-mean.png", view(img_out), png_tag{});

    boost::gil::threshold_adaptive(const_view(img), view(img_out), 11, threshold_adaptive_method::mean, threshold_direction::inverse, 2);
    write_view("out-threshold-adaptive-mean-inv.png", view(img_out), png_tag{});

    boost::gil::threshold_adaptive(const_view(img), view(img_out), 7, threshold_adaptive_method::gaussian, threshold_direction::regular, 2);
    write_view("out-threshold-adaptive-gaussian.png", view(img_out), png_tag{});

    boost::gil::threshold_adaptive(const_view(img), view(img_out), 11, threshold_adaptive_method::gaussian, threshold_direction::inverse, 2);
    write_view("out-threshold-adaptive-gaussian-inv.png", view(img_out), png_tag{});

    return 0;
}
