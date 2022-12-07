//
// Copyright 2022 Marco Langer <langer.m86@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil.hpp>
#include <boost/gil/extension/rasterization/circle.hpp>
#include <boost/gil/extension/rasterization/ellipse.hpp>
#include <boost/gil/extension/rasterization/line.hpp>

#include <boost/core/lightweight_test.hpp>

namespace gil = boost::gil;

template <typename Rasterizer>
void test_apply_rasterizers(Rasterizer const& rasterizer)
{
    gil::rgb8_image_t image(200, 200);
    gil::rgb8_pixel_t pixel{255, 0, 0};

    apply_rasterizer(view(image), rasterizer, pixel);
}

int main()
{
    test_apply_rasterizers(gil::midpoint_circle_rasterizer{{50, 50}, 30});
    test_apply_rasterizers(gil::trigonometric_circle_rasterizer{{50, 50}, 30});
    test_apply_rasterizers(gil::midpoint_ellipse_rasterizer{{50, 50}, {30, 20}});
    test_apply_rasterizers(gil::bresenham_line_rasterizer{{50, 50}, {30, 20}});

    return boost::report_errors();
}
