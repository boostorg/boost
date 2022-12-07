//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
// Copyright 2021 Pranam Lashkari <plashkari628@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/algorithm.hpp>
#include <boost/gil/image_processing/adaptive_histogram_equalization.hpp>

using namespace boost::gil;

// Demonstrates Adaptive Histogram Equalization (AHE)

// See also:
// histogram.cpp - General use of histograms in GIL
// histogram_equalization.cpp - Regular Histogram Equalization
// histogram_matching.cpp - Reference-based histogram computation

int main()
{
    gray8_image_t img;
    read_image("test_adaptive.png", img, png_tag{});
    gray8_image_t img_out(img.dimensions());

    boost::gil::non_overlapping_interpolated_clahe(view(img), view(img_out));
    write_view("out-adaptive.png", view(img_out), png_tag{});
    return 0;
}
