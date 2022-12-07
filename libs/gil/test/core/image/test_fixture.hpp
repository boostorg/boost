//
// Copyright 2019 Mateusz Loskot <mateusz at loskot dot net>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//
#ifndef BOOST_GIL_TEST_CORE_IMAGE_TEST_FIXTURE_HPP
#define BOOST_GIL_TEST_CORE_IMAGE_TEST_FIXTURE_HPP

#include <boost/gil.hpp>

#include <cstdint>
#include <initializer_list>
#include <limits>
#include <random>
#include <tuple>
#include <type_traits>

namespace boost { namespace gil { namespace test { namespace fixture {

using image_types = std::tuple
<
    gil::gray8_image_t,
    gil::gray16_image_t,
    gil::gray32_image_t,
    gil::bgr8_image_t,
    gil::bgr16_image_t,
    gil::bgr32_image_t,
    gil::rgb8_image_t,
    gil::rgb16_image_t,
    gil::rgb32_image_t,
    gil::rgba8_image_t,
    gil::rgba16_image_t,
    gil::rgba32_image_t
>;

#if defined(BOOST_NO_CXX17_HDR_MEMORY_RESOURCE)
    using pmr_image_types = std::tuple<>;
#else
    using pmr_image_types = std::tuple
    <
        gil::pmr::gray8_image_t,
        gil::pmr::gray16_image_t,
        gil::pmr::gray32_image_t,
        gil::pmr::bgr8_image_t,
        gil::pmr::bgr16_image_t,
        gil::pmr::bgr32_image_t,
        gil::pmr::rgb8_image_t,
        gil::pmr::rgb16_image_t,
        gil::pmr::rgb32_image_t,
        gil::pmr::rgba8_image_t,
        gil::pmr::rgba16_image_t,
        gil::pmr::rgba32_image_t
    >;
#endif //defined(BOOST_NO_CXX17_HDR_MEMORY_RESOURCE)

using rgb_interleaved_image_types = std::tuple
<
    gil::bgr8_image_t,
    gil::bgr16_image_t,
    gil::bgr32_image_t,
    gil::rgb8_image_t,
    gil::rgb16_image_t,
    gil::rgb32_image_t,
    gil::rgba8_image_t,
    gil::rgba16_image_t,
    gil::rgba32_image_t
>;

using bit_aligned_image_types = std::tuple
<
    gil::bit_aligned_image1_type<1, gil::gray_layout_t>::type,
    gil::bit_aligned_image1_type<3, gil::gray_layout_t>::type,
    gil::bit_aligned_image1_type<8, gil::gray_layout_t>::type,
    gil::bit_aligned_image3_type<2, 2, 2, gil::rgb_layout_t>::type,
    gil::bit_aligned_image3_type<5, 6, 5, gil::rgb_layout_t>::type,
    gil::bit_aligned_image3_type<6, 6, 6, gil::rgb_layout_t>::type
>;

template <typename Image, typename Generator>
auto generate_image(std::ptrdiff_t size_x, std::ptrdiff_t size_y, Generator&& generate) -> Image
{
    using pixel_t = typename Image::value_type;

    Image out(size_x, size_y);
    gil::for_each_pixel(view(out), [&generate](pixel_t& p) {
        gil::static_generate(p, [&generate]() { return generate(); });
    });

    return out;
}

template <typename Image>
auto create_image(std::ptrdiff_t size_x, std::ptrdiff_t size_y, int channel_value) -> Image
{
    using pixel_t = typename Image::value_type;
    using channel_t = typename gil::channel_type<pixel_t>::type;
    static_assert(std::is_integral<channel_t>::value, "channel must be integral type");

    Image out(size_x, size_y);
    gil::for_each_pixel(view(out), [&channel_value](pixel_t& p) {
        gil::static_fill(p, static_cast<channel_t>(channel_value));
    });

    return out;
}

}}}} // namespace boost::gil::test::fixture

#endif
