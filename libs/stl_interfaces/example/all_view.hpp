// Copyright (C) 2022 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_STL_INTERFACES_EXAMPLE_ALL_VIEW_HPP
#define BOOST_STL_INTERFACES_EXAMPLE_ALL_VIEW_HPP
#include <boost/stl_interfaces/view_adaptor.hpp>

namespace detail {
#if BOOST_STL_INTERFACES_USE_CONCEPTS
    template<typename T>
    using iterator_t = std::ranges::iterator_t<T>;
    template<typename T>
    using sentinel_t = std::ranges::sentinel_t<T>;
#else
    //[ iterator_sentinel_t_defn
    // These act as stand-ins for std::ranges::iterator_t and -sentinel_t; we
    // need these in C++17 and earlier.  They are not really complete, because
    // they ignore free implementations of begin() and end().  This means they
    // don't support builtin arrays, or other ranges that have ADL-findable
    // begin() and end().  If you make your own versions, you should probably
    // use the boost::begin() and -end() from Boost.Range, or the
    // implementations from range-v3.
    template<typename T>
    using iterator_t = decltype(std::declval<T &>().begin());
    template<typename T>
    using sentinel_t = decltype(std::declval<T &>().end());
    //]
#endif

    //[ all_view_defn
    // This type allows us to implement a simplified version of the
    // std::views::all range adaptor for pre-C++20 builds.  Instead of
    // producing different kinds of ranges based on whether R is a
    // std::ranges::view, or would be better represented as a
    // std::ranges::ref_view or std::ranges::owning_view, it just grabs
    // begin() and end() out of R.  It also uses member-begin() and -end(), so
    // it doesn't work with builtin arrays.  It should probably use
    // boost::begin() and -end(), or something comparable.
    //
    // We constrain the template to only accept object-types, so that we don't
    // instantiate all_view with pointer or reference types.  We should also
    // require that R have .begin() and .end() members, but this is an
    // intentionally simplified example.
    //
    // We're putting the view in a detail namespace, because we don't expect
    // users to use our view directly; they should use the associated view
    // adaptor instead.  If you also want users to directly construct your
    // view-type, you would move it out of detail::.
    //
    // If you want to make views and view adaptors that will work with
    // pre-C++20 code, and then provide concept constraints in C++20 and
    // later, this is a reasonable pattern -- write the template-head twice:
    // once for C++20 concepts, and one for SFINAE.  Note that
    // BOOST_STL_INTERFACES_USE_CONCEPTS includes defined(__cpp_lib_concepts)
    // && defined(__cpp_lib_ranges), and any preprocessor predicate you use
    // should as well.
#if BOOST_STL_INTERFACES_USE_CONCEPTS
    template<typename R>
    requires std::is_object_v<R>
#else
    template<
        typename R,
        typename Enable = std::enable_if_t<std::is_object<R>::value>>
#endif
    struct all_view : boost::stl_interfaces::view_interface<all_view<R>>
    {
        using iterator = iterator_t<R>;
        using sentinel = sentinel_t<R>;

        // Here, we want a constructor that takes a forwarding reference, so
        // we introduce a new template parameter R2, and constrain it to be
        // the same as R.  The int parameter is there to prevent getting in
        // the way of the special member functions like the copy constructor.
        // Since we don't want users directly constructing this type anyway,
        // the non-ideal ergonomics of this extra int don't matter.
#if BOOST_STL_INTERFACES_USE_CONCEPTS
        template<typename R2>
        requires std::is_same_v<std::remove_reference_t<R2>, R>
#else
        template<
            typename R2,
            typename E = std::enable_if_t<
                std::is_same<std::remove_reference_t<R2>, R>::value>>
#endif
        explicit all_view(int, R2 && r) : first_(r.begin()), last_(r.end()) {}

        iterator begin() const { return first_; }
        sentinel end() const { return last_; }

    private:
        iterator first_;
        sentinel last_;
    };

    // This just makes out implementations below a bit easier to write.
#if defined(__cpp_deduction_guides)
    template<typename R>
    all_view(int, R &&)->detail::all_view<std::remove_reference_t<R>>;
#endif
    //]

    //[all_impl_defn
    // For C++20 views, there is usually some type like this.  This type
    // implements the functions that construct our view.  An invocable object
    // that uses this implementation will follow.  We need to inherit from
    // range_adaptor_closure to make our view adaptor compatible with other
    // view adaptors using the operator| "pipe" syntax.
    struct all_impl : boost::stl_interfaces::range_adaptor_closure<all_impl>
    {
        template<typename R>
        constexpr auto operator()(R && r) const
        {
            // The use of std::remove_reference is important, so that we
            // instantiate all_view with a non-reference type.  It is also
            // important not to use std::decay or std::remove_cvref here
            // instead.  If you do that and you pass a T const & r, you'll end
            // up trying to initialize all_view<T>::first_ (which is a
            // T::iterator) from r.begin(), which is a T::const_iterator.
            // That won't work.
            return all_view<std::remove_reference_t<R>>(0, (R &&) r);
        }
    };
    //]
}

//[ old_all_defn
// Here we create the actual invocable that the user will call.  It is just a
// constexpr all_impl variable.  Before C++17, you need to put it in an
// anonymous namespace to avoid violating the ODR. With this in scope, the
// user has everything necessary to use old_all().  I called it old_all(),
// because there's an even easier way to do this, as shown with all() below.
#if defined(__cpp_inline_variables)
inline constexpr detail::all_impl old_all;
#else
namespace {
    constexpr detail::all_impl old_all;
}
#endif
//]

#if defined(__cpp_deduction_guides)
//[ all_defn
// This is the preferred way to make a view adaptor.  We can use a simple
// template called closure that already inherits from
// boost::stl_interfaces::range_adaptor_closure, and takes any function that
// can construct a closure from a given range.  In this case, our closure is
// just an all_view.  Later we'll see other kinds of closures.
inline constexpr boost::stl_interfaces::closure all = []<typename R>(R && r) {
    return detail::all_view(0, (R &&) r);
};
//]
#endif

//[ all_enable_borrowed_range
// Any view that you make that is a proper view -- that is, it does not own
// the elements between its .begin() and .end() -- should be designated as a
// borrowed range, so that the std::ranges code treats is properly.  Without
// this, std::ranges code will assume that .begin() taken from an rvalue
// reference to your view type is a dangling iterator.
//
// As an example of this behavior, say you call
// std::ranges::find(std::vector<int>{}, 42).  The result will be a
// std::dangling instead of an iterator, because any iterator you pull out of
// a std::vector<int> && is potentially a dangling reference to an element of
// a temporary std::vector<int>.
#if BOOST_STL_INTERFACES_USE_CONCEPTS
namespace std::ranges {
    template<typename View>
    inline constexpr bool enable_borrowed_range<detail::all_view<View>> = true;
}
#endif
//]

#endif
