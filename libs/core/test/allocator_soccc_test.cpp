/*
Copyright 2020 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/core/allocator_access.hpp>
#include <boost/core/lightweight_test.hpp>

template<class T>
struct A1 {
    typedef T value_type;
    A1(int n)
        : value(n) { }
    int value;
};

template<class T>
struct A2 {
    typedef T value_type;
    A2(int n)
        : value(n) { }
    A2 select_on_container_copy_construction() const {
        return A2(value + 1);
    }
    int value;
};

int main()
{
    BOOST_TEST_EQ(1, boost::
        allocator_select_on_container_copy_construction(A1<int>(1)).value);
    BOOST_TEST_EQ(2, boost::
        allocator_select_on_container_copy_construction(A2<int>(1)).value);
    return boost::report_errors();
}
