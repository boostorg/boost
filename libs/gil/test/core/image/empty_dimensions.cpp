//
// Copyright 2019-2020 Mateusz Loskot <mateusz at loskot dot net>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/gil.hpp>

#include <boost/core/lightweight_test.hpp>

#include "test_fixture.hpp"
#include "test_utility_output_stream.hpp"
#include "core/pixel/test_fixture.hpp"

namespace gil = boost::gil;
namespace fixture = boost::gil::test::fixture;

// Test cases of memory leak potential for image created with empty dimesions,
// see https://github.com/boostorg/gil/pull/649
// The main goal of these test cases is to trigger any memory leak detectors.

void test_default_constructor()
{
    boost::gil::rgb8_image_t image;
    BOOST_TEST_EQ(image.width(), 0);
    BOOST_TEST_EQ(image.height(), 0);
}

void test_copy_constructor_with_empty_image()
{
    boost::gil::rgb8_image_t image1;
    boost::gil::rgb8_image_t image2(image1);
    BOOST_TEST_EQ(image2.width(), 0);
    BOOST_TEST_EQ(image2.height(), 0);
}

struct test_constructor_with_empty_dimensions
{
    template <typename Image>
    void operator()(Image const &)
    {
        using image_t = Image;
        image_t image(0, 0);
        BOOST_TEST_EQ(image.width(), 0);
        BOOST_TEST_EQ(image.height(), 0);
    }

    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_constructor_with_empty_dimensions{});
    }
};

struct test_constructor_with_empty_dimensions_with_pixel
{
    template <typename Image>
    void operator()(Image const &)
    {
        using image_t = Image;
        gil::point_t const dimensions{0, 0};
        using pixel_t = typename image_t::view_t::value_type;
        pixel_t const rnd_pixel = fixture::pixel_generator<pixel_t>::random();
        image_t image(dimensions, rnd_pixel);
        BOOST_TEST_EQ(image.width(), dimensions.x);
        BOOST_TEST_EQ(image.height(), dimensions.y);

        bool none_visited = true;
        for (pixel_t const &p : gil::view(image))
        {
            none_visited = false;
            BOOST_TEST_EQ(p, rnd_pixel);
        }
        BOOST_TEST(none_visited);
    }

    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_constructor_with_empty_dimensions_with_pixel{});
    }
};

int main()
{
    test_default_constructor();
    test_copy_constructor_with_empty_image();
    test_constructor_with_empty_dimensions::run();
    test_constructor_with_empty_dimensions_with_pixel::run();

    return ::boost::report_errors();
}
