//
// Copyright 2022 Mateusz Loskot <mateusz at loskot dot net>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/gil.hpp>

#include <boost/core/lightweight_test.hpp>

#include "core/image/test_fixture.hpp"

namespace gil = boost::gil;
namespace fixture = boost::gil::test::fixture;

// See also test_is_1d_traversable in planar_rgba_view.cpp

struct test_continuous_image_types
{
    template <typename Image>
    void operator()(Image const&)
    {
        gil::point_t const dimensions{ 8, 3 };
        Image image(dimensions);
        auto& view = gil::view(image);
        BOOST_TEST(view.is_1d_traversable());
    }

    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_continuous_image_types{});
    }
};

struct test_interleaved_image_types
{
    template <typename Image>
    void operator()(Image const&)
    {
        gil::point_t const dimensions{ 8, 3 };
        Image image(dimensions);
        auto& view = gil::view(image);
        BOOST_TEST(view.is_1d_traversable());

        // planar copy from interleaved
        {
            using pixel_t = typename Image::view_t::value_type;
            gil::image<pixel_t, true> image2(image);
            auto& view2 = gil::view(image2);
            BOOST_TEST(view2.is_1d_traversable());
        }
    }

    static void run()
    {
        boost::mp11::mp_for_each<fixture::rgb_interleaved_image_types>(test_interleaved_image_types{});
    }
};

int main()
{
    test_continuous_image_types::run();
    test_interleaved_image_types::run();

    return ::boost::report_errors();
}
