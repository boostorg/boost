//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/gil/histogram.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/gil/pixel.hpp>

#include <boost/core/lightweight_test.hpp>

#include <string>

namespace gil = boost::gil;

void check_histogram_key_from_tuple()
{
    gil::histogram<int, int> h1;
    std::tuple<int, int> t1(1, 2);
    auto t2 = h1.key_from_tuple(t1);
    const bool same_type = std::is_same<std::tuple<int, int>, decltype(t2)>::value;

    BOOST_TEST(same_type);
    BOOST_TEST(std::get<0>(t2) == 1 && std::get<1>(t2) == 2);

    std::tuple<int, int, int, int> t3(1, 2, 4, 2);
    auto t4 = h1.key_from_tuple<0, 2>(t3);
    const bool same_type1 = std::is_same<std::tuple<int, int>, decltype(t4)>::value;

    BOOST_TEST(same_type1);
    BOOST_TEST(std::get<0>(t4) == 1 && std::get<1>(t4) == 4);
}

void check_histogram_key_from_pixel()
{
    gil::histogram<int> h1;
    gil::gray8_pixel_t g1(1);
    auto t1 = h1.key_from_pixel(g1);
    const bool same_type = std::is_same<std::tuple<int>, decltype(t1)>::value;

    BOOST_TEST(same_type);
    BOOST_TEST(std::get<0>(t1) == 1);

    gil::histogram<int, int> h2;
    gil::rgb8_pixel_t r1(1, 0, 3);
    auto t2 = h2.key_from_pixel<0, 2>(r1);
    const bool same_type1 = std::is_same<std::tuple<int, int>, decltype(t2)>::value;

    BOOST_TEST(same_type1);
    BOOST_TEST(std::get<0>(t2) == 1 && std::get<1>(t2) == 3);
}

int main() {

    check_histogram_key_from_tuple();
    check_histogram_key_from_pixel();

    return boost::report_errors();
}
