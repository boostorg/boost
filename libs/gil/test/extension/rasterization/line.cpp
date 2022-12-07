//
// Copyright 2020 Olzhas Zhumabek <anonymous.from.applecity@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "core/point/test_fixture.hpp"

#include <boost/gil/point.hpp>
#include <boost/gil/extension/rasterization/line.hpp>

#include <boost/core/lightweight_test.hpp>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iterator>
#include <random>
#include <vector>

namespace gil = boost::gil;

using line_type = std::vector<gil::point_t>;

struct endpoints
{
    gil::point_t start;
    gil::point_t end;
};

endpoints create_endpoints(std::mt19937& twister,
                           std::uniform_int_distribution<std::ptrdiff_t>& distr)
{
    gil::point_t start{distr(twister), distr(twister)};
    gil::point_t end{distr(twister), distr(twister)};
    return {start, end};
}

line_type create_line(endpoints points)
{
    gil::bresenham_line_rasterizer rasterizer(points.start, points.end);
    line_type forward_line(rasterizer.point_count());
    rasterizer(forward_line.begin());
    return forward_line;
}

void test_start_end(line_type const& line_points, endpoints points)
{
    BOOST_TEST_EQ(line_points.front(), points.start);
    BOOST_TEST_EQ(line_points.back(), points.end);
}

// Look at TODO below
// void test_two_way_equivalence(const line_type& forward, line_type backward)
// {
//     std::reverse(backward.begin(), backward.end());
//     BOOST_TEST_ALL_EQ(forward.begin(), forward.end(), backward.begin(), backward.end());
// }

void test_connectivity(line_type const& line_points)
{
    for (std::size_t i = 1; i < line_points.size(); ++i)
    {
        const auto x_diff = std::abs(line_points[i].x - line_points[i - 1].x);
        const auto y_diff = std::abs(line_points[i].y - line_points[i - 1].y);
        BOOST_TEST_LE(x_diff, 1);
        BOOST_TEST_LE(y_diff, 1);
    }
}

void test_bresenham_rasterizer_follows_equation(line_type line_points)
{
    auto start = line_points.front();
    auto end = line_points.back();

    auto width = std::abs(end.x - start.x) + 1;
    auto height = std::abs(end.y - start.y) + 1;
    if (width < height)
    {
        std::swap(width, height);
        std::transform(line_points.begin(), line_points.end(), line_points.begin(),
                       [](gil::point_t p)
                       {
                           return gil::point_t{p.y, p.x};
                       });
        // update start and end
        start = line_points.front();
        end = line_points.back();
    }
    const double sign = [start, end]()
    {
        auto const width_sign = end.x < start.x;
        auto const height_sign = end.y < start.y;
        auto const slope_sign = width_sign != height_sign;
        return slope_sign ? -1 : 1;
    }();
    const double slope = static_cast<double>(height) / static_cast<double>(width);
    const double intercept =
        static_cast<double>(start.y) - sign * slope * static_cast<double>(start.x);
    for (const auto& point : line_points)
    {
        double const expected_y = sign * slope * static_cast<double>(point.x) + intercept;
        auto const difference =
            std::abs(point.y - static_cast<std::ptrdiff_t>(std::round(expected_y)));
        BOOST_TEST_LE(difference, static_cast<std::ptrdiff_t>(slope + 1));
    }
}

int main()
{
    const std::ptrdiff_t size = 256;
    for (std::size_t seed = 0; seed <= 100; ++seed)
    {
        std::mt19937 twister(seed);
        std::uniform_int_distribution<std::ptrdiff_t> distr(0, size - 1);
        const std::size_t sample_count = 100;
        for (std::size_t sample_index = 0; sample_index < sample_count; ++sample_index)
        {
            auto endpoints = create_endpoints(twister, distr);
            auto forward_line = create_line(endpoints);
            test_start_end(forward_line, endpoints);
            // TODO: figure out if forward/backward equivalence is possible to provide
            // auto backward_line = create_line({endpoints.end, endpoints.start});
            // test_two_way_equivalence(forward_line, backward_line);
            test_connectivity(forward_line);
            // test_connectivity(backward_line);
            test_bresenham_rasterizer_follows_equation(forward_line);
            // test_bresenham_rasterizer_follows_equation(backward_line);
        }
    }

    return boost::report_errors();
}
