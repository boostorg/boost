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

void check_indexing_operator()
{
    gil::histogram<int> h1;
    h1(1) = 3;
    BOOST_TEST(h1(1) == 3);
    BOOST_TEST(h1(3) == 0);

    gil::histogram<int, char, std::string> h2;
    h2(1, 'a', "A") = 4;
    BOOST_TEST(h2(1, 'a', "A") == 4);
    BOOST_TEST(h2(1, 'a', "B") == 0);
}

int main() {

    check_indexing_operator();

    return boost::report_errors();
}
