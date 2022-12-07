/*
Copyright 2021 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/core/pointer_traits.hpp>
#include <boost/core/lightweight_test.hpp>

template<class>
struct valid {
    typedef void type;
};

template<class, class = void>
struct has_pointer {
    static const bool value = false;
};

template<class T>
struct has_pointer<T, typename valid<typename T::pointer>::type> {
    static const bool value = true;
};

template<class, class = void>
struct has_element_type {
    static const bool value = false;
};

template<class T>
struct has_element_type<T, typename valid<typename T::element_type>::type> {
    static const bool value = true;
};

template<class, class = void>
struct has_difference_type {
    static const bool value = false;
};

template<class T>
struct has_difference_type<T,
    typename valid<typename T::difference_type>::type> {
    static const bool value = true;
};

template<class, class, class = void>
struct has_rebind_to_type {
    static const bool value = false;
};

template<class T, class U>
struct has_rebind_to_type<T, U,
    typename valid<typename T::template rebind_to<U>::type>::type> {
    static const bool value = true;
};

struct P1 { };

struct P2 {
    typedef int element_type;
};

struct P3 {
    typedef int element_type;

#if !defined(BOOST_NO_CXX11_TEMPLATE_ALIASES)
    template<class>
    using rebind = P3;
#else
    template<class>
    struct rebind {
        typedef P3 other;
    };
#endif
};

template<class T>
struct S {
    typedef T element_type;
};

typedef S<int> P4;

int main()
{
    BOOST_TEST(!has_pointer<boost::pointer_traits<P1> >::value);
    BOOST_TEST(!has_element_type<boost::pointer_traits<P1> >::value);
    BOOST_TEST(!has_difference_type<boost::pointer_traits<P1> >::value);
    BOOST_TEST((!has_rebind_to_type<boost::pointer_traits<P1>, char>::value));
    BOOST_TEST(has_pointer<boost::pointer_traits<P2> >::value);
    BOOST_TEST(has_element_type<boost::pointer_traits<P2> >::value);
    BOOST_TEST(has_difference_type<boost::pointer_traits<P2> >::value);
    BOOST_TEST((!has_rebind_to_type<boost::pointer_traits<P2>, char>::value));
    BOOST_TEST(has_pointer<boost::pointer_traits<P3> >::value);
    BOOST_TEST(has_element_type<boost::pointer_traits<P3> >::value);
    BOOST_TEST(has_difference_type<boost::pointer_traits<P3> >::value);
    BOOST_TEST((has_rebind_to_type<boost::pointer_traits<P3>, char>::value));
    BOOST_TEST(has_pointer<boost::pointer_traits<P4> >::value);
    BOOST_TEST(has_element_type<boost::pointer_traits<P4> >::value);
    BOOST_TEST(has_difference_type<boost::pointer_traits<P4> >::value);
    BOOST_TEST((has_rebind_to_type<boost::pointer_traits<P4>, char>::value));
    BOOST_TEST(has_pointer<boost::pointer_traits<int*> >::value);
    BOOST_TEST(has_element_type<boost::pointer_traits<int*> >::value);
    BOOST_TEST(has_difference_type<boost::pointer_traits<int*> >::value);
    BOOST_TEST((has_rebind_to_type<boost::pointer_traits<int*>, char>::value));
    return boost::report_errors();
}
