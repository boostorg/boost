//
// Copyright 2021 Prathamesh Tagore <prathameshtagore@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/core/lightweight_test.hpp>
#include <boost/gil/image_processing/morphology.hpp>
#include <vector>

namespace gil = boost::gil;

// This function helps us fill pixels of a view given as 2nd argument with
// elements of the vector given as 1st argument.
void pixel_fill(std::vector<std::vector<int>>& original_binary_vector,
                boost::gil::gray8_image_t& original_img)
{
    for (std::ptrdiff_t view_row = 0; view_row < view(original_img).height(); ++view_row)
    {
        for (std::ptrdiff_t view_col = 0; view_col < view(original_img).width(); ++view_col)
        {
            view(original_img)(view_col, view_row) =
                gil::gray8_pixel_t(original_binary_vector[view_row][view_col]);
        }
    }
}

int main()
{
    std::vector<std::vector<int>> original_binary_vector{
        {0, 0, 0, 0, 0, 0},         {0, 0, 127, 144, 143, 0}, {0, 0, 128, 0, 142, 0},
        {0, 0, 129, 0, 141, 0},     {0, 0, 130, 140, 139, 0}, {0, 0, 131, 0, 0, 0},
        {0, 0, 132, 137, 136, 138}, {0, 0, 133, 134, 135, 0}};
    std::vector<std::vector<int>> orig_dil_imp{
        {255, 100, 100, 100}, {100, 100, 100, 100}, {100, 100, 100, 100}};
    // All vectors defined below will be used for creating expected image views
    // which are supposed to match the views obtained after applying morphological
    // operations.
    std::vector<std::vector<int>> exp_dil{
        {0, 127, 144, 144, 144, 143}, {0, 128, 144, 144, 144, 143}, {0, 129, 144, 144, 144, 143},
        {0, 130, 140, 142, 142, 142}, {0, 131, 140, 141, 141, 141}, {0, 132, 140, 140, 140, 139},
        {0, 133, 137, 137, 138, 138}, {0, 133, 137, 137, 138, 138}};
    // Following vector intends to check result of dilation operation when it is
    // applied 2 times on the original image.
    std::vector<std::vector<int>> exp_dil_iter2{
        {128, 144, 144, 144, 144, 144}, {129, 144, 144, 144, 144, 144},
        {130, 144, 144, 144, 144, 144}, {131, 144, 144, 144, 144, 144},
        {132, 140, 142, 142, 142, 142}, {133, 140, 141, 141, 141, 141},
        {133, 140, 140, 140, 140, 140}, {133, 137, 137, 138, 138, 138}};
    std::vector<std::vector<int>> exp_er{
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 132, 0, 0}};
    // Following vector intends to check result of erosion operation when it is
    // applied 2 times on the original image.
    std::vector<std::vector<int>> exp_er_iter2{
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};
    std::vector<std::vector<int>> exp_opening{
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0},       {0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 132, 132, 132, 0}, {0, 0, 132, 132, 132, 0}};
    std::vector<std::vector<int>> exp_closing{
        {0, 0, 127, 144, 143, 143}, {0, 0, 127, 144, 143, 143}, {0, 0, 128, 140, 142, 142},
        {0, 0, 129, 140, 141, 141}, {0, 0, 130, 140, 139, 139}, {0, 0, 131, 137, 137, 138},
        {0, 0, 132, 137, 137, 138}, {0, 0, 133, 137, 137, 138}};
    std::vector<std::vector<int>> exp_mg{{0, 127, 144, 144, 144, 143}, {0, 128, 144, 144, 144, 143},
                                         {0, 129, 144, 144, 144, 143}, {0, 130, 140, 142, 142, 142},
                                         {0, 131, 140, 141, 141, 141}, {0, 132, 140, 140, 140, 139},
                                         {0, 133, 137, 137, 138, 138}, {0, 133, 137, 5, 138, 138}};
    std::vector<std::vector<int>> exp_top_hat{{0, 0, 0, 0, 0, 0},       {0, 0, 127, 144, 143, 0},
                                              {0, 0, 128, 0, 142, 0},   {0, 0, 129, 0, 141, 0},
                                              {0, 0, 130, 140, 139, 0}, {0, 0, 131, 0, 0, 0},
                                              {0, 0, 0, 5, 4, 138},     {0, 0, 1, 2, 3, 0}};
    std::vector<std::vector<int>> exp_black_hat{
        {0, 0, 127, 144, 143, 143}, {0, 0, 0, 0, 0, 143}, {0, 0, 0, 140, 0, 142},
        {0, 0, 0, 140, 0, 141},     {0, 0, 0, 0, 0, 139}, {0, 0, 0, 137, 137, 138},
        {0, 0, 0, 0, 1, 0},         {0, 0, 0, 3, 2, 138}};
    std::vector<std::vector<int>> exp_dil_imp{
        {255, 255, 100, 100}, {255, 255, 100, 100}, {100, 100, 100, 100}};

    gil::gray8_image_t original_img(6, 8), obtained_dilation(6, 8), expected_dilation(6, 8);
    gil::gray8_image_t obtained_erosion(6, 8), expected_erosion(6, 8);
    gil::gray8_image_t obtained_opening(6, 8), expected_opening(6, 8);
    gil::gray8_image_t obtained_closing(6, 8), expected_closing(6, 8);
    gil::gray8_image_t obtained_mg(6, 8), expected_mg(6, 8);
    gil::gray8_image_t obtained_top_hat(6, 8), expected_top_hat(6, 8);
    gil::gray8_image_t obtained_black_hat(6, 8), expected_black_hat(6, 8);
    gil::gray8_image_t obtained_dil_iter2(6, 8), expected_dil_iter2(6, 8);
    gil::gray8_image_t obtained_er_iter2(6, 8), expected_er_iter2(6, 8);
    gil::gray8_image_t obtained_imp_dil(4, 3), expected_imp_dil(4, 3), original_imp_dil(4, 3);

    std::vector<float> ker_vec(9, 1.0f); // Structuring element
    gil::detail::kernel_2d<float> ker_mat(ker_vec.begin(), ker_vec.size(), 1, 1);

    pixel_fill(original_binary_vector, original_img);
    pixel_fill(exp_dil, expected_dilation);
    pixel_fill(exp_er, expected_erosion);
    pixel_fill(exp_opening, expected_opening);
    pixel_fill(exp_closing, expected_closing);
    pixel_fill(exp_mg, expected_mg);
    pixel_fill(exp_top_hat, expected_top_hat);
    pixel_fill(exp_black_hat, expected_black_hat);
    pixel_fill(exp_dil_iter2, expected_dil_iter2);
    pixel_fill(orig_dil_imp, original_imp_dil);
    pixel_fill(exp_dil_imp, expected_imp_dil);
    pixel_fill(exp_er_iter2, expected_er_iter2);

    // Different morphological operations are applied on the same initial image to
    // obtain results of our implementation which are then compared with expected
    // results.
    gil::dilate(view(original_img), view(obtained_dilation), ker_mat, 1);
    gil::erode(view(original_img), view(obtained_erosion), ker_mat, 1);
    gil::opening(view(original_img), view(obtained_opening), ker_mat);
    gil::closing(view(original_img), view(obtained_closing), ker_mat);
    gil::morphological_gradient(view(original_img), view(obtained_mg), ker_mat);
    gil::top_hat(view(original_img), view(obtained_top_hat), ker_mat);
    gil::black_hat(view(original_img), view(obtained_black_hat), ker_mat);
    gil::dilate(view(original_imp_dil), view(obtained_imp_dil), ker_mat, 1);
    gil::dilate(view(original_img), view(obtained_dil_iter2), ker_mat, 2);
    gil::erode(view(original_img), view(obtained_er_iter2), ker_mat, 2);

    // Testing obtained results with expected results.
    BOOST_TEST(gil::equal_pixels(gil::view(obtained_dilation), gil::view(expected_dilation)));
    BOOST_TEST(gil::equal_pixels(gil::view(obtained_erosion), gil::view(expected_erosion)));
    BOOST_TEST(gil::equal_pixels(gil::view(obtained_opening), gil::view(expected_opening)));
    BOOST_TEST(gil::equal_pixels(gil::view(obtained_closing), gil::view(expected_closing)));
    BOOST_TEST(gil::equal_pixels(gil::view(obtained_mg), gil::view(expected_mg)));
    BOOST_TEST(gil::equal_pixels(gil::view(obtained_top_hat), gil::view(expected_top_hat)));
    BOOST_TEST(gil::equal_pixels(gil::view(obtained_black_hat), gil::view(expected_black_hat)));
    BOOST_TEST(gil::equal_pixels(gil::view(obtained_imp_dil), gil::view(expected_imp_dil)));
    BOOST_TEST(gil::equal_pixels(gil::view(obtained_dil_iter2), gil::view(expected_dil_iter2)));
    BOOST_TEST(gil::equal_pixels(gil::view(obtained_er_iter2), gil::view(expected_er_iter2)));

    return boost::report_errors();
}
