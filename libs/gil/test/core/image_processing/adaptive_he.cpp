//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil/histogram.hpp>
#include <boost/gil/image_view.hpp>
#include <boost/gil/image_processing/histogram_equalization.hpp>
#include <boost/gil/image_processing/adaptive_histogram_equalization.hpp>

#include <boost/core/lightweight_test.hpp>

#include <cmath>
#include <vector>

namespace gil = boost::gil;

double epsilon = 1.0;

std::uint8_t image_matrix[] =
{
    1, 1, 1, 1,
    3, 3, 3, 3,
    5, 5, 5, 5,
    7, 7, 7, 7
};
gil::gray8c_view_t gray_view = gil::interleaved_view(4, 4, reinterpret_cast<gil::gray8c_pixel_t*>(image_matrix), 4);

void check_actual_clip_limit()
{
    gil::histogram<unsigned char> h;
    for(std::size_t i = 0; i < 100; i++)
    {
        if (i % 40 == 0)
        {
            h(i) = 60;
        }
        else
        {
            h(i) = 5;
        }
    }
    double limit = 0.01;
    double value = gil::detail::actual_clip_limit(h, limit);

    long actual_limit = round(value * h.sum()), max_bin_val = 0;
    double excess = 0;
    for(std::size_t i = 0; i < 100; i++)
    {
        if (h(i) > actual_limit)
            excess += actual_limit - h(i);
        max_bin_val = std::max<long>(max_bin_val, h(i));
    }
    BOOST_TEST((std::abs(excess / h.size() + actual_limit) - limit * h.sum()) < epsilon);
}

void check_clip_and_redistribute()
{
    gil::histogram<unsigned char> h, h2;
    for(std::size_t i = 0; i < 100; i++)
    {
        if (i % 50 == 0)
        {
            h(i) = 60;
        }
        else
        {
            h(i) = 5;
        }
    }
    bool check = true;
    double limit = 0.001;
    gil::detail::clip_and_redistribute(h, h2, limit);
    for(std::size_t i = 0; i < 100; i++)
    {
        check = check & (std::abs(limit * h.sum() - h2(i)) < epsilon);
    }
    BOOST_TEST(check);
}

void check_non_overlapping_interpolated_clahe()
{
    {
        gil::gray8_image_t img1(4, 4), img2(4, 4), img3(4, 4);
        gil::histogram_equalization(gray_view, view(img2));
        gil::non_overlapping_interpolated_clahe(gray_view, view(img3), 8, 8, 1.0);
        BOOST_TEST(gil::equal_pixels(view(img2), view(img3)));
    }
    {
        gil::gray8_image_t img1(8, 8), img2(8, 8), img3(4, 4);
        gil::copy_pixels(gray_view, gil::subimage_view(view(img1), 0, 0, 4, 4));
        gil::copy_pixels(gray_view, gil::subimage_view(view(img1), 0, 4, 4, 4));
        gil::copy_pixels(gray_view, gil::subimage_view(view(img1), 4, 0, 4, 4));
        gil::copy_pixels(gray_view, gil::subimage_view(view(img1), 4, 4, 4, 4));
        gil::histogram_equalization(gray_view, view(img3));
        gil::non_overlapping_interpolated_clahe(view(img1), view(img2), 8, 8, 1.0);
        BOOST_TEST(gil::equal_pixels(gil::subimage_view(view(img2), 0, 0, 4, 4), view(img3)));
        BOOST_TEST(gil::equal_pixels(gil::subimage_view(view(img2), 0, 4, 4, 4), view(img3)));
        BOOST_TEST(gil::equal_pixels(gil::subimage_view(view(img2), 4, 0, 4, 4), view(img3)));
        BOOST_TEST(gil::equal_pixels(gil::subimage_view(view(img2), 4, 4, 4, 4), view(img3)));
    }
}

int main()
{
    check_actual_clip_limit();
    check_clip_and_redistribute();
    check_non_overlapping_interpolated_clahe();

    return boost::report_errors();
}
