// Copyright (C) 2022 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/stl_interfaces/reverse_iterator.hpp>
#include <boost/stl_interfaces/view_interface.hpp>
#include <boost/stl_interfaces/view_adaptor.hpp>

#include "ill_formed.hpp"
#include "../example/all_view.hpp"
#include "../example/reverse_view.hpp"
#include "../example/take_view.hpp"

#include <boost/core/lightweight_test.hpp>

#include <algorithm>
#include <vector>


namespace detail {
    template<
        typename R,
        bool ReverseView = is_reverse_view<std::decay_t<R>>::value>
    struct reverse_impl_impl
    {
        static constexpr auto call(R && r) { return ((R &&) r).base(); }
    };
    template<typename R>
    struct reverse_impl_impl<R, false>
    {
        static constexpr auto call(R && r)
        {
            return reverse_view<std::remove_reference_t<R>>(0, (R &&) r);
        }
    };

    struct reverse_impl
        : boost::stl_interfaces::range_adaptor_closure<reverse_impl>
    {
        template<typename R>
        constexpr auto operator()(R && r) const
        {
            return reverse_impl_impl<R>::call((R &&) r);
        }
    };

    struct take_impl
    {
        template<typename R>
        constexpr auto operator()(R && r, int n) const
        {
            return take_view<std::remove_reference_t<R>>((R &&) r, n);
        }

        constexpr auto operator()(int n) const
        {
            using closure_func_type =
                decltype(boost::stl_interfaces::bind_back(*this, n));
            return boost::stl_interfaces::closure<closure_func_type>(
                boost::stl_interfaces::bind_back(*this, n));
        }
    };
}

#if defined(__cpp_inline_variables)
inline constexpr detail::reverse_impl old_reverse;
inline constexpr detail::take_impl old_take;
#else
namespace {
    constexpr detail::reverse_impl old_reverse;
    constexpr detail::take_impl old_take;
}
#endif

int main()
{
    // non-closures
    {
        std::vector<int> vec1 = {0, 1, 2, 3, 4, 5, 6, 7};

        std::vector<int> vec2;
        for (auto x : old_all(vec1) | old_reverse) {
            vec2.push_back(x);
        }

        std::reverse(vec2.begin(), vec2.end());
        BOOST_TEST(vec1 == vec2);
    }

    {
        std::vector<int> const vec1 = {0, 1, 2, 3, 4, 5, 6, 7};

        std::vector<int> vec2;
        for (auto x : old_all(vec1) | old_reverse) {
            vec2.push_back(x);
        }

        std::reverse(vec2.begin(), vec2.end());
        BOOST_TEST(vec1 == vec2);
    }

    {
        std::vector<int> const vec1 = {0, 1, 2, 3, 4, 5, 6, 7};

        std::vector<int> vec2;
        for (auto x : old_all(vec1) | old_reverse | old_reverse) {
            vec2.push_back(x);
        }

        BOOST_TEST(vec1 == vec2);
    }

    // Mismatched begin and end; only test this in C++17 and later.
#if 201703L <= __cplusplus
    {
        std::vector<int> const vec1 = {0, 1, 2, 3, 4, 5, 6, 7};

        std::vector<int> vec2;
        for (auto x : old_all(vec1) | old_take(3)) {
            vec2.push_back(x);
        }

        BOOST_TEST(vec2 == (std::vector<int>{0, 1, 2}));
    }

    {
        std::vector<int> const vec1 = {0, 1, 2, 3, 4, 5, 6, 7};

        std::vector<int> vec2;
        for (auto x : old_all(vec1) | old_reverse | old_take(3)) {
            vec2.push_back(x);
        }

        BOOST_TEST(vec2 == (std::vector<int>{7, 6, 5}));
    }
#endif

#if 201703L <= __cplusplus
    // closures
    {
        std::vector<int> vec1 = {0, 1, 2, 3, 4, 5, 6, 7};

        std::vector<int> vec2;
        for (auto x : all(vec1) | reverse) {
            vec2.push_back(x);
        }

        std::reverse(vec2.begin(), vec2.end());
        BOOST_TEST(vec1 == vec2);
    }

    {
        std::vector<int> const vec1 = {0, 1, 2, 3, 4, 5, 6, 7};

        std::vector<int> vec2;
        for (auto x : all(vec1) | reverse) {
            vec2.push_back(x);
        }

        std::reverse(vec2.begin(), vec2.end());
        BOOST_TEST(vec1 == vec2);
    }

    {
        std::vector<int> const vec1 = {0, 1, 2, 3, 4, 5, 6, 7};

        std::vector<int> vec2;
        for (auto x : all(vec1) | reverse | reverse) {
            vec2.push_back(x);
        }

        BOOST_TEST(vec1 == vec2);
    }

    {
        std::vector<int> const vec1 = {0, 1, 2, 3, 4, 5, 6, 7};

        std::vector<int> vec2;
        for (auto x : all(vec1) | take(3)) {
            vec2.push_back(x);
        }

        BOOST_TEST(vec2 == (std::vector<int>{0, 1, 2}));
    }

    {
        std::vector<int> const vec1 = {0, 1, 2, 3, 4, 5, 6, 7};

        std::vector<int> vec2;
        for (auto x : all(vec1) | reverse | take(3)) {
            vec2.push_back(x);
        }

        BOOST_TEST(vec2 == (std::vector<int>{7, 6, 5}));
    }
#endif

    // User views mixed with std views.
#if BOOST_STL_INTERFACES_USE_CONCEPTS
    {
        std::vector<int> vec1 = {0, 1, 2, 3, 4, 5, 6, 7};

        std::vector<int> vec2;
        for (auto x : all(vec1) | std::views::reverse | old_reverse |
                          std::views::reverse) {
            vec2.push_back(x);
        }

        std::reverse(vec2.begin(), vec2.end());
        BOOST_TEST(vec1 == vec2);
    }

    {
        std::vector<int> const vec1 = {0, 1, 2, 3, 4, 5, 6, 7};

        std::vector<int> vec2;
        for (auto x : all(vec1) | reverse | std::views::take(3)) {
            vec2.push_back(x);
        }

        BOOST_TEST(vec2 == (std::vector<int>{7, 6, 5}));
    }
#endif

    return boost::report_errors();
}
