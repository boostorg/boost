// Copyright 2022 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#if defined(__GNUC__) && __GNUC__ == 12
# pragma GCC diagnostic ignored "-Wrestrict" // false positive
#endif

#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <boost/core/type_name.hpp>
#include <string>
#include <ostream>

namespace detail
{

template<class T1, class T2>
constexpr auto equals( T1 t1, T2 t2, int ) -> decltype( t1 == t2 )
{
    return t1 == t2;
}

template<class T1, class T2>
constexpr bool equals( T1 /*t1*/, T2 /*t2*/, long )
{
    return false;
}

using namespace boost::describe;

template<class T, class C,
    class L = describe_members<C, mod_any_access | mod_any_member>>
char const* get_member_name( T C::* pm, int )
{
    char const * name = nullptr;

    boost::mp11::mp_for_each<L>([&](auto D){

        if( equals( D.pointer, pm, 0 ) ) name = D.name;

    });

    return name;
}

template<class T, class C>
char const* get_member_name( T C::* /*pm*/, long )
{
    return nullptr;
}

} // namespace detail

template<class T, class C>
std::string pm_to_string( T C::* pm )
{
    char const * name = ::detail::get_member_name( pm, 0 );

    if( name == nullptr ) name = "(unknown)";

    return "&" + boost::core::type_name<C>() + "::" + name
        + " [" + boost::core::type_name<T>() + "]";
}

template<class T, class C>
std::ostream& operator<<( std::ostream& os, T C::* pm )
{
    os << pm_to_string( pm );
    return os;
}

struct X
{
    int m;
    int f() const { return m; }
};

BOOST_DESCRIBE_STRUCT(X, (), (m, f))

struct Y: public X
{
    int m;
    int g() const { return -m; }
};

BOOST_DESCRIBE_STRUCT(Y, (X), (m, g))

struct Z
{
    void h() {}
};

#if !defined(_MSC_VER) || defined(__clang__)

// MSVC doesn't support BOOST_DESCRIBE_CLASS inside
// templates until 2022 in C++20 mode

template<class T1, class T2> struct pair
{
    T1 first;
    T2 second;

    BOOST_DESCRIBE_CLASS(pair, (), (first, second), (), ())
};

#endif

#include <iostream>

int main()
{
    std::cout << &X::m << std::endl;
    std::cout << &X::f << std::endl;

    std::cout << &Y::m << std::endl;
    std::cout << &Y::f << std::endl;
    std::cout << &Y::g << std::endl;

    std::cout << &Z::h << std::endl;

#if !defined(_MSC_VER) || defined(__clang__)
    std::cout << &pair<int, float>::second << std::endl;
#endif
}
