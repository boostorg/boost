//
// Copyright 2018-2020 Mateusz Loskot <mateusz at loskot dot net>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#include <boost/gil/algorithm.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/image_view_factory.hpp>

#include <boost/core/lightweight_test.hpp>

namespace gil = boost::gil;

void test_lambda_expression()
{
    gil::gray8_pixel_t const gray128(128);
    gil::gray8_image_t image(2, 2, gray128);

    int sum{0};
    gil::for_each_pixel(gil::view(image), [&sum](gil::gray8_pixel_t& p) {
        sum += gil::at_c<0>(p);
    });
    BOOST_TEST_EQ(sum, 2 * 2 * 128);
}

struct accumulator
{
    void operator()(gil::gray8_pixel_t const& p) {
        sum += gil::at_c<0>(p);
    }

    int sum = 0;
};

void test_function_object_1d_traversable()
{
    gil::gray8_pixel_t const gray128(128);
    gil::gray8_image_t image(2, 2, gray128);

    accumulator acc;
    acc = gil::for_each_pixel(
        gil::const_view(image),
        acc
    );
    BOOST_TEST_EQ(acc.sum, 2 * 2 * 128);
}


void test_function_object_not_1d_traversable()
{
    gil::gray8_pixel_t const gray128(128);
    gil::gray8_image_t image(4, 4, gray128);

    accumulator acc;
    acc = gil::for_each_pixel(
        gil::subimage_view(gil::const_view(image), gil::point_t{1, 1}, gil::point_t{2, 2}),
        acc
    );
    BOOST_TEST_EQ(acc.sum, 2 * 2 * 128);
}

int main()
{
    test_lambda_expression();
    test_function_object_1d_traversable();
    test_function_object_not_1d_traversable();

    return ::boost::report_errors();
}
