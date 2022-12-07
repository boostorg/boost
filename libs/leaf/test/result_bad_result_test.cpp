// Copyright 2018-2022 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/leaf/config.hpp>

#ifdef BOOST_LEAF_NO_EXCEPTIONS

#include <iostream>

int main()
{
    std::cout << "Unit test not applicable." << std::endl;
    return 0;
}

#else

#ifdef BOOST_LEAF_TEST_SINGLE_HEADER
#   include "leaf.hpp"
#else
#   include <boost/leaf/handle_errors.hpp>
#   include <boost/leaf/pred.hpp>
#   include <boost/leaf/result.hpp>
#endif

#include "lightweight_test.hpp"
#ifdef BOOST_LEAF_BOOST_AVAILABLE
#   include <boost/config/workaround.hpp>
#else
#   define BOOST_WORKAROUND(a,b) 0
#endif

namespace leaf = boost::leaf;

struct e_test { int value; };

int check( leaf::bad_result const &, leaf::match_value<e_test, 42> )
{
    return 1;
}

struct res { int val; };

int main()
{
    {
        int r = leaf::try_catch(
            []
            {
                leaf::result<int> r = leaf::new_error(e_test{42});
                (void) r.value();
                return 0;
            },
            check );
        BOOST_TEST_EQ(r, 1);
    }
#if !BOOST_WORKAROUND( BOOST_GCC, < 50000 )
    {
        int r = leaf::try_catch(
            []
            {
                leaf::result<int> const r = leaf::new_error(e_test{42});
                (void) r.value();
                return 0;
            },
            check );
        BOOST_TEST_EQ(r, 1);
    }
#endif
    {
        leaf::result<res> r = leaf::new_error(e_test{42});
        BOOST_TEST(r.operator->()==0);
    }
#if !BOOST_WORKAROUND( BOOST_GCC, < 50000 )
    {
        leaf::result<res> const r = leaf::new_error(e_test{42});
        BOOST_TEST(r.operator->()==0);
    }
#endif
    {
        int r = leaf::try_catch(
            []
            {
                leaf::result<void> r = leaf::new_error(e_test{42});
                (void) r.value();
                return 0;
            },
            check );
        BOOST_TEST_EQ(r, 1);
    }
#if !BOOST_WORKAROUND( BOOST_GCC, < 50000 )
    {
        int r = leaf::try_catch(
            []
            {
                leaf::result<void> const r = leaf::new_error(e_test{42});
                (void) r.value();
                return 0;
            },
            check );
        BOOST_TEST_EQ(r, 1);
    }
#endif

    return boost::report_errors();
}

#endif
