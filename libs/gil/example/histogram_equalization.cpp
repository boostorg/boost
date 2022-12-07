//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/image_processing/histogram_equalization.hpp>

using namespace boost::gil;

// Demonstrates Histogram Equalization

// See also:
// histogram.cpp - General use of histograms in GIL
// adaptive_histogram_equalization.cpp - Adaptive Histogram Equalization
// histogram_matching.cpp - Reference-based histogram computation

int main()
{
    gray8_image_t img;

    read_image("test_adaptive.png", img, png_tag{});
    gray8_image_t img_out(img.dimensions());

    // Consider changing image to independent color space, e.g. cmyk
    boost::gil::histogram_equalization(view(img),view(img_out));

    write_view("histogram_gray_equalized.png", view(img_out), png_tag{});

    return 0;
}
