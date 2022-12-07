/*
Copyright 2022 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/core/allocator_access.hpp>
#include <boost/core/lightweight_test.hpp>

template<class T>
struct A {
    typedef T value_type;
    A() { }
};

int main()
{
    {
        A<int> a;
        int i[3] = { 5, 5, 5 };
        boost::allocator_construct_n(a, &i[0], 3);
        BOOST_TEST_EQ(i[0], 0);
        BOOST_TEST_EQ(i[1], 0);
        BOOST_TEST_EQ(i[2], 0);
    }
    {
        A<int> a;
        int i[4] = { 5, 5, 5, 5 };
        int j[2] = { 1, 2 };
        boost::allocator_construct_n(a, &i[0], 4, &j[0], 2);
        BOOST_TEST_EQ(i[0], 1);
        BOOST_TEST_EQ(i[1], 2);
        BOOST_TEST_EQ(i[2], 1);
        BOOST_TEST_EQ(i[3], 2);
    }
    {
        A<int> a;
        int i[3] = { 5, 5, 5 };
        int j[3] = { 1, 2, 3 };
        boost::allocator_construct_n(a, &i[0], 3, &j[0]);
        BOOST_TEST_EQ(i[0], 1);
        BOOST_TEST_EQ(i[1], 2);
        BOOST_TEST_EQ(i[2], 3);
    }
    return boost::report_errors();
}
