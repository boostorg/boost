/*
Copyright 2022 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/core/allocator_access.hpp>
#include <boost/core/lightweight_test.hpp>

struct S {
    static int count;
    S() {
        ++count;
    }
    S(const S&) {
        ++count;
    }
    ~S() {
        --count;
    }
};

int S::count = 0;

template<class T>
struct A1 {
    typedef T value_type;
    A1() { }
};

#if !defined(BOOST_NO_CXX11_ALLOCATOR)
template<class T>
struct A2 {
    typedef T value_type;
    A2() { }
    template<class U>
    void destroy(U* p) {
        *p = U();
    }
};
#endif

int main()
{
    {
        A1<int> a;
        S s[3];
        boost::allocator_destroy_n(a, &s[0], 3);
        BOOST_TEST_EQ(S::count, 0);
        ::new((void*)&s[0]) S();
        ::new((void*)&s[1]) S();
        ::new((void*)&s[2]) S();
    }
#if !defined(BOOST_NO_CXX11_ALLOCATOR)
    {
        A2<int> a;
        int i[3] = { 5, 5, 5 };
        boost::allocator_destroy_n(a, &i[0], 3);
        BOOST_TEST_EQ(i[0], 0);
        BOOST_TEST_EQ(i[1], 0);
        BOOST_TEST_EQ(i[2], 0);
    }
#endif
    return boost::report_errors();
}
