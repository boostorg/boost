//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/gil/extension/histogram/std.hpp>
#include <boost/gil/image_view.hpp>
#include <boost/gil/image.hpp>
#include <boost/core/lightweight_test.hpp>

// Supported Container types for histogram
#include <vector>
#include <map>
#include <array>
#include <unordered_map>

// Basic tests to make sure compatible container types produce
// expected output histogram.

namespace gil = boost::gil;

gil::gray8_image_t img1(4, 4, gil::gray8_pixel_t(1));
gil::gray8_view_t v1 = view(img1);

gil::rgb8_image_t img2(4, 4, gil::rgb8_pixel_t(1));
gil::rgb8_view_t v2 = view(img2);

gil::gray16_image_t img3(4, 4, gil::gray16_pixel_t(1));
gil::gray16_view_t v3 = view(img3);

template<typename T>
bool check_equal(T &cont1, T &cont2, std::size_t size)
{
    bool ch = true;
    for(std::size_t i = 0; i < size; ++i)
    {
        ch = ch & (cont1[i] == cont2[i]);
    }
    return ch;
}

template<typename T>
bool check_equal(T &cont1, T &cont2)
{
    bool ch = true;
    for(auto &it : cont1)
    {
        ch = ch & (cont1[it.first] == cont2[it.first]);
    }
    return ch;
}

void check_fill_histogram_vector()
{
    std::vector<int> c1, c1_expected(256,0);
    c1_expected[1] = 16;
    gil::fill_histogram(v1, c1);
    BOOST_TEST(check_equal(c1, c1_expected, c1_expected.size()));

    c1_expected[1] = 32;
    gil::fill_histogram(v1, c1, true);
    BOOST_TEST(check_equal(c1, c1_expected, c1_expected.size()));

    std::vector<int> c2, c2_expected;
    gil::fill_histogram(v2, c2);
    gil::fill_histogram(gil::color_converted_view<gil::gray8_pixel_t>(v2), c2_expected);
    BOOST_TEST(check_equal(c2, c2_expected, c2_expected.size()));
}


void check_fill_histogram_array()
{
    std::array<int, 256> c1{0}, c1_expected{0};
    c1_expected[1] = 16;
    gil::fill_histogram(v1, c1);
    BOOST_TEST(check_equal(c1, c1_expected, c1_expected.size()));

    c1_expected[1] = 32;
    gil::fill_histogram(v1, c1, true);
    BOOST_TEST(check_equal(c1, c1_expected, c1_expected.size()));

    std::array<int, 256> c2{0}, c2_expected{0};
    gil::fill_histogram(v1, c2);
    gil::fill_histogram(gil::color_converted_view<gil::gray8_pixel_t>(v2), c2_expected);
    BOOST_TEST(check_equal(c2, c2_expected, c2_expected.size()));

    // Check binning
    std::array<int, 256> c3{0}, c3_expected{0};
    c3_expected[0] = 16;
    gil::fill_histogram(v3, c3);
    BOOST_TEST(check_equal(c3, c3_expected, c3_expected.size()));
}

void check_fill_histogram_map()
{
    std::map<int, int> c1, c1_expected;
    c1_expected[1] = 16;
    gil::fill_histogram(v1, c1);
    BOOST_TEST(check_equal(c1, c1_expected));

    c1_expected[1] = 32;
    gil::fill_histogram(v1, c1, true);
    BOOST_TEST(check_equal(c1, c1_expected));

    std::map<int, int> c2, c2_expected;
    gil::fill_histogram(v2, c2);
    gil::fill_histogram(gil::color_converted_view<gil::gray8_pixel_t>(v2), c2_expected);
    BOOST_TEST(check_equal(c2, c2_expected));
}

void check_cumulative_histogram_vector()
{
    std::vector<int> v(8);
    for (std::size_t i = 0; i < v.size(); i++)
    {
        v[i] = 1;
    }
    auto v1 = gil::cumulative_histogram(v);
    bool check = true;
    for (std::size_t i = 0; i < v.size(); i++)
    {
        if(v1[i] != int(i) + 1)
            check = false;
    }
    BOOST_TEST(check);
}

void check_cumulative_histogram_array()
{
    std::array<int, 8> arr;
    for (std::size_t i = 0; i < arr.size(); i++)
    {
        arr[i] = 1;
    }
    auto arr1 = gil::cumulative_histogram(arr);
    bool check = true;
    for (std::size_t i = 0; i < arr.size(); i++)
    {
        if(arr1[i] != int(i) + 1)
            check = false;
    }
    BOOST_TEST(check);
}

void check_cumulative_histogram_map()
{
    std::map<int, int> mp;
    for (std::size_t i = 0; i < 8; i++)
    {
        mp[i] = 1;
    }
    auto mp1 = gil::cumulative_histogram(mp);
    bool check = true;
    for (std::size_t i = 0; i < mp.size(); i++)
    {
        if(mp1[i] != int(i) + 1)
            check = false;
    }
    BOOST_TEST(check);
}

int main()
{
    check_fill_histogram_vector();
    check_fill_histogram_array();
    check_fill_histogram_map();

    check_cumulative_histogram_vector();
    check_cumulative_histogram_array();
    check_cumulative_histogram_map();

    return boost::report_errors();
}
