//
// Copyright 2020 Olzhas Zhumabek <anonymous.from.applecity@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil/point.hpp>
#include <boost/gil/extension/rasterization/circle.hpp>

#include <boost/core/lightweight_test.hpp>

#include <cmath>
#include <cstdint>
#include <vector>

namespace gil = boost::gil;

template <typename Rasterizer>
void test_rasterizer_follows_equation(Rasterizer rasterizer)
{
    std::ptrdiff_t const radius = rasterizer.radius;
    std::vector<gil::point_t> circle_points(rasterizer.point_count());
    std::ptrdiff_t const r_squared = radius * radius;
    rasterizer(circle_points.begin());
    std::vector<gil::point_t> first_octant(rasterizer.point_count() / 8);

    for (std::size_t i = 0, octant_index = 0; i < circle_points.size(); i += 8, ++octant_index)
    {
        first_octant[octant_index] = circle_points[i];
    }

    for (auto const& point : first_octant)
    {
        double const y_exact = std::sqrt(r_squared - point.x * point.x);
        std::ptrdiff_t lower_result = static_cast<std::ptrdiff_t>(std::floor(y_exact));
        std::ptrdiff_t upper_result = static_cast<std::ptrdiff_t>(std::ceil(y_exact));
        BOOST_TEST(point.y >= lower_result && point.y <= upper_result);
    }
}

template <typename Rasterizer>
void test_connectivity(Rasterizer rasterizer)
{
    std::vector<gil::point_t> circle_points(rasterizer.point_count());
    rasterizer(circle_points.begin());
    for (std::size_t i = 0; i < 8; ++i)
    {
        std::vector<gil::point_t> octant(circle_points.size() / 8);
        for (std::size_t octant_index = i, index = 0; octant_index < circle_points.size();
             octant_index += 8, ++index)
        {
            octant[index] = circle_points[octant_index];
        }

        for (std::size_t index = 1; index < octant.size(); ++index)
        {
            const auto diff_x = std::abs(octant[index].x - octant[index - 1].x);
            const auto diff_y = std::abs(octant[index].y - octant[index - 1].y);
            BOOST_TEST_LE(diff_x, 1);
            BOOST_TEST_LE(diff_y, 1);
        }
    }
}

int main()
{
    for (std::ptrdiff_t radius = 5; radius <= 512; ++radius)
    {
        test_rasterizer_follows_equation(gil::midpoint_circle_rasterizer{{0, 0}, radius});
        // TODO: find out a new testing procedure for trigonometric rasterizer
        // test_equation_following(radius, gil::trigonometric_circle_rasterizer{});
        test_connectivity(gil::midpoint_circle_rasterizer{{radius, radius}, radius});
        test_connectivity(gil::trigonometric_circle_rasterizer{{radius, radius}, radius});
    }

    return boost::report_errors();
}
