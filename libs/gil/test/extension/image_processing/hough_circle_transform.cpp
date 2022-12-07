// Boost.GIL (Generic Image Library) - tests
//
// Copyright 2020 Olzhas Zhumabek <anonymous.from.applecity@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/core/lightweight_test.hpp>

#include <boost/gil/image.hpp>
#include <boost/gil/extension/image_processing/hough_transform.hpp>
#include <boost/gil/image_view.hpp>
#include <boost/gil/typedefs.hpp>

#include <cstddef>
#include <iostream>
#include <limits>
#include <vector>

namespace gil = boost::gil;

template <typename Rasterizer>
void exact_fit_test(std::ptrdiff_t radius, gil::point_t offset, Rasterizer rasterizer)
{
    std::vector<gil::point_t> circle_points(rasterizer.point_count());
    rasterizer(circle_points.begin());
    // const std::ptrdiff_t diameter = radius * 2 - 1;
    const std::ptrdiff_t width = offset.x + radius + 1;
    const std::ptrdiff_t height = offset.y + radius + 1;
    gil::gray8_image_t image(width, height);
    auto input = gil::view(image);

    for (const auto& point : circle_points)
    {
        input(point) = std::numeric_limits<gil::uint8_t>::max();
    }

    using param_t = gil::hough_parameter<std::ptrdiff_t>;
    const auto radius_parameter = param_t{radius, 0, 1};
    // const auto x_parameter = param_t::from_step_count(offset.x, neighborhood, half_step_count);
    // const auto y_parameter = param_t::from_step_count(offset.y, neighborhood, half_step_count);
    const auto x_parameter = param_t{offset.x, 0, 1};
    const auto y_parameter = param_t{offset.y, 0, 1};

    std::vector<gil::gray16_image_t> output_images(
        radius_parameter.step_count,
        gil::gray16_image_t(x_parameter.step_count, y_parameter.step_count));
    std::vector<gil::gray16_view_t> output_views(radius_parameter.step_count);
    std::transform(output_images.begin(), output_images.end(), output_views.begin(),
                   [](gil::gray16_image_t& img)
                   {
                       return gil::view(img);
                   });
    gil::hough_circle_transform_brute(input, radius_parameter, x_parameter, y_parameter,
                                      output_views.begin(), rasterizer);
    if (output_views[0](0, 0) != rasterizer.point_count())
    {
        std::cout << "accumulated value: " << static_cast<int>(output_views[0](0, 0))
                  << " expected value: " << rasterizer.point_count() << "\n\n";
    }
    BOOST_TEST(output_views[0](0, 0) == rasterizer.point_count());
}

int main()
{
    const int test_dim_length = 20;
    for (std::ptrdiff_t radius = 5; radius < test_dim_length; ++radius)
    {
        for (std::ptrdiff_t x_offset = radius; x_offset < radius + test_dim_length; ++x_offset)
        {
            for (std::ptrdiff_t y_offset = radius; y_offset < radius + test_dim_length; ++y_offset)
            {

                exact_fit_test(radius, {x_offset, y_offset},
                                gil::midpoint_circle_rasterizer{{x_offset, y_offset}, radius});
                exact_fit_test(radius, {x_offset, y_offset},
                               gil::trigonometric_circle_rasterizer{{x_offset, y_offset}, radius});
            }
        }
    }

    return boost::report_errors();
}
