//
// Copyright 2019 Olzhas Zhumabek <anonymous.from.applecity@gmail.com>
// Copyright 2021 Pranam Lashkari <plashkari628@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/gil/typedefs.hpp>
#include <boost/gil/image_processing/numeric.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/image_processing/convolve.hpp>
#include <string>
#include <iostream>

namespace gil = boost::gil;

// Demonstrates the use of Sobel and Scharr filters to detect edges
// The example generates a couple of images representing respectively the result of
// applying the filter horizontally and vertically.
// The functions generating the filters (or kernels) are defined in include/boost/gil/image_processing/numeric.hpp.

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cerr << "usage: " << argv[0] << ": <input.png> <sobel|scharr> <output-x.png> <output-y.png>\n";
        return -1;
    }

    gil::gray8_image_t input_image;
    gil::read_image(argv[1], input_image, gil::png_tag{});
    auto input = gil::view(input_image);
    auto filter_type = std::string(argv[2]);

    gil::gray16_image_t dx_image(input_image.dimensions());
    auto dx = gil::view(dx_image);
    gil::gray16_image_t dy_image(input_image.dimensions());
    auto dy = gil::view(dy_image);
    if (filter_type == "sobel")
    {
        gil::detail::convolve_2d(input, gil::generate_dx_sobel(1), dx);
        gil::detail::convolve_2d(input, gil::generate_dy_sobel(1), dy);
    }
    else if (filter_type == "scharr")
    {
        gil::detail::convolve_2d(input, gil::generate_dx_scharr(1), dx);
        gil::detail::convolve_2d(input, gil::generate_dy_scharr(1), dy);
    }
    else
    {
        std::cerr << "unrecognized gradient filter type. Must be either sobel or scharr\n";
        return -1;
    }

    gil::write_view(argv[3], dx, gil::png_tag{});
    gil::write_view(argv[4], dy, gil::png_tag{});
}
