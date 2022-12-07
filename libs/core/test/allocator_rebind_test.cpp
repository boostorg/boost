/*
Copyright 2020 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/core/allocator_access.hpp>
#include <boost/core/is_same.hpp>
#include <boost/core/lightweight_test_trait.hpp>

template<class T>
struct A1 {
    typedef T value_type;
    template<class>
    struct rebind {
        typedef A1<int> other;
    };
};

template<class T>
struct A2 {
    typedef T value_type;
};

int main()
{
    BOOST_TEST_TRAIT_TRUE((boost::core::is_same<A1<int>,
        boost::allocator_rebind<A1<char>, bool>::type>));
    BOOST_TEST_TRAIT_TRUE((boost::core::is_same<A2<int>,
        boost::allocator_rebind<A2<char>, int>::type>));
    return boost::report_errors();
}
