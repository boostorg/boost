//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/gil/histogram.hpp>

#include <boost/core/lightweight_test.hpp>

#include <string>

namespace gil = boost::gil;

void check_sub_histogram_without_tuple()
{
    gil::histogram<int, int, std::string, int> h;
    h(1, 1, "A", 1) = 1;
    h(1, 2, "B", 1) = 1;
    h(2, 1, "C", 1) = 1;
    h(2, 1, "D", 1) = 1;
    h(2, 3, "E", 4) = 1;
    auto h1 = h.sub_histogram<0,3>();
    BOOST_TEST(h1(1, 1) == 2);
    BOOST_TEST(h1(2, 1) == 2);
    BOOST_TEST(h1(2, 4) == 1);
    BOOST_TEST(h1(5, 5) == 0);
}

void check_sub_histogram_with_tuple()
{
    gil::histogram<int, int, std::string, int> h;
    h(1, 1, "A", 1) = 3;
    h(1, 2, "C", 1) = 1;
    h(2, 1, "C", 1) = 1;
    h(2, 1, "A", 1) = 1;
    h(2, 3, "E", 4) = 1;
    h(1, 3, "A", 1) = 2;
    std::tuple<double, int, std::string, int> t(1.0, 1000, "A", 1);
    // This means 1st dimension is useless for matching.
    auto h1 = h.sub_histogram<0,2,3>(t, t);
    BOOST_TEST(h1(1, 1, "A", 1) == 3);
    BOOST_TEST(h1(1, 2, "C", 1) == 0);
    BOOST_TEST(h1(2, 1, "C", 1) == 0);
    BOOST_TEST(h1(2, 1, "A", 1) == 0);
    BOOST_TEST(h1(2, 1, "A", 1) == 0);
    BOOST_TEST(h1(1, 3, "A", 1) == 2);
}

int main() {

    check_sub_histogram_without_tuple();
    check_sub_histogram_with_tuple();

    return boost::report_errors();
}
