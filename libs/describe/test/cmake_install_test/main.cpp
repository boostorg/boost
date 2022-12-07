// Copyright 2017, 2020, 2021, 2022 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/describe.hpp>
#include <cassert>

#define BOOST_TEST(expr) assert(expr)
#define BOOST_TEST_EQ(x1, x2) assert((x1)==(x2))

struct X
{
    int m;
};

BOOST_DESCRIBE_STRUCT(X, (), (m))

struct Y: public X
{
    void m() {}
};

BOOST_DESCRIBE_STRUCT(Y, (X), (m))

#if !defined(BOOST_DESCRIBE_CXX14)

#pragma message "Skipping test because C++14 is not available"

int main()
{
}

#else

BOOST_DEFINE_ENUM(E, v1, v2, v3)

#include <boost/mp11.hpp>

int main()
{
    using namespace boost::describe;
    using namespace boost::mp11;

    BOOST_TEST_EQ( (mp_size< describe_enumerators<E> >::value), 3 );
    BOOST_TEST_EQ( (mp_size< describe_bases<Y, mod_any_access> >::value), 1 );
    BOOST_TEST_EQ( (mp_size< describe_members<Y, mod_any_access | mod_inherited | mod_hidden> >::value), 1 );
}

#endif // !defined(BOOST_DESCRIBE_CXX14)
