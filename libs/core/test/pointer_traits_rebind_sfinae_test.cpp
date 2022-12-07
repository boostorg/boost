/*
Copyright 2021 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/config.hpp>
#if !defined(BOOST_NO_CXX11_TEMPLATE_ALIASES) && \
    (!defined(BOOST_GCC) || (BOOST_GCC >= 40800)) && \
    (!defined(BOOST_MSVC) || (BOOST_MSVC >= 1900))
#include <boost/core/pointer_traits.hpp>
#include <boost/core/lightweight_test.hpp>

template<class>
struct valid {
    typedef void type;
};

#if !defined(BOOST_GCC) || (BOOST_GCC >= 50000)
template<class, class, class = void>
struct has_rebind {
    static const bool value = false;
};

template<class T, class U>
struct has_rebind<T, U,
    typename valid<typename T::template rebind<U> >::type> {
    static const bool value = true;
};
#else
template<template<class> class T, class U>
using defer = T<U>;

template<class T, class U>
using rebind = defer<T::template rebind, U>;

template<class, class, template<class, class> class = rebind, class = void>
struct has_rebind {
    static const bool value = false;
};

template<class T, class U, template<class, class> class R>
struct has_rebind<T, U, R, typename valid<R<T, U> >::type> {
    static const bool value = true;
};
#endif

struct P1 { };

struct P2 {
    typedef int element_type;
};

struct P3 {
    typedef int element_type;

    template<class>
    using rebind = P3;
};

template<class T>
struct S {
    typedef T element_type;
};

typedef S<int> P4;

int main()
{
    BOOST_TEST((!has_rebind<boost::pointer_traits<P1>, char>::value));
    BOOST_TEST((!has_rebind<boost::pointer_traits<P2>, char>::value));
    BOOST_TEST((has_rebind<boost::pointer_traits<P3>, char>::value));
    BOOST_TEST((has_rebind<boost::pointer_traits<P4>, char>::value));
    BOOST_TEST((has_rebind<boost::pointer_traits<int*>, char>::value));
    return boost::report_errors();
}
#else
int main()
{
    return 0;
}
#endif
