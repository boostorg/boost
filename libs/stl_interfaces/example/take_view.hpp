// Copyright (C) 2022 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_STL_INTERFACES_EXAMPLE_TAKE_VIEW_HPP
#define BOOST_STL_INTERFACES_EXAMPLE_TAKE_VIEW_HPP
#include "all_view.hpp"

namespace detail {
    //[ take_view_defn
    // This is a really simple iterator that converts the given iterator Iter
    // to a forward_iterator that counts how many times it has ben
    // incremented.  It counts down from an initial count to zero.
    template<typename Iter>
    struct take_iterator
        : boost::stl_interfaces::iterator_interface<
              take_iterator<Iter>,
              std::forward_iterator_tag,
              typename std::iterator_traits<Iter>::value_type,
              typename std::iterator_traits<Iter>::reference,
              typename std::iterator_traits<Iter>::pointer,
              typename std::iterator_traits<Iter>::difference_type>
    {
        constexpr take_iterator() = default;
        constexpr explicit take_iterator(Iter it, int n) :
            it_(std::move(it)), n_(n)
        {}

        constexpr Iter base() const { return it_; }
        constexpr int count() const { return n_; }

        constexpr take_iterator & operator++()
        {
            ++it_;
            --n_;
            return *this;
        }

    private:
        friend boost::stl_interfaces::access;
        constexpr Iter & base_reference() { return it_; }
        constexpr Iter const & base_reference() const { return it_; }

        template<typename Iter2>
        friend struct take_iterator;

        Iter it_;
        int n_;
    };

    // This sentinel compares equal to any take_iterator whose count has
    // reached zero, or the end of the underlying range if that comes first.
    template<typename Sentinel>
    struct take_sentinel
    {
        take_sentinel() = default;
        explicit take_sentinel(Sentinel sent) : sent_(sent) {}

        template<typename Iter>
        friend constexpr bool
        operator==(take_iterator<Iter> it, take_sentinel s)
        {
            return !it.count() || it.base() == s.sent_;
        }
        template<typename Iter>
        friend constexpr bool
        operator!=(take_iterator<Iter> it, take_sentinel s)
        {
            return !(it == s);
        }

    private:
        Sentinel sent_;
    };

    // The take_iterator and take_sentinel templates do all the hard work,
    // which leaves take_view quite simple.
#if BOOST_STL_INTERFACES_USE_CONCEPTS
    template<std::ranges::view View>
    requires std::is_object_v<View>
#else
    template<
        typename View,
        typename Enable = std::enable_if_t<std::is_object<View>::value>>
#endif
    struct take_view : boost::stl_interfaces::view_interface<take_view<View>>
    {
        using iterator = take_iterator<iterator_t<View>>;
        using sentinel = take_sentinel<sentinel_t<View>>;

        // We don't need a phony initial int param for this constructor, since
        // it already takes two parameters; it won't get confused for a copy
        // or a move.  The count here is just an int to keep things simple.
#if BOOST_STL_INTERFACES_USE_CONCEPTS
        template<typename View2>
        requires std::is_same_v<std::remove_reference_t<View2>, View>
#else
        template<
            typename View2,
            typename E = std::enable_if_t<
                std::is_same<std::remove_reference_t<View2>, View>::value>>
#endif
        explicit take_view(View2 && r, int n) :
            first_(r.begin(), n), last_(r.end())
        {}

        iterator begin() const { return first_; }
        sentinel end() const { return last_; }

    private:
        iterator first_;
        sentinel last_;
    };

#if defined(__cpp_deduction_guides)
    template<typename R>
    take_view(R &&, int)->detail::take_view<std::remove_reference_t<R>>;
#endif
    //]
}

#if defined(__cpp_deduction_guides)
//[ take_defn
// Use the adaptor template to support calling the given lambda with either
// all the parameters or all the parameters after the first.
inline constexpr boost::stl_interfaces::adaptor take =
    []<typename R>(R && r, int n) { return detail::take_view((R &&) r, n); };
//]
#endif

#if BOOST_STL_INTERFACES_USE_CONCEPTS
namespace std::ranges {
    template<typename View>
    inline constexpr bool enable_borrowed_range<detail::take_view<View>> = true;
}
#endif

#endif
