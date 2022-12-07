//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/image_processing/histogram_matching.hpp>

#include <vector>

using namespace boost::gil;

// Demonstrates Histogram Matching

// See also:
// histogram_equalization.cpp - Regular Histogram Equalization
// adaptive_histogram_equalization.cpp - Adaptive Histogram Equalization
// histogram.cpp - General use of histograms in GIL

std::vector<std::vector<bool>> get_mask(gray8_view_t const& mask)
{
    std::vector<std::vector<bool>> mask_vec(mask.height(), std::vector<bool>(mask.width(), 0));
    for (std::ptrdiff_t i = 0; i < mask.height(); i++)
    {
        for (std::ptrdiff_t j = 0; j < mask.width(); j++)
        {
            mask_vec[i][j] = mask(j, i);
        }
    }
    return mask_vec;
}

int main()
{
    gray8_image_t img;
    read_image("test_adaptive.png", img, png_tag{});

    gray8_image_t ref_img;
    read_image("test_adaptive.png", ref_img, png_tag{});

    gray8_image_t img_out(img.dimensions());

    boost::gil::histogram_matching(view(img), view(ref_img), view(img_out));

    write_view("histogram_gray_matching.png", view(img_out), png_tag{});

    return 0;
}
