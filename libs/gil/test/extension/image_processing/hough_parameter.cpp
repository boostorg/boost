// Boost.GIL (Generic Image Library) - tests
//
// Copyright 2020 Olzhas Zhumabek <anonymous.from.applecity@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <boost/core/lightweight_test.hpp>
#include <boost/gil/extension/image_processing/hough_parameter.hpp>

namespace gil = boost::gil;

void from_step_count_test()
{
    const double middle_point = 0.5;
    const std::size_t step_count = 5;
    const double neighborhood = 1.0;
    auto param =
        gil::hough_parameter<double>::from_step_count(middle_point, neighborhood, step_count);
    BOOST_TEST(param.start_point == middle_point - neighborhood);
    BOOST_TEST(param.step_count == step_count * 2 + 1);
    BOOST_TEST(param.step_size == neighborhood / step_count);

    bool middle_point_occured = false;
    for (std::size_t i = 0; i < param.step_count; ++i)
    {
        auto current = param.start_point + param.step_size * i;
        if (current == middle_point)
        {
            middle_point_occured = true;
            break;
        }
    }
    BOOST_TEST(middle_point_occured);
}

void from_step_size_test(const double middle_point, const double step_size,
                         const double neighborhood)
{
    const std::size_t expected_step_count =
        static_cast<std::size_t>(neighborhood / step_size) * 2 + 1;
    auto param =
        gil::hough_parameter<double>::from_step_size(middle_point, neighborhood, step_size);
    BOOST_TEST(param.start_point == middle_point - step_size * std::floor(expected_step_count / 2));
    BOOST_TEST(param.step_count == expected_step_count);
    BOOST_TEST(param.step_size == step_size);

    bool middle_point_occured = false;
    for (std::size_t i = 0; i < param.step_count; ++i)
    {
        auto current = param.start_point + param.step_size * i;
        if (current == middle_point)
        {
            middle_point_occured = true;
            break;
        }
    }
    BOOST_TEST(middle_point_occured);
}

void minimum_step_angle_test(const std::ptrdiff_t width, const std::ptrdiff_t height)
{
    const auto bigger_dim = width > height ? width : height;
    const double expected_angle = std::atan2(1.0, bigger_dim);
    BOOST_TEST(expected_angle == gil::minimum_angle_step({width, height}));
}

int main()
{
    from_step_count_test();
    // ideal case
    from_step_size_test(2.0, 0.25, 1.0);
    from_step_size_test(5.0, 2, 5.0);
    minimum_step_angle_test(1200, 800);
    minimum_step_angle_test(800, 1200);
    return boost::report_errors();
}
