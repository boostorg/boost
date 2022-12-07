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

#include <boost/mp11.hpp>
#include <boost/core/lightweight_test.hpp>

#include <string>

namespace gil = boost::gil;
namespace mp11 = boost::mp11;

void check_helper_fn_pixel_to_tuple()
{
    gil::gray8_pixel_t g1(2);
    auto g2 = gil::detail::pixel_to_tuple(g1, mp11::make_index_sequence<1>{});

    bool const same_gray_type = std::is_same<std::tuple<gil::uint8_t>, decltype(g2)>::value;
    BOOST_TEST(same_gray_type);
    BOOST_TEST(g1[0] == std::get<0>(g2));

    gil::rgb8_pixel_t r1(1,2,3);
    auto r2 = gil::detail::pixel_to_tuple(r1, mp11::index_sequence<0, 1, 2>{});

    bool const same_rgb_type = std::is_same<std::tuple<gil::uint8_t, gil::uint8_t, gil::uint8_t>,
                    decltype(r2)>::value;
    BOOST_TEST(same_rgb_type);
    BOOST_TEST(r1[0] == std::get<0>(r2) && r1[1] == std::get<1>(r2) && r1[2] == std::get<2>(r2));

    auto r3 = gil::detail::pixel_to_tuple(r1, mp11::index_sequence<1, 2, 0>{});
    BOOST_TEST(r1[0] == std::get<2>(r3) && r1[1] == std::get<0>(r3) && r1[2] == std::get<1>(r3));
}

void check_helper_fn_tuple_to_tuple()
{
    std::tuple<int> t1(1);
    auto t2 = gil::detail::tuple_to_tuple(t1, mp11::make_index_sequence<1>{});

    bool const same_gray_type = std::is_same<std::tuple<int>, decltype(t2)>::value;
    BOOST_TEST(same_gray_type);
    BOOST_TEST(std::get<0>(t1) == std::get<0>(t2));

    std::tuple<int, int, std::string> r1(1, 2, "A");
    auto r2 = gil::detail::tuple_to_tuple(r1, mp11::index_sequence<0, 1, 2>{});

    bool const same_rgb_type = std::is_same<std::tuple<int, int, std::string>,
                    decltype(r2)>::value;
    BOOST_TEST(same_rgb_type);
    BOOST_TEST( std::get<0>(r1) == std::get<0>(r2) &&
                std::get<1>(r1) == std::get<1>(r2) &&
                std::get<2>(r1) == std::get<2>(r2));

    auto r3 = gil::detail::tuple_to_tuple(r1, mp11::index_sequence<1, 2, 0>{});
    BOOST_TEST( std::get<0>(r1) == std::get<2>(r3) &&
                std::get<1>(r1) == std::get<0>(r3) &&
                std::get<2>(r1) == std::get<1>(r3));
}

void check_helper_tuple_limit()
{
    using type1 = std::tuple<int, short>;
    using type2 = std::tuple<unsigned int, unsigned char>;
    type1 t1_min(std::numeric_limits<int>::min(), std::numeric_limits<short>::min());
    type1 t1_max(std::numeric_limits<int>::max(), std::numeric_limits<short>::max());
    type2 t2_min(std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned char>::min());
    type2 t2_max(std::numeric_limits<unsigned int>::max(), std::numeric_limits<unsigned char>::max());

    BOOST_TEST(t1_min == gil::detail::tuple_limit<type1>::min());
    BOOST_TEST(t1_max == gil::detail::tuple_limit<type1>::max());
    BOOST_TEST(t2_min == gil::detail::tuple_limit<type2>::min());
    BOOST_TEST(t2_max == gil::detail::tuple_limit<type2>::max());

}

void check_filler()
{
    boost::gil::histogram<int> h;
    boost::gil::detail::filler<1> f;
    std::tuple<int> l1{4}, h1{13};
    f(h, l1, h1);

    std::tuple<int> l2{20}, h2{33};
    f(h, l2, h2);

    bool check = true;
    for(int i = 0; i < 100; i++)
    {
        if((i >= 4 && i <= 13) || (i >= 20 && i <= 33))
        {
            if(h.find(std::tuple<int>(i))==h.end())
                check = false;
        }
    }
    BOOST_TEST(check);
}

int main() {

    check_helper_fn_pixel_to_tuple();
    check_helper_fn_tuple_to_tuple();
    check_helper_tuple_limit();
    check_filler();

    return boost::report_errors();
}
