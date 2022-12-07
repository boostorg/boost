//
// Copyright 2019 Miral Shah <miralshah2211@gmail.com>
// Copyright 2019 Mateusz Loskot <mateusz at loskot dot net>
// Copyright 2021 Pranam Lashkari <plashkari628@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <iostream>
#include <boost/gil/image_processing/kernel.hpp>
#include <boost/gil/image_processing/convolve.hpp>
#include <boost/gil/extension/io/png.hpp>

#include <boost/gil/extension/io/jpeg.hpp>

using namespace boost::gil;
using namespace std;

// Convolves the image with a 2d kernel.

// Note that the kernel can be fixed or resizable:
// kernel_2d_fixed<float, N> k(elements, centre_y, centre_x) produces a fixed kernel
// kernel_2d<float> k(elements, size, centre_y, centre_x) produces a resizable kernel
// The size of the kernel matrix is deduced as the square root of the number of the elements (9 elements yield a 3x3 matrix)

// See also:
// convolution.cpp - Convolution with 2d kernels


int main()
{
    gray8_image_t img;
    read_image("src_view.png", img, png_tag{});
    gray8_image_t img_out(img.dimensions()), img_out1(img.dimensions());

    std::vector<float> v(9, 1.0f / 9.0f);
    detail::kernel_2d<float> kernel(v.begin(), v.size(), 1, 1);
    detail::convolve_2d(view(img), kernel, view(img_out1));

    write_view("out-convolve2d.png", view(img_out1), png_tag{});

    std::vector<float> v1(3, 1.0f / 3.0f);
    kernel_1d<float> kernel1(v1.begin(), v1.size(), 1);

    detail::convolve_1d<gray32f_pixel_t>(const_view(img), kernel1, view(img_out), boundary_option::extend_zero);
    write_view("out-convolve_option_extend_zero.png", view(img_out), png_tag{});

    if (equal_pixels(view(img_out1), view(img_out)))
      cout << "convolve_option_extend_zero" << endl;

    return 0;
}
