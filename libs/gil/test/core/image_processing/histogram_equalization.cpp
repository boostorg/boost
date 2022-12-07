//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil/color_base_algorithm.hpp>
#include <boost/gil/histogram.hpp>
#include <boost/gil/image_view.hpp>
#include <boost/gil/io/write_view.hpp>
#include <boost/gil/image_processing/histogram_equalization.hpp>
#include <boost/gil/extension/toolbox/metafunctions/get_pixel_type.hpp>

#include <boost/core/lightweight_test.hpp>

#include <vector>

const int a = 5;
const double epsilon = 0.005; // Decided by the value 1/255 i.e. an error of 1 px in 255 px
boost::gil::gray8_image_t original(a, a);
boost::gil::gray8_image_t processed_1(a, a), processed_2(a, a), expected(a, a);
std::vector<std::vector<int> > test1_random{
    {  1,   10,  10,  10,  10},
    {  20,  25,  25,  55,  20},
    {  0,   55,  55,  55,  20},
    {  20,  255, 255, 255, 0},
    {  100, 100, 100, 10,  0}};
std::vector<std::vector<int> > expected_test1{
    {  40,  91,  91,  91,  91},
    {  132, 153, 153, 193, 132},
    {  30,  193, 193, 193, 132},
    {  132, 255, 255, 255, 30},
    {  224, 224, 224, 91,  30}};

std::vector<std::vector<int> > all_white{
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255}};

std::vector<std::vector<int> > expected_all_white{
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255}};

std::vector<std::vector<int> > binary_img{
    {0  , 0  , 0  , 0  , 0  },
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255}};

std::vector<std::vector<int> > expected_binary_img{
    {51 , 51 , 51 , 51 , 51 },
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255},
    {255, 255, 255, 255, 255}};

std::vector<std::vector<int> > test2_uniform{
    {  0,   10,  20,  30,  40},
    {  50,  60,  70,  80,  90},
    {  100, 110, 120, 130, 140},
    {  150, 160, 170, 180, 190},
    {  200, 210, 220, 230, 240}};
std::vector<std::vector<int> > expected_test2{
    {  10,  20,  30,  40,  51},
    {  61,  71,  81,  91,  102},
    {  112, 122, 132, 142, 153},
    {  163, 173, 183, 193, 204},
    {  214, 224, 234, 244, 255}};

std::vector<std::vector<int> > test3_2peaks{
    {  0,   0,   0,   0,   10},
    {  40,  43,  44,  46,  50},
    {  55,  56,  44,  46,  44},
    {  200, 201, 202, 203, 200},
    {  201, 202, 201, 201, 22}};
std::vector<std::vector<int> > expected_test3{
    {  40,  40,  40,  40,  51},
    {  71,  81,  112, 132, 142},
    {  153, 163, 112, 132, 112},
    {  183, 224, 244, 255, 183},
    {  224, 244, 224, 224, 61}};

std::vector<std::vector<int> > test_mask{
    {1,   10,  10,  10,  10},
    {20,  25,  25,  25,  20},
    {0,   25,  25,  25,  20},
    {20,  25,  25,  25,  0},
    {100, 100, 100, 10,  0}};
std::vector<std::vector<int> > expected_test_mask{
    {1,   10,  10,  10,  10},
    {20,  255,  255,  255,  20},
    {0,   255,  255,  255,  20},
    {20,  255,  255,  255,  0},
    {100, 100, 100, 10,  0}};

std::vector<std::vector<bool> > mask{
    {0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {0, 1, 1, 1, 0},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 0, 0}};

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
bool equal_pixels(SrcView const& v1, SrcView const& v2, double threshold)
{
    double sum=0.0;
    using pixel_t = typename boost::gil::get_pixel_type<SrcView>::type;
    using channel_t = typename boost::gil::channel_type<SrcView>::type;
    channel_t max_p = std::numeric_limits<channel_t>::max();
    channel_t min_p = std::numeric_limits<channel_t>::min();
    long int num_pixels = v1.width() * v1.height();
    std::size_t num_channels = boost::gil::num_channels<SrcView>::value;
    for (std::ptrdiff_t y = 0; y < v1.height(); ++y)
    {
        auto it1 = v1.row_begin(y);
        auto it2 = v2.row_begin(y);
        for (std::ptrdiff_t x = 0; x < v2.width(); ++x)
        {
            for(std::ptrdiff_t c = 0; c < num_channels; ++c)
            {
                sum += abs(it1[x][c]-it2[x][c]);
            }
        }
    }
    return ( abs(sum) / (num_pixels * num_channels * (max_p - min_p)) < threshold );
}

void test_random_image()
{
    vector_to_gray_image(original,test1_random);
    vector_to_gray_image(expected,expected_test1);

    histogram_equalization(boost::gil::const_view(original),boost::gil::view(processed_1));
    BOOST_TEST(equal_pixels(boost::gil::view(processed_1), boost::gil::view(expected), epsilon));

    // Process image again to look for differences
    histogram_equalization(boost::gil::const_view(processed_1),boost::gil::view(processed_2));
    BOOST_TEST(equal_pixels(boost::gil::view(processed_1), boost::gil::view(processed_2), epsilon));

    // Test overloaded version
    boost::gil::histogram<unsigned char> hist, process_1, process_2;
    fill_histogram(boost::gil::const_view(original), hist, 1, false, false);
    histogram_equalization(hist, process_1);

    histogram_equalization(process_1, process_2);
    BOOST_TEST(process_1.equals(process_2));
}

void test_random_image_with_mask()
{
    vector_to_gray_image(original,test_mask);
    vector_to_gray_image(expected,expected_test_mask);
    histogram_equalization(boost::gil::const_view(original),boost::gil::view(processed_1), 1, true, mask);
    BOOST_TEST(equal_pixels(boost::gil::view(processed_1), boost::gil::view(expected), epsilon));

    // Process image again to look for differences
    histogram_equalization(boost::gil::const_view(processed_1),boost::gil::view(processed_2), 1, true, mask);
    BOOST_TEST(equal_pixels(boost::gil::view(processed_1), boost::gil::view(processed_2), epsilon));

    // Test overloaded version
    boost::gil::histogram<unsigned char> hist, process_1, process_2;
    fill_histogram(boost::gil::const_view(original), hist, 1, false, false);
    histogram_equalization(hist, process_1);

    histogram_equalization(process_1, process_2);
    BOOST_TEST(process_1.equals(process_2));
}

void test_uniform_image()
{
    vector_to_gray_image(original,test2_uniform);
    vector_to_gray_image(expected,expected_test2);
    histogram_equalization(boost::gil::const_view(original),boost::gil::view(processed_1));
    BOOST_TEST(equal_pixels(boost::gil::view(processed_1), boost::gil::view(expected), epsilon));

    // Process image again to look for differences
    histogram_equalization(boost::gil::const_view(processed_1),boost::gil::view(processed_2));
    BOOST_TEST(equal_pixels(boost::gil::view(processed_1), boost::gil::view(processed_2), epsilon));

    // Test overloaded version
    boost::gil::histogram<unsigned char> hist, process_1, process_2;
    fill_histogram(boost::gil::const_view(original), hist, 1, false, false);
    histogram_equalization(hist, process_1);

    histogram_equalization(process_1, process_2);
    BOOST_TEST(process_1.equals(process_2));
}

void test_all_white_image()
{
    vector_to_gray_image(original,all_white);
    vector_to_gray_image(expected,expected_all_white);
    histogram_equalization(boost::gil::const_view(original),boost::gil::view(processed_1));
    BOOST_TEST(equal_pixels(boost::gil::view(processed_1), boost::gil::view(expected), epsilon));

    // Process image again to look for differences
    histogram_equalization(boost::gil::const_view(processed_1),boost::gil::view(processed_2));
    BOOST_TEST(equal_pixels(boost::gil::view(processed_1), boost::gil::view(processed_2), epsilon));

    // Test overloaded version
    boost::gil::histogram<unsigned char> hist, process_1, process_2;
    fill_histogram(boost::gil::const_view(original), hist, 1, false, false);
    histogram_equalization(hist, process_1);

    histogram_equalization(process_1, process_2);
    BOOST_TEST(process_1.equals(process_2));
}

void test_binary_image()
{
    vector_to_gray_image(original,binary_img);
    vector_to_gray_image(expected,expected_binary_img);
    histogram_equalization(boost::gil::const_view(original),boost::gil::view(processed_1));
    BOOST_TEST(equal_pixels(boost::gil::view(processed_1), boost::gil::view(expected), epsilon));

    // Process image again to look for differences
    histogram_equalization(boost::gil::const_view(processed_1),boost::gil::view(processed_2));
    BOOST_TEST(equal_pixels(boost::gil::view(processed_1), boost::gil::view(processed_2), epsilon));

    // Test overloaded version
    boost::gil::histogram<unsigned char> hist, process_1, process_2;
    fill_histogram(boost::gil::const_view(original), hist, 1, false, false);
    histogram_equalization(hist, process_1);

    histogram_equalization(process_1, process_2);
    BOOST_TEST(process_1.equals(process_2));
}

void test_double_peaked_image()
{
    vector_to_gray_image(original,test3_2peaks);
    vector_to_gray_image(expected,expected_test3);
    histogram_equalization(boost::gil::const_view(original),boost::gil::view(processed_1));
    BOOST_TEST(equal_pixels(boost::gil::view(processed_1), boost::gil::view(expected), epsilon));

    // Process image again to look for differences
    histogram_equalization(boost::gil::const_view(processed_1),boost::gil::view(processed_2));
    BOOST_TEST(equal_pixels(boost::gil::view(processed_1), boost::gil::view(processed_2), epsilon));

    // Test overloaded version
    boost::gil::histogram<unsigned char> hist, process_1, process_2;
    fill_histogram(boost::gil::const_view(original), hist, 1, false, false);
    histogram_equalization(hist, process_1);

    histogram_equalization(process_1, process_2);
    BOOST_TEST(process_1.equals(process_2));
}

int main()
{
    //Basic tests for grayscale histogram_equalization
    test_random_image();
    test_random_image_with_mask();
    test_all_white_image();
    test_binary_image();
    test_uniform_image();
    test_double_peaked_image();

    return boost::report_errors();
}
