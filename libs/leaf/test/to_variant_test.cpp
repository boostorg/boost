// Copyright 2018-2022 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if __cplusplus < 201703L

#include <iostream>

int main()
{
    std::cout << "Unit test not applicable." << std::endl;
    return 0;
}

#else

#ifdef BOOST_LEAF_TEST_SINGLE_HEADER
#   include "leaf.hpp"
#else
#   include <boost/leaf/to_variant.hpp>
#endif

#include "_test_ec.hpp"
#include "lightweight_test.hpp"

namespace leaf = boost::leaf;

enum class E1 { e11, e12, e13 };
enum class E2 { e21, e22, e23 };
enum class E3 { e31, e32, e33 };

int main()
{
#if !defined(__clang__) || __clang_major__ < 5 || __clang_major__ > 7 // See https://github.com/llvm/llvm-project/issues/32569
    {
        auto v = leaf::to_variant<E1, E2, E3>(
            [ ]() -> leaf::result<int>
            {
                return 42;
            } );
        BOOST_TEST(v.index() == 0);
        BOOST_TEST(std::get<0>(v) == 42);
    }

    {
        auto v = leaf::to_variant<E1, E2, E3>(
            [ ]() -> leaf::result<int>
            {
                return leaf::new_error(E1::e12, E3::e31);
            } );
        BOOST_TEST(v.index() == 1);
        auto t = std::get<1>(v);

        BOOST_TEST(std::get<0>(t).has_value());
        BOOST_TEST(!std::get<1>(t).has_value());
        BOOST_TEST(std::get<2>(t).has_value());

        BOOST_TEST(std::get<0>(t).value() == E1::e12);
        BOOST_TEST(std::get<2>(t).value() == E3::e31);
    }
#endif

    return boost::report_errors();
}

#endif
