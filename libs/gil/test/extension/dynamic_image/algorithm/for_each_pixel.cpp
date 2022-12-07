//
// Copyright 2022 Marco Langer <langer.m86 at gmail dot com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/gil/extension/dynamic_image/algorithm.hpp>
#include <boost/gil/extension/dynamic_image/any_image.hpp>

#include <boost/core/lightweight_test.hpp>

#include "core/image/test_fixture.hpp"
#include "extension/dynamic_image/test_fixture.hpp"

namespace gil = boost::gil;
namespace fixture = boost::gil::test::fixture;

struct accumulator
{
    template <typename Pixel>
    void operator()(Pixel const& p)
    {
        sum += gil::at_c<0>(p);
    }

    int sum{};
};

struct test_for_each_pixel
{
    template <typename Image>
    void operator()(Image const&)
    {
        using image_t = Image;
        fixture::dynamic_image image(fixture::create_image<image_t>(2, 2, 128));
        accumulator acc = gil::for_each_pixel(gil::const_view(image), accumulator());
        BOOST_TEST_EQ(acc.sum, 2 * 2 * 128);
    }

    static void run()
    {
        boost::mp11::mp_for_each<fixture::image_types>(test_for_each_pixel{});
    }
};

int main()
{
    test_for_each_pixel::run();

    return ::boost::report_errors();
}
