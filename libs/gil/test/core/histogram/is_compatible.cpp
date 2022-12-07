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
namespace mp11 = boost::mp11;


void check_is_pixel_compatible()
{
    gil::histogram<int> h1;
    gil::histogram<int, double> h2;
    gil::histogram<int, unsigned char> h3;
    gil::histogram<int, std::string> h4;

    BOOST_TEST(h1.is_pixel_compatible());
    BOOST_TEST(h2.is_pixel_compatible());
    BOOST_TEST(h3.is_pixel_compatible());
    BOOST_TEST(!h4.is_pixel_compatible());
}

void check_is_tuple_compatible()
{
    gil::histogram<int> h1;
    gil::histogram<double> h2;
    gil::histogram<int, double> h3;
    gil::histogram<int, unsigned char> h4;
    gil::histogram<int, std::string> h5;

    std::tuple<int> t1;
    std::tuple<float> t2;
    std::tuple<int, double> t3;
    std::tuple<int, unsigned char> t4;
    std::tuple<int, std::string> t5;

    BOOST_TEST(h1.is_tuple_compatible(t1));
    BOOST_TEST(h1.is_tuple_compatible(t2));
    BOOST_TEST(!h1.is_tuple_compatible(t3));
    BOOST_TEST(!h1.is_tuple_compatible(t5));

    BOOST_TEST(h2.is_tuple_compatible(t1));
    BOOST_TEST(h2.is_tuple_compatible(t2));
    BOOST_TEST(!h2.is_tuple_compatible(t3));

    BOOST_TEST(!h3.is_tuple_compatible(t1));
    BOOST_TEST(h3.is_tuple_compatible(t3));
    BOOST_TEST(h3.is_tuple_compatible(t4));
    BOOST_TEST(!h3.is_tuple_compatible(t5));

    BOOST_TEST(!h4.is_tuple_compatible(t1));
    BOOST_TEST(h4.is_tuple_compatible(t3));
    BOOST_TEST(h4.is_tuple_compatible(t4));
    BOOST_TEST(!h4.is_tuple_compatible(t5));
}

int main() {

    check_is_pixel_compatible();
    check_is_tuple_compatible();

    return boost::report_errors();
}
