// Copyright 2018-2022 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifdef BOOST_LEAF_TEST_SINGLE_HEADER
#   include "leaf.hpp"
#else
#   include <boost/leaf/result.hpp>
#   include <boost/leaf/handle_errors.hpp>
#endif

#ifdef BOOST_LEAF_BOOST_AVAILABLE
#   include <boost/config/workaround.hpp>
#else
#   define BOOST_WORKAROUND(a,b) 0
#endif

#include "lightweight_test.hpp"

namespace leaf = boost::leaf;

struct value
{
    int x;

    explicit value( int x ): x(x) { };

#ifndef BOOST_LEAF_NO_CXX11_REF_QUALIFIERS
    value( value const & ) = delete;
    value( value && ) = default;
#endif
};

leaf::result<value> f1( bool success )
{
    if( success )
        return value { 21 };
    else
        return leaf::new_error();
}

#if BOOST_LEAF_GNUC_STMTEXPR

leaf::result<value> f2( bool success )
{
    return value { BOOST_LEAF_CHECK(f1(success)).x + BOOST_LEAF_CHECK(f1(success)).x };
}

#else

leaf::result<value> f2( bool success )
{
    BOOST_LEAF_AUTO(a, f1(success));
    BOOST_LEAF_AUTO(b, f1(success));
    return value { a.x + b.x };
}

#endif

leaf::result<void> f3( bool success )
{
    BOOST_LEAF_CHECK(f2(success));
    return { };
}

int main()
{
    BOOST_TEST_EQ(f2(true).value().x, 42);
    BOOST_TEST(!f2(false));
    BOOST_TEST(f3(true));
    BOOST_TEST(!f3(false));

    return boost::report_errors();
}
