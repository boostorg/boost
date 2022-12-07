//
// Copyright 2020 Debabrata Mandal <mandaldebabrata123@gmail.com>
//
// Distributed under the Boost Software License, Version 1.0
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <boost/gil/histogram.hpp>

#include <boost/core/lightweight_test.hpp>
#include <boost/container_hash/hash.hpp>

#include <tuple>

namespace gil = boost::gil;
namespace mp11 = boost::mp11;

void check_detail_hash_tuple ()
{
    std::tuple<int, int> t(1, 1);
    std::size_t seed1 = 0;
    boost::hash_combine(seed1, std::get<0>(t));
    boost::hash_combine(seed1, std::get<1>(t));

    gil::detail::hash_tuple<int, int> g;
    std::size_t seed2 = g(t);
    BOOST_TEST(seed1 == seed2);
}

int main()
{
    check_detail_hash_tuple();
    return boost::report_errors();
}
