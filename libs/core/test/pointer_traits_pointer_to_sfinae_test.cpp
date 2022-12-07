/*
Copyright 2021 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/config.hpp>
#if !defined(BOOST_NO_CXX11_DECLTYPE_N3276)
#include <boost/core/pointer_traits.hpp>
#include <boost/core/lightweight_test.hpp>

template<class T>
class has_pointer_to {
    template<class>
    struct result {
        char x, y;
    };

    template<class O>
    static auto check(int) -> result<decltype(O::pointer_to)>;

    template<class O>
    static char check(long);

public:
    static const bool value = sizeof(check<T>(0)) > 1;
};

struct P1 { };

struct P2 {
    typedef int element_type;

    static int* pointer_to(int& value) {
        return &value;
    }
};

struct P3 {
    typedef void element_type;
};

struct P4 {
    typedef int element_type;
};

struct P5 {
    typedef int element_type;

    static int* pointer_to() {
        return 0;
    }
};

int main()
{
    BOOST_TEST((!has_pointer_to<boost::pointer_traits<P1> >::value));
    BOOST_TEST(has_pointer_to<boost::pointer_traits<P2> >::value);
    BOOST_TEST(!has_pointer_to<boost::pointer_traits<P3> >::value);
    BOOST_TEST(!has_pointer_to<boost::pointer_traits<P4> >::value);
    BOOST_TEST(!has_pointer_to<boost::pointer_traits<P5> >::value);
    BOOST_TEST(has_pointer_to<boost::pointer_traits<int*> >::value);
    BOOST_TEST(!has_pointer_to<boost::pointer_traits<void*> >::value);
    return boost::report_errors();
}
#else
int main()
{
    return 0;
}
#endif
