//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/io/read_image.hpp>

#include <iostream>

using namespace boost::gil;

// Explains how to use the histogram class and some of its features
// that can be applied for a variety of tasks.

// See also:
// histogram_equalization.cpp - Regular Histogram Equalization
// adaptive_histogram_equalization.cpp - Adaptive Histogram Equalization
// histogram_matching.cpp - Reference-based histogram computation

int main()
{
   // Create a histogram class. Use uint or unsigned short as the default axes type in most cases.
    histogram<unsigned char> h;

    // Fill histogram with GIL images (of any color space)
    gray8_image_t g;
    read_image("test_adaptive.png", g, png_tag{});

    fill_histogram
    (
        view(g), // Input image view
        h,       // Histogram to be filled
        1,       // Histogram bin widths
        false,   // Specify whether to accumulate over the values already present in h (default = false)
        true,    // Specify whether to have a sparse (true) or continuous histogram (false) (default = true)
        false,   // Specify if image mask is to be specified
        {{}},    // Mask as a 2D vector. Used only if prev argument specified
        {0},     // Lower limit on the values in histogram (default numeric_limit::min() on axes)
        {255},   // Upper limit on the values in histogram (default numeric_limit::max() on axes)
        true     // Use specified limits if this is true (default is false)
    );

    // Normalize the histogram
    h.normalize();

    // Get a cumulative histogram from the histogram
    auto h2 = cumulative_histogram(h);

    return 0;
}
