//
// Copyright 2020 Olzhas Zhumabek <anonymous.from.applecity@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil/algorithm.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <boost/gil/image.hpp>
#include "boost/gil/extension/image_processing/diffusion.hpp"
#include <boost/gil/image_view.hpp>
#include <boost/gil/io/typedefs.hpp>
#include <boost/gil/typedefs.hpp>

#include <cmath>
#include <iostream>
#include <string>
#include <type_traits>

namespace gil = boost::gil;

// Demonstrates Anisotropic Diffusion

// This example uses the Perona-Malik's diffusion algorithm, which is the default in GIL.
// In addition, Gaussian conductivity and two wide range conductivity functions are also available.
// see include/boost/gil/image_processing/diffusion.hpp

void gray_version(std::string const& input_path, std::string const& output_path,
                  unsigned int iteration_count, float kappa)
{
    gil::gray8_image_t input;
    gil::read_image(input_path, input, gil::png_tag{});
    auto input_view = gil::view(input);

    gil::gray32f_image_t gray(input.dimensions());
    auto gray_view = gil::view(gray);

    gil::transform_pixels(input_view, gray_view, [](const gil::gray8_pixel_t& p) { return p[0]; });
    double sum_before = 0;
    gil::for_each_pixel(gray_view, [&sum_before](gil::gray32f_pixel_t p) { sum_before += p[0]; });
    gil::gray32f_image_t output(gray.dimensions());
    auto output_view = gil::view(output);

    // gil::anisotropic_diffusion(gray_view, output_view, iteration_count, {kappa, delta_t});
    gil::default_anisotropic_diffusion(gray_view, output_view, iteration_count, kappa);
    double sum_after = 0;
    gil::for_each_pixel(output_view, [&sum_after](gil::gray32f_pixel_t p) { sum_after += p[0]; });

    gil::gray8_image_t true_output(output.dimensions());
    gil::transform_pixels(output_view, gil::view(true_output),
                          [](gil::gray32f_pixel_t p) { return static_cast<gil::uint8_t>(p[0]); });

    gil::write_view(output_path, gil::view(true_output), gil::png_tag{});

    std::cout << "sum of intensity before diffusion: " << sum_before << '\n'
              << "sum of intensity after diffusion: " << sum_after << '\n'
              << "difference: " << sum_after - sum_before << '\n';
}

void rgb_version(const std::string& input_path, const std::string& output_path,
                 unsigned int iteration_count, float kappa)
{
    gil::rgb8_image_t input;
    gil::read_image(input_path, input, gil::png_tag{});
    auto input_view = gil::view(input);

    gil::rgb32f_image_t gray(input.dimensions());
    auto gray_view = gil::view(gray);

    gil::transform_pixels(input_view, gray_view, [](const gil::rgb8_pixel_t& p) {
        return gil::rgb32f_pixel_t(p[0], p[1], p[2]);
    });
    double sum_before[3] = {};
    gil::for_each_pixel(gray_view, [&sum_before](gil::rgb32f_pixel_t p) {
        sum_before[0] += p[0];
        sum_before[1] += p[1];
        sum_before[2] += p[2];
    });
    gil::rgb32f_image_t output(gray.dimensions());
    auto output_view = gil::view(output);

    // gil::anisotropic_diffusion(gray_view, output_view, iteration_count, {kappa, delta_t});
    gil::default_anisotropic_diffusion(gray_view, output_view, iteration_count, kappa);
    double sum_after[3] = {};
    gil::for_each_pixel(output_view, [&sum_after](gil::rgb32f_pixel_t p) {
        sum_after[0] += p[0];
        sum_after[1] += p[1];
        sum_after[2] += p[2];
    });

    gil::rgb8_image_t true_output(output.dimensions());
    gil::transform_pixels(output_view, gil::view(true_output), [](gil::rgb32f_pixel_t p) {
        return gil::rgb8_pixel_t(static_cast<gil::uint8_t>(p[0]), static_cast<gil::uint8_t>(p[1]),
                                 static_cast<gil::uint8_t>(p[2]));
    });

    gil::write_view(output_path, gil::view(true_output), gil::png_tag{});
    std::cout << "sum of intensity before diffusion: (" << sum_before[0] << ", " << sum_before[1]
              << ", " << sum_before[2] << ")\n"
              << "sum of intensity after diffusion: (" << sum_after[0] << ", " << sum_after[1]
              << ", " << sum_after[2] << ")\n"
              << "difference: (" << sum_after[0] - sum_before[0] << ", "
              << sum_after[1] - sum_before[1] << ", " << sum_after[2] - sum_before[2] << ")\n";
}

int main(int argc, char* argv[])
{
    if (argc != 6)
    {
        std::cerr << "usage: " << argv[0]
                  << " <input.png> <output.png>"
                     " <colorspace: gray|rgb> <positive iteration count> <positive kappa~30>\n";
        return -1;
    }
    std::string input_path = argv[1];
    std::string output_path = argv[2];
    std::string colorspace = argv[3];

    unsigned int iteration_count = static_cast<unsigned int>(std::stoul(argv[4]));
    float kappa = std::stof(argv[5]);
    if (colorspace == "gray")
    {
        gray_version(input_path, output_path, iteration_count, kappa);
    }
    else if (colorspace == "rgb")
    {
        rgb_version(input_path, output_path, iteration_count, kappa);
    }
    else
    {
        std::cerr << "unknown colorspace option passed (did you type gray with A?)\n";
    }
}
