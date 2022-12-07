//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil/image_view.hpp>
#include <boost/gil/histogram.hpp>
#include <boost/gil/color_base_algorithm.hpp>
#include <boost/gil/image_processing/histogram_matching.hpp>
#include <boost/gil/extension/toolbox/metafunctions/get_pixel_type.hpp>
#include <boost/core/lightweight_test.hpp>

#include <vector>
#include <iostream>

const int a = 5;
const double epsilon = 0.000001; // Decided by the value 5/255 i.e. an error of 5 px in 255 px
boost::gil::gray8_image_t original(a, a), reference(a, a);
boost::gil::gray8_image_t processed(a, a), processed2(a, a);
std::vector<std::vector<int> > test1_random{
    {  1,   10,  10,  10,  10},
    {  20,  25,  25,  55,  20},
    {  0,   55,  55,  55,  20},
    {  20,  255, 255, 255, 0},
    {  100, 100, 100, 10,  0}};
std::vector<std::vector<int> > test1_reference{
    {  0,   10,  20,  30,  40},
    {  50,  60,  70,  80,  90},
    {  100, 110, 120, 130, 140},
    {  150, 160, 170, 180, 190},
    {  200, 210, 220, 230, 240}};

std::vector<std::vector<int> > test2_uniform{
    {  0,   10,  20,  30,  40},
    {  50,  60,  70,  80,  90},
    {  100, 110, 120, 130, 140},
    {  150, 160, 170, 180, 190},
    {  200, 210, 220, 230, 240}};
std::vector<std::vector<int> > test2_reference{
    {  10,  20,  30,  40,  51},
    {  61,  71,  81,  91,  102},
    {  112, 122, 132, 142, 153},
    {  163, 173, 183, 193, 204},
    {  214, 224, 234, 244, 255}};

std::vector<std::vector<int> > test3_equal_image{
    {  0,   10,  20,  30,  40},
    {  50,  60,  70,  80,  90},
    {  100, 110, 120, 130, 140},
    {  150, 160, 170, 180, 190},
    {  200, 210, 220, 230, 240}};

std::vector<std::vector<int> > test3_reference{
    {  0,   10,  20,  30,  40},
    {  50,  60,  70,  80,  90},
    {  100, 110, 120, 130, 140},
    {  150, 160, 170, 180, 190},
    {  200, 210, 220, 230, 240}};

void vector_to_gray_image(boost::gil::gray8_image_t& img,
    std::vector<std::vector<int> >& grid)
{
    for(std::ptrdiff_t y=0; y<grid.size(); ++y)
    {
        for(std::ptrdiff_t x=0; x<grid[0].size(); ++x)
        {
            boost::gil::view(img)(x,y) = boost::gil::gray8_pixel_t(grid[y][x]);
        }
    }
}

template<typename SrcView>
bool equal_histograms(SrcView const& v1, SrcView const& v2, double threshold = epsilon)
{
    double sum=0.0;
    using channel_t = typename boost::gil::channel_type<SrcView>::type;
    boost::gil::histogram<channel_t> h1, h2;
    using value_t   = typename boost::gil::histogram<channel_t>::value_type;
    channel_t max_p = std::numeric_limits<channel_t>::max();
    channel_t min_p = std::numeric_limits<channel_t>::min();
    long int num_pixels = v1.width() * v1.height();

    boost::gil::fill_histogram(v1, h1, 1, false, false);
    boost::gil::fill_histogram(v2, h2, 1, false, false);
    auto ch1 = boost::gil::cumulative_histogram(h1);
    auto ch2 = boost::gil::cumulative_histogram(h2);
    std::for_each(ch1.begin(), ch1.end(), [&](value_t const& v) {
        sum+=abs(v.second-ch1[v.first]);
    });
    return ( abs(sum) / (ch1.size() * (max_p - min_p)) < threshold );
}

void test_random_image()
{
    vector_to_gray_image(original,test1_random);
    vector_to_gray_image(reference,test1_reference);
    histogram_matching(boost::gil::const_view(original),boost::gil::const_view(reference),boost::gil::view(processed));
    BOOST_TEST(equal_histograms(boost::gil::view(processed), boost::gil::view(reference)));

    histogram_matching(boost::gil::const_view(processed),boost::gil::const_view(reference),boost::gil::view(processed2));
    BOOST_TEST(equal_histograms(boost::gil::view(processed), boost::gil::view(processed2)));
}

void test_uniform_image()
{
    vector_to_gray_image(original,test2_uniform);
    vector_to_gray_image(reference,test2_reference);
    histogram_matching(boost::gil::const_view(original),boost::gil::const_view(reference),boost::gil::view(processed));
    BOOST_TEST(equal_histograms(boost::gil::view(processed), boost::gil::view(reference)));

    histogram_matching(boost::gil::const_view(processed),boost::gil::const_view(reference),boost::gil::view(processed2));
    BOOST_TEST(equal_histograms(boost::gil::view(processed), boost::gil::view(processed2)));
}

void test_equal_image()
{
    vector_to_gray_image(original,test3_equal_image);
    vector_to_gray_image(reference,test3_reference);
    histogram_matching(boost::gil::const_view(original),boost::gil::const_view(reference),boost::gil::view(processed));
    BOOST_TEST(equal_histograms(boost::gil::view(processed), boost::gil::view(reference)));

    histogram_matching(boost::gil::const_view(processed),boost::gil::const_view(reference),boost::gil::view(processed2));
    BOOST_TEST(equal_histograms(boost::gil::view(processed), boost::gil::view(processed2)));
}

int main()
{
    //Basic tests for grayscale histogram_equalization
    test_random_image();
    test_uniform_image();
    test_equal_image();

    return boost::report_errors();
}
