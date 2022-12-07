//
// // Copyright 2021 Olzhas Zhumabek <anonymous.from.applecity@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include "boost/gil/image_view_factory.hpp"
#include <boost/gil/image.hpp>
#include <boost/gil/image_view.hpp>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace boost { namespace gil {
template <typename View>
void hstack(const std::vector<View>& views, const View& output_view)
{
    if (views.size() == 0)
    {
        throw std::invalid_argument("empty views vector is passed - cannot create stacked image");
    }

    auto height = views.front().height();
    for (const auto& view : views)
    {
        if (view.height() != height)
        {
            throw std::invalid_argument("one or many views are not of the same height");
        }
    }

    std::ptrdiff_t full_width =
        std::accumulate(views.begin(), views.end(), 0,
                        [](std::ptrdiff_t old, const View& view) { return old + view.width(); });
    if (output_view.width() != full_width || output_view.height() != height)
    {
        throw std::invalid_argument("the destination view is not of the right dimensions");
    }

    std::ptrdiff_t current_x = 0;
    for (std::size_t i = 0; i < views.size(); ++i)
    {
        auto subview =
            subimage_view(output_view, current_x, 0, views[i].width(), views[i].height());
        copy_pixels(views[i], subview);
        current_x += views[i].width();
    }
}

template <typename View>
image<typename View::value_type> hstack(const std::vector<View>& views)
{
    if (views.size() == 0)
    {
        throw std::invalid_argument("empty views vector is passed - cannot create stacked image");
    }

    std::ptrdiff_t full_width =
        std::accumulate(views.begin(), views.end(), 0,
                        [](std::ptrdiff_t old, const View& view) { return old + view.width(); });
    std::ptrdiff_t height = views.front().height();
    image<typename View::value_type> result_image(full_width, height);
    hstack(views, view(result_image));
    return result_image;
}

template <typename View>
void vstack(const std::vector<View>& views, const View& output_view)
{
    if (views.size() == 0)
    {
        throw std::invalid_argument("empty views vector is passed - cannot create stacked image");
    }

    auto full_height =
        std::accumulate(views.begin(), views.end(), 0,
                        [](std::ptrdiff_t old, const View& view) { return old + view.height(); });
    std::ptrdiff_t width = views.front().height();

    for (const auto& view : views)
    {
        if (view.width() != width)
        {
            throw std::invalid_argument("one or many views are not of the same width");
        }
    }

    if (output_view != full_height || output_view.width() != width)
    {
        throw std::invalid_argument("the destination view is not of the right dimensions");
    }

    std::ptrdiff_t current_y = 0;
    for (std::size_t i = 0; i < views.size(); ++i)
    {
        auto subview =
            subimage_view(output_view, 0, current_y, views[i].width(), views[i].height());
        copy_pixels(views[i], subview);
        current_y += views[i].height();
    }
}

template <typename View>
image<typename View::value_type> vstack(const std::vector<View>& views)
{
    if (views.size() == 0)
    {
        throw std::invalid_argument("empty views vector is passed - cannot create stacked image");
    }

    auto full_height =
        std::accumulate(views.begin(), views.end(), 0,
                        [](std::ptrdiff_t old, const View& view) { return old + view.height(); });
    std::ptrdiff_t width = views.front().height();

    image<typename View::value_type> result_image(width, full_height);
    hstack(views, view(result_image));
    return result_image;
}
}} // namespace boost::gil
