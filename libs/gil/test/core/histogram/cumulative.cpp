//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/gil/histogram.hpp>

#include <boost/core/lightweight_test.hpp>

namespace gil = boost::gil;

void check_cumulative()
{
    gil::histogram<int> h1;
    for (int i = 0; i < 8; i++)
    {
        h1(i) = 1;
    }
    auto h2 = cumulative_histogram(h1);
    bool check1 = true;
    for (int i = 0; i < 8; i++)
    {
        if(h2(i) != i+1)
            check1 = false;
    }
    BOOST_TEST(check1);

    gil::histogram<int , int> h3;
    h3(1, 3) = 1;
    h3(1, 4) = 2;
    h3(2, 1) = 3;
    h3(2, 2) = 1;
    h3(2, 5) = 2;
    h3(3, 2) = 3;
    h3(3, 9) = 1;
    auto h4 = cumulative_histogram(h3);
    BOOST_TEST(h4(1, 3) == 1);
    BOOST_TEST(h4(1, 4) == 3);
    BOOST_TEST(h4(2, 1) == 3);
    BOOST_TEST(h4(2, 2) == 4);
    BOOST_TEST(h4(2, 5) == 9);
    BOOST_TEST(h4(3, 2) == 7);
    BOOST_TEST(h4(3, 9) == 13);
}

int main() {

    check_cumulative();

    return boost::report_errors();
}
