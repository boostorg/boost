//
// Copyright 2022 Mateusz Loskot <mateusz at loskot dot net>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/gil.hpp>

#include <boost/core/lightweight_test.hpp>

#include "test_fixture.hpp"
#include "core/pixel/test_fixture.hpp"

namespace gil = boost::gil;
namespace fixture = boost::gil::test::fixture;

struct test_constructor_with_alignment
{
    template <typename Image>
    void operator()(Image const&)
    {
        std::size_t alignment = 0;
        Image image(alignment);
        BOOST_TEST(image.width() == 0);
        BOOST_TEST(image.height() == 0);
    }

    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_constructor_with_alignment{});
    }
};

struct test_constructor_with_alignment_allocator
{
    template <typename Image>
    void operator()(Image const&)
    {
        std::size_t alignment = 0;
        std::allocator<unsigned char> allocator;
        Image image(alignment, allocator);
        BOOST_TEST(image.width() == 0);
        BOOST_TEST(image.height() == 0);
    }

    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_constructor_with_alignment_allocator{});
    }
};

struct test_constructor_with_dimensions_alignment
{
    template <typename Image>
    void operator()(Image const&)
    {
        gil::point_t const dimensions{256, 128};
        std::size_t alignment = 0;
        Image image(dimensions, alignment);
        BOOST_TEST(image.width() == dimensions.x);
        BOOST_TEST(image.height() == dimensions.y);
    }

    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_constructor_with_dimensions_alignment{});
    }
};

struct test_constructor_with_dimensions_alignment_allocator
{
    template <typename Image>
    void operator()(Image const&)
    {
        gil::point_t const dimensions{256, 128};
        std::size_t alignment = 0;
        std::allocator<unsigned char> allocator;
        Image image(dimensions, alignment, allocator);
        BOOST_TEST(image.width() == dimensions.x);
        BOOST_TEST(image.height() == dimensions.y);
    }

    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_constructor_with_dimensions_alignment_allocator{});
    }
};

struct test_constructor_with_dimensions_pixel_alignment
{
    template <typename Image>
    void operator()(Image const &)
    {
        gil::point_t const dimensions{3, 3};
        using pixel_t = typename Image::view_t::value_type;
        pixel_t const rnd_pixel = fixture::pixel_generator<pixel_t>::random();

        Image image(dimensions, rnd_pixel, 32u);
        BOOST_TEST(image.width() == dimensions.x);
        BOOST_TEST(image.height() == dimensions.y);

        for (pixel_t const& p : gil::view(image))
        {
            BOOST_TEST(p == rnd_pixel);
        }
    }

    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_constructor_with_dimensions_pixel_alignment{});
    }
};

int main()
{
    test_constructor_with_alignment::run();
    test_constructor_with_alignment_allocator::run();
    test_constructor_with_dimensions_alignment::run();
    test_constructor_with_dimensions_alignment_allocator::run();
    test_constructor_with_dimensions_pixel_alignment::run();
    test_constructor_with_dimensions_pixel_alignment::run();

    return ::boost::report_errors();
}
