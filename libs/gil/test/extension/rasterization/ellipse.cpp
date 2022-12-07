//
// Copyright 2021 Prathamesh Tagore <prathameshtagore@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil/point.hpp>
#include <boost/gil/extension/rasterization/ellipse.hpp>

#include <boost/core/lightweight_test.hpp>

#include <cmath>
#include <cstdint>
#include <vector>

namespace gil = boost::gil;

// This function utilizes the fact that sum of distances of a point on an ellipse from its foci
// is equal to the length of major axis of the ellipse.
// Parameters b and a represent half of lengths of vertical and horizontal axis respectively.
void test_rasterizer_follows_equation(
    std::vector<gil::point_t> const& trajectory_points, float a, float b)
{
    float focus_x, focus_y;
    if (a > b) // For horizontal ellipse
    {
        focus_x = a * std::sqrt(1 - b * b / (a * a));
        focus_y = 0;
    }
    else // For vertical ellipse
    {
        focus_x = 0;
        focus_y = b * std::sqrt(1 - a * a / (b * b));
    }

    for (auto trajectory_point : trajectory_points)
    {
        // To suppress conversion warnings from compiler
        gil::point<float> point {
            static_cast<float>(trajectory_point[0]), static_cast<float>(trajectory_point[1])};

        double dist_sum = std::sqrt(std::pow(focus_x - point[0], 2) +
            std::pow(focus_y - point[1], 2)) + std::sqrt(std::pow( - focus_x - point[0], 2) +
            std::pow( - focus_y - point[1], 2));
        if (a > b)
        {
            BOOST_TEST(std::abs(dist_sum - 2 * a) < 1);
        }
        else
        {
            BOOST_TEST(std::abs(dist_sum - 2 * b) < 1);
        }
    }
}

// This function verifies that the difference between x co-ordinates and y co-ordinates for two
// successive trajectory points is less than or equal to 1. This ensures that the curve is connected.
void test_connectivity(std::vector<gil::point_t> const& points)
{
    for (std::size_t i = 1; i < points.size(); ++i)
    {
        std::ptrdiff_t diff_x = points[i][0] - points[i - 1][0];
        std::ptrdiff_t diff_y = points[i][1] - points[i - 1][1];
        BOOST_TEST_LE(diff_x, 1);
        BOOST_TEST_LE(diff_y, 1);
    }
}

// We verify all test cases for the portion of ellipse in first quadrant, since all other portions
// can be constructed with simple reflection, they tend to be correct if first quadrant is verified.
int main()
{
    for (float a = 1; a < 101; ++a)
    {
        for (float b = 1; b < 101; ++b)
        {
            auto rasterizer = gil::midpoint_ellipse_rasterizer{{},
                {static_cast<unsigned int>(a), static_cast<unsigned int>(b)}};
            std::vector<gil::point_t> points = rasterizer.obtain_trajectory();
            test_rasterizer_follows_equation(points, a, b);
            test_connectivity(points);
        }
    }
    return boost::report_errors();
}
