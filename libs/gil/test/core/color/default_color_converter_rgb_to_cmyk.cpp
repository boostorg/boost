#include <boost/gil.hpp>
#include <boost/gil/color_convert.hpp>
#include <boost/gil/rgb.hpp>
#include <boost/gil/cmyk.hpp>
#include <boost/mp11.hpp>
#include <boost/core/lightweight_test.hpp>
#include <type_traits>

#include "test_utility_output_stream.hpp"

namespace gil = boost::gil;

void test_unsigned_rgb_to_unsigned_cmyk()
{
    // black
    {
        gil::rgb8_pixel_t src{0, 0, 0};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{0,0,0,255};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    // white
    {
        gil::rgb8_pixel_t src{255, 255, 255};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{0, 0, 0, 0};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    // blue
    {
        gil::rgb8_pixel_t src{0, 0, 255};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{255, 255, 0, 0};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    // yellow
    {
        gil::rgb8_pixel_t src{255, 255, 0};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{0, 0, 255, 0};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 1
    {
        gil::rgb8_pixel_t src{1, 2, 3};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{170, 85, 0, 252};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 2
    {
        gil::rgb8_pixel_t src{11, 117, 250};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{243, 135, 0, 5};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 3
    {
        gil::rgb8_pixel_t src{230, 45, 100};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{0, 205, 144, 25};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 4
    {
        gil::rgb8_pixel_t src{200, 255, 53};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{55, 0, 202, 0};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
}

void test_signed_rgb_to_unsigned_cmyk()
{
    // black
    {
        gil::rgb8s_pixel_t src{-128, -128, -128};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{0,0,0,255};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    // white
    {
        gil::rgb8s_pixel_t src{127, 127, 127};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{0, 0, 0, 0};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    // blue
    {
        gil::rgb8s_pixel_t src{-128, -128, 127};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{255, 255, 0, 0};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    // yellow
    {
        gil::rgb8s_pixel_t src{127, 127, -128};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{0, 0, 255, 0};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 1
    {
        gil::rgb8s_pixel_t src{-127, -126, -125};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{170, 85, 0, 252};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 2
    {
        gil::rgb8s_pixel_t src{-117, -11, 122};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{243, 135, 0, 5};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 3
    {
        gil::rgb8s_pixel_t src{102, -83, -28};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{0, 205, 144, 25};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 4
    {
        gil::rgb8s_pixel_t src{72, 127, -75};
        gil::cmyk8_pixel_t dst;
        gil::cmyk8_pixel_t expected_dst{55, 0, 202, 0};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
}

void test_unsigned_rgb_to_signed_cmyk()
{
    // black
    {
        gil::rgb8_pixel_t src{0, 0, 0};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{-128, -128, -128, 127};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    // white
    {
        gil::rgb8_pixel_t src{255, 255, 255};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{-128, -128, -128, -128};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    // blue
    {
        gil::rgb8_pixel_t src{0, 0, 255};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{127, 127, -128, -128};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    // yellow
    {
        gil::rgb8_pixel_t src{255, 255, 0};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{-128, -128, 127, -128};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 1
    {
        gil::rgb8_pixel_t src{1, 2, 3};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{42, -43, -128, 124};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 2
    {
        gil::rgb8_pixel_t src{11, 117, 250};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{115, 7, -128, -123};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 3
    {
        gil::rgb8_pixel_t src{230, 45, 100};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{-128, 77, 16, -103};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 4
    {
        gil::rgb8_pixel_t src{200, 255, 53};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{-73, -128, 74, -128};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
}

void test_signed_rgb_to_signed_cmyk()
{
    // black
    {
        gil::rgb8s_pixel_t src{-128, -128, -128};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{-128,-128,-128, 127};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    // white
    {
        gil::rgb8s_pixel_t src{127, 127, 127};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{-128, -128, -128, -128};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    // blue
    {
        gil::rgb8s_pixel_t src{-128, -128, 127};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{127, 127, -128, -128};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    // yellow
    {
        gil::rgb8s_pixel_t src{127, 127, -128};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{-128, -128, 127, -128};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 1
    {
        gil::rgb8s_pixel_t src{-127, -126, -125};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{42, -43, -128, 124};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 2
    {
        gil::rgb8s_pixel_t src{-117, -11, 122};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{115, 7, -128, -123};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 3
    {
        gil::rgb8s_pixel_t src{102, -83, -28};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{-128, 77, 16, -103};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
    //random pixel values 4
    {
        gil::rgb8s_pixel_t src{72, 127, -75};
        gil::cmyk8s_pixel_t dst;
        gil::cmyk8s_pixel_t expected_dst{-73, -128, 74, -128};

        gil::color_convert(src, dst);
        BOOST_TEST_EQ(dst, expected_dst);
    }
}

int main()
{
    test_unsigned_rgb_to_unsigned_cmyk();
    test_signed_rgb_to_unsigned_cmyk();
    test_unsigned_rgb_to_signed_cmyk();
    test_signed_rgb_to_signed_cmyk();
    return ::boost::report_errors();
}
