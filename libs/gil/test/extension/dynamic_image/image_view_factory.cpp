//
// Copyright 2022 Marco Langer <langer.m86 at gmail dot com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/gil/extension/dynamic_image/algorithm.hpp>
#include <boost/gil/extension/dynamic_image/image_view_factory.hpp>

#include <boost/core/lightweight_test.hpp>

#include <array>

#include "core/image/test_fixture.hpp"
#include "extension/dynamic_image/test_fixture.hpp"

namespace gil = boost::gil;
namespace fixture = boost::gil::test::fixture;

template <std::size_t Channels>
struct generator
{
    generator(int const* data) : data_(data) {}

    auto operator()() -> int
    {
        if(++i_ == Channels) {
            i_ = 0;
            return *data_++;
        }
        else
        {
            return *data_;
        }
    }

    int i_= 0;
    int const* data_;
};

struct test_flipped_up_down_view
{
    template <typename Image>
    void operator()(Image const&)
    {
        using image_t = Image;
        using pixel_t = typename image_t::value_type;
        static constexpr std::size_t num_channels = gil::num_channels<pixel_t>::value;

        std::array<int, 9> pixel_data =
        {
            0, 1, 2,
            3, 4, 5,
            6, 7, 8
        };
        std::array<int, 9> expected_pixel_data =
        {
            6, 7, 8,
            3, 4, 5,
            0, 1, 2
        };

        fixture::dynamic_image source_image =
            fixture::generate_image<image_t>(
                3, 3, generator<num_channels>{pixel_data.data()});

        fixture::dynamic_image expected_image =
            fixture::generate_image<image_t>(
                3, 3, generator<num_channels>{expected_pixel_data.data()});

        auto result_view = gil::flipped_up_down_view(gil::const_view(source_image));

        BOOST_TEST(
            gil::equal_pixels(
                result_view,
                gil::const_view(expected_image)));
    }

    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_flipped_up_down_view{});
    }
};


int main()
{
    test_flipped_up_down_view::run();

    return ::boost::report_errors();
}
