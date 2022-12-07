//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/gil/histogram.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/image_view.hpp>
#include <boost/gil/typedefs.hpp>

#include <boost/core/lightweight_test.hpp>
#include <cmath>
#include <iostream>
#include <vector>

namespace gil = boost::gil;
namespace mp11 = boost::mp11;

std::uint8_t big_matrix[] =
{
    1, 2, 3, 4, 5, 6, 7, 8,
    1, 2, 1, 2, 1, 2, 1, 2,
    1, 2, 3, 4, 5, 6, 7, 8,
    3, 4, 3, 4, 3, 4, 3, 4,
    1, 2, 3, 4, 5, 6, 7, 8,
    5, 6, 5, 6, 5, 6, 5, 6,
    1, 2, 3, 4, 5, 6, 7, 8,
    7, 8, 7, 8, 7, 8, 7, 8
};

void check_normalize()
{
    auto epsilon = 1e-6;
    // 1D histogram
    double expected[64];
    gil::histogram<int> h1;
    int sum = 0;
    for (std::size_t i = 0; i < 64; i++)
    {
        h1(i) = big_matrix[i];
        sum += big_matrix[i];
    }
    for (std::size_t i = 0; i < 64; i++)
    {
        expected[i] = double(big_matrix[i]) / sum;
    }
    h1.normalize();

    bool check = true;
    for (std::size_t i = 0; i < 64; i++)
    {
        check = check & (std::abs(expected[i] - h1(i)) < epsilon);
    }
    BOOST_TEST(check);

    // 2D histogram
    double expected2[8][8];
    gil::histogram<int, int> h2;
    int sum2 = 0;
    for (std::size_t i = 0; i < 64; i++)
    {
        h2(i/8, i%8) = big_matrix[i];
        sum2 += big_matrix[i];
    }
    for (std::size_t i = 0; i < 64; i++)
    {
        expected2[i/8][i%8] = double(big_matrix[i]) / sum2;
    }
    h2.normalize();

    bool check2 = true;
    for (std::size_t i = 0; i < 64; i++)
    {
        check2 = check2 & (std::abs(expected2[i/8][i%8] - h2(i/8,i%8)) < epsilon);
    }
    BOOST_TEST(check2);
}

void check_nearest_key()
{
    {
        gil::histogram<int> h1;
        h1(1) = 1;
        h1(3) = 4;
        h1(4) = 4;
        h1(6) = 1;
        std::tuple<int> k1{2}, k2{3}, k3{5};
        std::tuple<int> k1_expected{1}, k2_expected{3}, k3_expected{4};
        BOOST_TEST(k1_expected == h1.nearest_key(k1));
        BOOST_TEST(k2_expected == h1.nearest_key(k2));
        BOOST_TEST(k3_expected == h1.nearest_key(k3));
    }

    {
        gil::histogram<int, int> h2;
        h2(1, 1) = 1;
        h2(1, 4) = 1;
        h2(2, 4) = 1;
        h2(4, 4) = 1;
        std::tuple<int, int> k1(1, 1), k2(1, 3), k3(2, 1), k4(2, 7), k5(4, 4);
        std::tuple<int, int> k1_exp(1, 1), k2_exp(1, 1), k3_exp(1, 4), k4_exp(2, 4), k5_exp(4, 4);
        BOOST_TEST(k1_exp == h2.nearest_key(k1));
        BOOST_TEST(k2_exp == h2.nearest_key(k2));
        BOOST_TEST(k3_exp == h2.nearest_key(k3));
        BOOST_TEST(k4_exp == h2.nearest_key(k4));
        BOOST_TEST(k5_exp == h2.nearest_key(k5));
    }

}

void check_equals()
{
    gil::histogram<unsigned char> h, h2;
    h(1) = 3;
    h(4) = 1;
    h(2) = 6;
    h(7) = 3;
    h(9) = 7;
    h2 = h;
    BOOST_TEST(h2.equals(h));

    gil::histogram<double> h3;
    h3(1) = 3;
    h3(4) = 1;
    h3(2) = 6;
    h3(7) = 3;
    h3(9) = 7;
    BOOST_TEST(h3.equals(h));
}

void check_sum()
{
    gil::histogram<unsigned char> h;
    h(1) = 3;
    h(4) = 1;
    h(2) = 6;
    h(7) = 3;
    h(9) = 7;
    auto sm = h.sum();
    BOOST_TEST(sm == 20);
}

void check_max_key()
{
    gil::histogram<unsigned char> h;
    h(1) = 3;
    h(4) = 1;
    h(2) = 6;
    h(7) = 3;
    h(9) = 7;
    BOOST_TEST(std::get<0>(h.max_key()) == 9);

    gil::histogram<unsigned char, int> h2;
    h2(1, 4) = 3;
    h2(4, 2) = 1;
    h2(2, 5) = 6;
    h2(7, 4) = 3;
    h2(9, 1) = 7;
    h2(9, 3) = 7;
    BOOST_TEST(std::get<0>(h2.max_key()) == 9 && std::get<1>(h2.max_key()) == 3);
}

void check_min_key()
{
    gil::histogram<unsigned char> h;
    h(1) = 3;
    h(4) = 1;
    h(2) = 6;
    h(7) = 3;
    h(9) = 7;
    BOOST_TEST(std::get<0>(h.min_key()) == 1);

    gil::histogram<unsigned char, int> h2;
    h2(1, 4) = 3;
    h2(4, 2) = 1;
    h2(2, 5) = 6;
    h2(7, 4) = 3;
    h2(9, 1) = 7;
    h2(9, 3) = 7;
    BOOST_TEST(std::get<0>(h2.min_key()) == 1 && std::get<1>(h2.min_key()) == 4);
}

void check_sorted_keys()
{
    gil::histogram<unsigned char> h;
    h(1) = 3;
    h(4) = 1;
    h(2) = 6;
    h(7) = 3;
    h(9) = 7;

    std::vector<std::tuple<unsigned char>> v;
    v.push_back(std::tuple<unsigned char>(1));
    v.push_back(std::tuple<unsigned char>(2));
    v.push_back(std::tuple<unsigned char>(4));
    v.push_back(std::tuple<unsigned char>(7));
    v.push_back(std::tuple<unsigned char>(9));
    BOOST_TEST(v == h.sorted_keys());
}

int main() {

    check_normalize();
    check_nearest_key();
    check_equals();
    check_max_key();
    check_min_key();
    check_sum();
    check_sorted_keys();

    return boost::report_errors();
}
