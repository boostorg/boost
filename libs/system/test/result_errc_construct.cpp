// Copyright 2021 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/system/result.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <type_traits>

using namespace boost::system;

int main()
{
    BOOST_TEST_TRAIT_FALSE((std::is_convertible<errc::errc_t, result<int>>));
    BOOST_TEST_TRAIT_FALSE((std::is_constructible<result<int>, errc::errc_t>));

    BOOST_TEST_TRAIT_FALSE((std::is_convertible<errc::errc_t, result<double>>));
    BOOST_TEST_TRAIT_FALSE((std::is_constructible<result<double>, errc::errc_t>));

    BOOST_TEST_TRAIT_FALSE((std::is_convertible<errc::errc_t, result<bool>>));
    BOOST_TEST_TRAIT_FALSE((std::is_constructible<result<bool>, errc::errc_t>));

    return boost::report_errors();
}
