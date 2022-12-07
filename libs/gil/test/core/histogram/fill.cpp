//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/gil/histogram.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/image_view.hpp>
#include <boost/gil/typedefs.hpp>

#include <boost/core/lightweight_test.hpp>
#include <iostream>

namespace gil = boost::gil;
namespace mp11 = boost::mp11;

gil::gray8_image_t img1(4, 4, gil::gray8_pixel_t(1));
gil::gray8_view_t v1 = view(img1);

gil::rgb8_image_t img2(4, 4, gil::rgb8_pixel_t(1));
gil::rgb8_view_t v2 = view(img2);

std::uint8_t sparse_matrix[] =
{
    1, 1, 1, 1,
    3, 3, 3, 3,
    5, 5, 5, 5,
    7, 7, 7, 7
};

std::uint8_t big_matrix[] =
{
    1, 2, 3, 4, 5, 6, 7, 8,
    1, 2, 1, 2, 1, 2, 1, 2,
    1, 2, 3, 4, 5, 6, 7, 8,
    3, 4, 3, 4, 3, 4, 3, 4,
    1, 2, 3, 4, 5, 6, 7, 8,
    5, 6, 5, 6, 5, 6, 5, 6,
    1, 2, 3, 4, 5, 6, 7, 8,
    7, 8, 7, 8, 7, 8, 7, 8
};

std::uint8_t big_rgb_matrix[] =
{
    1, 2, 3, 2, 3, 4, 3, 4, 5, 4, 5, 6, 5, 6, 7, 6, 7, 8, 7, 8, 9, 8, 9, 10,
    1, 2, 3, 2, 3, 4, 1, 2, 3, 2, 3, 4, 1, 2, 3, 2, 3, 4, 1, 2, 3, 2, 3, 4,
    1, 2, 3, 2, 3, 4, 3, 4, 5, 4, 5, 6, 5, 6, 7, 6, 7, 8, 7, 8, 9, 8, 9, 10,
    3, 4, 5, 4, 5, 6, 3, 4, 5, 4, 5, 6, 3, 4, 5, 4, 5, 6, 3, 4, 5, 4, 5, 6,
    1, 2, 3, 2, 3, 4, 3, 4, 5, 4, 5, 6, 5, 6, 7, 6, 7, 8, 7, 8, 9, 8, 9, 10,
    5, 6, 7, 6, 7, 8, 5, 6, 7, 6, 7, 8, 5, 6, 7, 6, 7, 8, 5, 6, 7, 6, 7, 8,
    1, 2, 3, 2, 3, 4, 3, 4, 5, 4, 5, 6, 5, 6, 7, 6, 7, 8, 7, 8, 9, 8, 9, 10,
    7, 8, 9, 8, 9, 10, 7, 8, 9, 8, 9, 10, 7, 8, 9, 8, 9, 10, 7, 8, 9, 8, 9, 10,
};

std::vector<std::vector<bool>> mask =
{
    {1, 0, 0, 1},
    {0, 0, 1, 1},
    {0, 1, 0, 1},
    {1, 1, 0, 0},
};

gil::gray8c_view_t sparse_gray_view = gil::interleaved_view(4, 4, reinterpret_cast<gil::gray8c_pixel_t*>(sparse_matrix), 4);

gil::gray8c_view_t big_gray_view = gil::interleaved_view(8, 8, reinterpret_cast<gil::gray8c_pixel_t*>(big_matrix), 8);

gil::rgb8c_view_t big_rgb_view = gil::interleaved_view(8, 8, reinterpret_cast<gil::rgb8c_pixel_t*>(big_rgb_matrix), 24);

void check_histogram_fill_test1()
{
    gil::histogram<int> h1;

    h1.fill(big_gray_view);

    bool check_gray_fill = true;
    for (std::size_t i = 1; i <= 8; ++i)
    {
        if(h1(i) != 8)
        {
            check_gray_fill = false;
        }
    }
    BOOST_TEST(check_gray_fill);
}

void check_histogram_fill_test2()
{
    gil::histogram<int, int, int> h3;
    h3.fill(big_rgb_view);

    bool check_rgb_fill = true;
    for (std::size_t i = 1; i <= 8; ++i)
    {
        if(h3(i, i+1, i+2) != 8)
        {
            check_rgb_fill = false;
        }
    }
    BOOST_TEST(check_rgb_fill);
}

void check_histogram_fill_test3()
{
    gil::histogram<int> h2;
    h2.fill<1>(big_rgb_view);
    bool check_gray_fill2 = true;
    for (std::size_t i = 1; i <= 8; ++i)
    {
        if(h2(i+1) != 8)
        {
            check_gray_fill2 = false;
        }
    }
    BOOST_TEST(check_gray_fill2);
}

void check_histogram_fill_test4()
{
    gil::histogram<int> h1;
    // Check with limits
    std::tuple<int> lower{2}, higher{6};
    h1.clear();
    h1.fill(big_gray_view, 1, false, {{}}, lower, higher, true);
    bool check_gray_fill = true;
    check_gray_fill = true;
    for (std::size_t i = 1; i <= 8; ++i)
    {
        if(i<2 || i>6)
        {
            check_gray_fill = check_gray_fill & (h1(i)==0);continue;
        }
        if(h1(i) != 8)
        {
            check_gray_fill = false;
        }
    }
    BOOST_TEST(check_gray_fill);
}

void check_histogram_fill_test5()
{
    gil::histogram<int, int, int> h3;
    std::tuple<int, int ,int> lower1{2,2,2}, higher1{6,6,6};
    h3.clear();
    h3.fill(big_rgb_view, 1, false, {{}}, lower1, higher1, true);

    bool check_rgb_fill = true;
    check_rgb_fill = true;
    for (std::size_t i = 1; i <= 8; ++i)
    {
        if(!(i >= 2 && (i+2) <= 6))
        {
            check_rgb_fill = check_rgb_fill & (h3(i, i+1, i+2)==0);continue;
        }
        if(h3(i, i+1, i+2) != 8)
        {
            check_rgb_fill = false;
        }
    }
    BOOST_TEST(check_rgb_fill);
}

void check_histogram_fill_test6()
{
    gil::histogram<int> h2;
    h2.clear();
    std::tuple<int> lower{2}, higher{6};
    h2.fill<1>(big_rgb_view, 1, false, {{}}, lower, higher, true);
    bool check_gray_fill2 = true;
    check_gray_fill2 = true;
    for (std::size_t i = 1; i <= 8; ++i)
    {
        if(i+1 < 2 || i+1 > 6)
        {
            check_gray_fill2 = check_gray_fill2 & (h2(i+1)==0);continue;
        }
        if(h2(i+1) != 8)
        {
            check_gray_fill2 = false;
        }
    }
    BOOST_TEST(check_gray_fill2);
}

void check_histogram_fill_test7()
{
    //Check masking
    gil::histogram<int> h4;
    std::tuple<int> low{1}, high{8};
    gil::fill_histogram(sparse_gray_view, h4, 1, false, false, true, mask, low, high, true);

    bool check_1d = true;
    for (std::size_t i = 1; i <= 8; ++i)
    {
        if(i%2==1)
        {
            check_1d = check_1d & (h4(i)==2);
        }
    }
    BOOST_TEST(check_1d);
}

void check_histogram_fill_algorithm()
{
    gil::histogram<short> h1;

    gil::fill_histogram<1>(big_rgb_view, h1);

    bool check_1d = true;
    for (std::size_t i = 1; i <= 8; ++i)
    {
        if(h1(i+1) != 8)
        {
            check_1d = false;
        }
    }
    BOOST_TEST(check_1d);

    gil::histogram<int, int> h2;

    gil::fill_histogram<2, 1>(big_rgb_view, h2);

    bool check_2d = true;
    for (std::size_t i = 1; i <= 8; ++i)
    {
        if(h2(i+2, i+1) != 8)
        {
            check_2d = false;
        }
    }
    BOOST_TEST(check_2d);

    gil::histogram<int> h3;

    std::tuple<int> low(1), high(8);
    gil::fill_histogram(sparse_gray_view, h3, 1, false, false, false, {{}}, low, high, true);

    check_1d = true;
    for (std::size_t i = 1; i <= 8; ++i)
    {
        if(h3.find(std::tuple<int>(i)) == h3.end())
        {
            check_1d = false;
        }
        else
        {
            check_1d = check_1d & (i % 2 == 1 ? (h3(i) == 4) : (h3(i) == 0));
        }
    }
    BOOST_TEST(check_1d);
}

void check_fill_bin_width()
{
    gil::histogram<unsigned char> h1;
    gil::fill_histogram(big_gray_view, h1, 2);
    bool check1 = true;
    for(std::size_t i = 1; i <= 3; ++i)
    {
        check1 = check1 & (h1(i) == 16);
    }
    check1 = check1 & (h1(0) == 8) & (h1(4) == 8);
    BOOST_TEST(check1);
}

int main() {

    check_histogram_fill_test1();
    check_histogram_fill_test2();
    check_histogram_fill_test3();
    check_histogram_fill_test4();
    check_histogram_fill_test5();
    check_histogram_fill_test6();
    check_histogram_fill_test7();
    check_histogram_fill_algorithm();
    check_fill_bin_width();

    return boost::report_errors();
}