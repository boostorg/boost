//
// Copyright 2020 Olzhas Zhumabek <anonymous.from.applecity@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
#include <core/test_fixture.hpp>
#include <boost/gil/algorithm.hpp>
#include <boost/gil/color_base_algorithm.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/extension/image_processing/diffusion.hpp>
#include <boost/gil/image_view.hpp>
#include <boost/gil/pixel.hpp>
#include <boost/gil/typedefs.hpp>

#include <boost/core/lightweight_test.hpp>

#include <cmath>
#include <limits>
#include <random>

namespace gil = boost::gil;

template <typename PixelType, typename DiffusionFunction>
void diffusion_function_check(DiffusionFunction diffusion)
{
    using limits = std::numeric_limits<gil::uint8_t>;
    using channel_type = typename gil::channel_type<PixelType>::type;
    for (channel_type value = limits::min(); value <= limits::max(); ++value)
    {
        PixelType pixel;
        gil::static_fill(pixel, value);
        auto diffusivity_value = diffusion(pixel);
        gil::static_for_each(diffusivity_value, [](channel_type channel_value) {
            BOOST_TEST(0 <= channel_value && channel_value <= 1.0);
        });
    }
}

void brightness_function_test()
{
    std::vector<gil::rgb32f_pixel_t> rgb_pixels{
        gil::rgb32f_pixel_t(0, 11, 14),     gil::rgb32f_pixel_t(2, 117, 200),
        gil::rgb32f_pixel_t(223, 2, 180),   gil::rgb32f_pixel_t(250, 254, 100),
        gil::rgb32f_pixel_t(255, 255, 255),
    };
    for (const auto& pixel : rgb_pixels)
    {
        boost::gil::laplace_function::stencil_type<gil::rgb32f_pixel_t> stencil;
        std::fill(stencil.begin(), stencil.end(), pixel);
        auto brightness_stencil = boost::gil::brightness_function::identity{}(stencil);
        for (const auto& brightness_pixel : brightness_stencil)
        {
            BOOST_TEST(pixel == brightness_pixel);
        }
    }

    std::vector<float> corresponding_luminance_values{8.878f, 98.5436f, 61.8362f, 242.0308f, 255};
    std::size_t index = 0;
    for (const auto& pixel : rgb_pixels)
    {
        boost::gil::laplace_function::stencil_type<gil::rgb32f_pixel_t> stencil;
        std::fill(stencil.begin(), stencil.end(), pixel);
        auto brightness_stencil = boost::gil::brightness_function::rgb_luminance{}(stencil);
        for (const auto& brightness_pixel : brightness_stencil)
        {
            gil::static_for_each(brightness_pixel, [&corresponding_luminance_values,
                                                    index](boost::gil::float32_t value) {
                BOOST_TEST(std::abs(value - corresponding_luminance_values[index]) < 1.0f);
            });
        }
        ++index;
    }

    std::vector<gil::gray32f_pixel_t> gray_pixels{
        gil::gray32f_pixel_t(11),  gil::gray32f_pixel_t(0),  gil::gray32f_pixel_t(255),
        gil::gray32f_pixel_t(123), gil::gray32f_pixel_t(17),
    };
    for (const auto& pixel : gray_pixels)
    {
        boost::gil::laplace_function::stencil_type<gil::gray32f_pixel_t> stencil;
        std::fill(stencil.begin(), stencil.end(), pixel);
        auto brightness_stencil = boost::gil::brightness_function::identity{}(stencil);
        for (const auto& brightness_pixel : brightness_stencil)
        {
            BOOST_TEST(pixel == brightness_pixel);
        }
    }
}

template <typename ImageType, typename OutputImageType>
void heat_conservation_test(std::uint32_t seed)
{
    gil::test::fixture::random_value<std::uint32_t> dist(seed, 0,
                                                         std::numeric_limits<gil::uint8_t>::max());

    ImageType image(32, 32);
    auto view = gil::view(image);
    constexpr std::ptrdiff_t num_channels = gil::num_channels<decltype(view)>::value;
    double before_diffusion[num_channels] = {0};
    for (auto& pixel : view)
    {
        for (std::size_t channel_index = 0; channel_index < num_channels; ++channel_index)
        {
            pixel[channel_index] = static_cast<gil::uint8_t>(dist());
            before_diffusion[channel_index] += pixel[channel_index];
        }
    }

    OutputImageType output(32, 32);
    auto output_view = gil::view(output);
    gil::default_anisotropic_diffusion(view, output_view, 10, 5);
    double after_diffusion[num_channels] = {0};
    for (const auto& pixel : output_view)
    {
        for (std::size_t channel_index = 0; channel_index < num_channels; ++channel_index)
        {
            after_diffusion[channel_index] += pixel[channel_index];
        }
    }

    for (std::ptrdiff_t channel_index = 0; channel_index < num_channels; ++channel_index)
    {
        const auto percentage =
            before_diffusion[channel_index] != 0
                ? std::abs(after_diffusion[channel_index] - before_diffusion[channel_index]) /
                      after_diffusion[channel_index] * 100.0
                : std::abs(after_diffusion[channel_index] - before_diffusion[channel_index]) /
                      after_diffusion[channel_index] * 100.0;
#ifdef BOOST_GIL_TEST_DEBUG
        std::cout << percentage << ' ';
#endif
        BOOST_TEST(percentage < 1);
    }
#ifdef BOOST_GIL_TEST_DEBUG
    std::cout << '\n';
#endif
}

template <typename PixelType>
void test_stencil_pixels(const gil::laplace_function::stencil_type<PixelType>& stencil,
                         const PixelType& reference)
{
    for (const auto& stencil_entry : stencil)
    {
        BOOST_TEST(stencil_entry == reference);
    }
}

template <typename PixelType>
void test_stencil(const gil::laplace_function::stencil_type<PixelType>& stencil,
                  const std::array<float, 8>& expected_values)
{
    for (std::size_t i = 0; i < 8; ++i)
    {
        PixelType expected_pixel;
        gil::static_fill(expected_pixel, expected_values[i]);
        BOOST_TEST(stencil[i] == expected_pixel);
    }
}

void laplace_functions_test()
{
    // sanity checks
    auto zero_rgb_pixel = []() {
        gil::rgb32f_pixel_t zero_pixel;
        gil::static_fill(zero_pixel, 0);
        return zero_pixel;
    }();
    auto zero_gray_pixel = []() {
        gil::gray32f_pixel_t zero_pixel;
        gil::static_fill(zero_pixel, 0);
        return zero_pixel;
    }();

    auto image_size = gil::point_t(16, 16);
    gil::rgb32f_image_t rgb_image(image_size, zero_rgb_pixel);
    gil::gray32f_image_t gray_image(image_size, zero_gray_pixel);
    auto rgb = gil::view(rgb_image);
    auto gray = gil::view(gray_image);

    auto s4way = gil::laplace_function::stencil_5points{};
    auto s8way = gil::laplace_function::stencil_9points_standard{};
    for (std::ptrdiff_t y = 1; y < image_size.y - 1; ++y)
    {
        for (std::ptrdiff_t x = 1; x < image_size.x - 1; ++x)
        {
            auto rgb_4way_stencil = s4way.compute_laplace(rgb, {x, y});
            auto gray_4way_stencil = s4way.compute_laplace(gray, {x, y});

            auto rgb_8way_stencil = s8way.compute_laplace(rgb, {x, y});
            auto gray_8way_stencil = s8way.compute_laplace(gray, {x, y});

            test_stencil_pixels(rgb_4way_stencil, zero_rgb_pixel);
            test_stencil_pixels(rgb_8way_stencil, zero_rgb_pixel);
            test_stencil_pixels(gray_4way_stencil, zero_gray_pixel);
            test_stencil_pixels(gray_8way_stencil, zero_gray_pixel);
        }
    }

    // a predefined case
    // 5  11 2
    // 17 25 58
    // 90 31 40

    using rgb_pixel = gil::rgb32f_pixel_t;
    rgb(1, 1) = rgb_pixel(5, 5, 5);
    rgb(2, 1) = rgb_pixel(11, 11, 11);
    rgb(3, 1) = rgb_pixel(2, 2, 2);
    rgb(1, 2) = rgb_pixel(17, 17, 17);
    rgb(2, 2) = rgb_pixel(25, 25, 25);
    rgb(3, 2) = rgb_pixel(58, 58, 58);
    rgb(1, 3) = rgb_pixel(90, 90, 90);
    rgb(2, 3) = rgb_pixel(31, 31, 31);
    rgb(3, 3) = rgb_pixel(40, 40, 40);

    using gray_pixel = gil::gray32f_pixel_t;
    gray(1, 1) = gray_pixel(5);
    gray(2, 1) = gray_pixel(11);
    gray(3, 1) = gray_pixel(2);
    gray(1, 2) = gray_pixel(17);
    gray(2, 2) = gray_pixel(25);
    gray(3, 2) = gray_pixel(58);
    gray(1, 3) = gray_pixel(90);
    gray(2, 3) = gray_pixel(31);
    gray(3, 3) = gray_pixel(40);

    // 4 way stencil should be
    // 0 -14 0
    // -8  <not computed for center> 33
    // 0 6 0

    auto test_point = gil::point_t(2, 2);
    std::array<float, 8> _4way_expected{0, -14, 0, 33, 0, 6, 0, -8};
    auto rgb_4way = s4way.compute_laplace(rgb, test_point);
    test_stencil(rgb_4way, _4way_expected);
    auto gray_4way = s4way.compute_laplace(gray, test_point);
    test_stencil(gray_4way, _4way_expected);

    // 8 way stencil should be
    // -20 -14 -23
    // -8 <not computed for center> 33
    // 65 6 15

    std::array<float, 8> _8way_expected{-20, -14, -23, 33, 15, 6, 65, -8};
    auto rgb_8way = s8way.compute_laplace(rgb, test_point);
    test_stencil(rgb_8way, _8way_expected);
    auto gray_8way = s8way.compute_laplace(gray, test_point);
    test_stencil(gray_8way, _8way_expected);

    // reduce result for 4 way should be (-14 - 8 + 6 + 33) * 0.25 = 4.25
    auto rgb_reduced_4way = s4way.reduce(rgb_4way);
    gil::static_for_each(rgb_reduced_4way,
                         [](gil::float32_t value) { BOOST_TEST(value == 4.25f); });
    auto gray_reduced_4way = s4way.reduce(gray_4way);
    gil::static_for_each(gray_reduced_4way,
                         [](gil::float32_t value) { BOOST_TEST(value == 4.25f); });

    // reduce result for 8 way should be ((-20-23+15+65)*0.5+(-14+33+6-8)) * 0.125 = (18.5+17) *
    // 0.125 = 4.4375
    auto rgb_reduced_8way = s8way.reduce(rgb_8way);
    gil::static_for_each(rgb_reduced_8way,
                         [](gil::float32_t value) { BOOST_TEST(value == 4.4375); });
    auto gray_reduced_8way = s8way.reduce(gray_8way);
    gil::static_for_each(gray_reduced_8way,
                         [](gil::float32_t value) { BOOST_TEST(value == 4.4375); });
}

int main()
{
    for (std::uint32_t seed = 0; seed < 100; ++seed)
    {
        heat_conservation_test<gil::gray8_image_t, gil::gray32f_image_t>(seed);
        heat_conservation_test<gil::rgb8_image_t, gil::rgb32f_image_t>(seed);
    }

    for (double kappa = 5; kappa <= 70; ++kappa)
    {
        diffusion_function_check<gil::rgb32f_pixel_t>(
            gil::conductivity::perona_malik_conductivity{kappa});
        diffusion_function_check<gil::rgb32f_pixel_t>(
            gil::conductivity::gaussian_conductivity{kappa});
        diffusion_function_check<gil::rgb32f_pixel_t>(
            gil::conductivity::wide_regions_conductivity{kappa});
        diffusion_function_check<gil::rgb32f_pixel_t>(
            gil::conductivity::more_wide_regions_conductivity{kappa});

        diffusion_function_check<gil::gray32f_pixel_t>(
            gil::conductivity::perona_malik_conductivity{kappa});
        diffusion_function_check<gil::gray32f_pixel_t>(
            gil::conductivity::gaussian_conductivity{kappa});
        diffusion_function_check<gil::gray32f_pixel_t>(
            gil::conductivity::wide_regions_conductivity{kappa});
        diffusion_function_check<gil::gray32f_pixel_t>(
            gil::conductivity::more_wide_regions_conductivity{kappa});
    }

    brightness_function_test();

    laplace_functions_test();

    return boost::report_errors();
}
