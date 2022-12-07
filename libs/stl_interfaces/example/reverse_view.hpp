// Copyright (C) 2022 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_STL_INTERFACES_EXAMPLE_REVERSE_VIEW_HPP
#define BOOST_STL_INTERFACES_EXAMPLE_REVERSE_VIEW_HPP
#include "all_view.hpp"

namespace detail {
    //[ reverse_view_defn
    // We need to treat iterator/sentinel ranges differently from iterator
    // ranges (a.k.a. common_ranges).  If the iterator and sentinel are
    // different types, we need to advance the iterator to the end of the
    // range before we can move through the range in reverse.
    template<bool CommonRange>
    struct set_rev_rng_first
    {
        template<typename V>
        static auto call(V const & v)
        {
            return boost::stl_interfaces::make_reverse_iterator(v.end());
        }
    };

    template<>
    struct set_rev_rng_first<false>
    {
        template<typename V>
        static auto call(V const & v)
        {
            auto v_f = v.begin();
            auto const v_l = v.end();
            while (v_f != v_l) {
                ++v_f;
            }
            return boost::stl_interfaces::make_reverse_iterator(v_f);
        }
    };

    // This view reverses whatever view you construct it from.  Unlike
    // all_view, it requires that it be constructed from a view.  This is
    // enforced through a constraint in C++20 and later, but is left up to the
    // user in earlier C++ modes.
#if BOOST_STL_INTERFACES_USE_CONCEPTS
    template<std::ranges::view View>
    requires std::is_object_v<View>
#else
    template<
        typename View,
        typename Enable = std::enable_if_t<std::is_object<View>::value>>
#endif
    struct reverse_view
        : boost::stl_interfaces::view_interface<reverse_view<View>>
    {
        using view_iterator = iterator_t<View>;
        using view_sentinel = sentinel_t<View>;

        // This would be better off as a constraint in C++20 and later.
        static_assert(
            std::is_base_of<
                std::bidirectional_iterator_tag,
                typename std::iterator_traits<
                    view_iterator>::iterator_category>::value,
            "A reversed view must have bidirectional iterators.");

        using iterator = boost::stl_interfaces::reverse_iterator<view_iterator>;

        constexpr reverse_view() = default;

#if BOOST_STL_INTERFACES_USE_CONCEPTS
        template<typename V>
        requires std::is_same_v<std::remove_reference_t<V>, View>
#else
        template<
            typename V,
            typename E = std::enable_if_t<
                std::is_same<std::remove_reference_t<V>, View>::value>>
#endif
        constexpr reverse_view(int, V && v) : v_{(V &&) v}
        {
            // To keep the code simpler, we just store the iterator to the end
            // of v, whether v is a common_range or has different iterator and
            // sentinel types.
            first_ = set_rev_rng_first<
                std::is_same<view_iterator, view_sentinel>::value>::call(v_);
        }

        constexpr iterator begin() const { return first_; }
        constexpr iterator end() const
        {
            return boost::stl_interfaces::make_reverse_iterator(v_.begin());
        }

        // Return the underlying view that this view reverses.
        constexpr View base() const { return v_; }

    private:
        View v_ = View();
        iterator first_;
    };

    // is_reverse_view lets us detect construction of a reverse_view from
    // another reverse_view, and take appropriate action (see below).
    template<typename T>
    struct is_reverse_view : std::false_type
    {};
    template<typename T>
    struct is_reverse_view<reverse_view<T>> : std::true_type
    {};

#if defined(__cpp_deduction_guides)
    template<typename R>
    reverse_view(int, R &&)->detail::reverse_view<std::remove_reference_t<R>>;
#endif
    //]
}

#if defined(__cpp_deduction_guides)
//[ reverse_defn
// We want to condition how we construct our view based on whether R is itself
// a reverse_view.  If R is a reverse_view, just return the view it's
// reversing.
//
// In C++20 and later, you might want to constrain this lambda to require that
// R is a std::ranges::view, since that's what reverse_view requires.
inline constexpr boost::stl_interfaces::closure reverse =
    []<typename R>(R && r) {
        if constexpr (detail::is_reverse_view<std::decay_t<R>>::value) {
            return ((R &&) r).base();
        } else {
            return detail::reverse_view(0, (R &&) r);
        }
    };
//]
#endif

//[ reverse_enable_borrowed_range
// Don't forget to designate our view as a borrowed range.
#if BOOST_STL_INTERFACES_USE_CONCEPTS
namespace std::ranges {
    template<typename View>
    inline constexpr bool enable_borrowed_range<detail::reverse_view<View>> =
        true;
}
#endif
//]

#endif
