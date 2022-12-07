//
// Copyright 2019 Miral Shah <miralshah2211@gmail.com>
// Copyright 2019-2021 Pranam Lashkari <plashkari628@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/gil.hpp>
#include <boost/gil/image_processing/convolve.hpp>

#include <boost/core/lightweight_test.hpp>

#include <cstddef>

namespace gil = boost::gil;

std::uint8_t img[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 255, 0, 0, 0, 255, 0, 0,
    0, 0, 0, 255, 0, 255, 0, 0, 0,
    0, 0, 0, 0, 255, 0, 0, 0, 0,
    0, 0, 0, 255, 0, 255, 0, 0, 0,
    0, 0, 255, 0, 0, 0, 255, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

std::uint8_t output[] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 28, 28, 28, 0, 28, 28, 28, 0,
    0, 28, 56, 56, 56, 56, 56, 28, 0,
    0, 28, 56, 85, 85, 85, 56, 28, 0,
    0, 0, 56, 85, 141, 85, 56, 0, 0,
    0, 28, 56, 85, 85, 85, 56, 28, 0,
    0, 28, 56, 56, 56, 56, 56, 28, 0,
    0, 28, 28, 28, 0, 28, 28, 28, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

void test_convolve_2d_with_normalized_mean_filter()
{
    gil::gray8c_view_t src_view =
        gil::interleaved_view(9, 9, reinterpret_cast<const gil::gray8_pixel_t*>(img), 9);

    gil::image<gil::gray8_pixel_t> temp_img(src_view.width(), src_view.height());
    typename gil::image<gil::gray8_pixel_t>::view_t temp_view = view(temp_img);
    gil::gray8_view_t dst_view(temp_view);

    std::vector<float> v(9, 1.0f / 9.0f);
    gil::detail::kernel_2d<float> kernel(v.begin(), v.size(), 1, 1);

    gil::detail::convolve_2d(src_view, kernel, dst_view);

    gil::gray8c_view_t out_view =
        gil::interleaved_view(9, 9, reinterpret_cast<const gil::gray8_pixel_t*>(output), 9);

    BOOST_TEST(gil::equal_pixels(out_view, dst_view));
}

void test_convolve_2d_with_image_using_float32_t()
{
gil::float32_t img[] =
{
    0.1, 0.2, 0.1, 0.1, 0.4, 0.1, 0.76, 0.1, 0.1,
    0.1, 0.2, 0.1, 0.1, 0.4, 0.1, 0.5, 0.1, 0.1,
    0.1, 0.2, 0.1, 0.1, 0.4, 0.1, 0.5, 0.1, 0.1,
    0.1, 0.6, 0.6, 0.1, 0.54, 0.1, 0.5, 0.1, 0.1,
    0.1, 0.2, 0.1, 0.1, 0.4, 0.1, 0.5, 0.1, 0.1,
    0.1, 0.2, 0.1, 0.84, 0.4, 0.1, 0.5, 0.1, 0.1,
    0.1, 0.1, 0.1, 0.1, 0.4, 0.1, 0.5, 0.1, 0.1,
    0.1, 0.3, 0.1, 0.1, 0.4, 0.1, 0.32, 0.1, 0.1,
    0.1, 0.2, 0.1, 0.1, 0.4, 0.1, 0.21, 0.1, 0.1
};

gil::float32_t exp_output[] =
{
    0.1, 0.2, 0.1, 0.1, 0.4, 0.1, 0.76, 0.1, 0.1,
    0.1, 0.2, 0.1, 0.1, 0.4, 0.1, 0.5, 0.1, 0.1,
    0.1, 0.2, 0.1, 0.1, 0.4, 0.1, 0.5, 0.1, 0.1,
    0.1, 0.6, 0.6, 0.1, 0.54, 0.1, 0.5, 0.1, 0.1,
    0.1, 0.2, 0.1, 0.1, 0.4, 0.1, 0.5, 0.1, 0.1,
    0.1, 0.2, 0.1, 0.84, 0.4, 0.1, 0.5, 0.1, 0.1,
    0.1, 0.1, 0.1, 0.1, 0.4, 0.1, 0.5, 0.1, 0.1,
    0.1, 0.3, 0.1, 0.1, 0.4, 0.1, 0.32, 0.1, 0.1,
    0.1, 0.2, 0.1, 0.1, 0.4, 0.1, 0.21, 0.1, 0.1
};

gil::gray32f_image_t img_gray_out(9, 9);
gil::gray32fc_view_t src_view =
        gil::interleaved_view(9, 9, reinterpret_cast<gil::gray32f_pixel_t const*>(img), 9);
gil::gray32fc_view_t exp_out_view =
        gil::interleaved_view(9, 9, reinterpret_cast<gil::gray32f_pixel_t const*>(exp_output), 9);
std::vector<float> v(1, 1);
gil::detail::kernel_2d<float> kernel(v.begin(), v.size(), 0, 0); // impulse kernel
gil::detail::convolve_2d(src_view, kernel, view(img_gray_out));
BOOST_TEST(gil::equal_pixels(exp_out_view, view(img_gray_out)));
}

int main()
{
    test_convolve_2d_with_normalized_mean_filter();
    test_convolve_2d_with_image_using_float32_t();
    return ::boost::report_errors();
}
