// Copyright 2018-2022 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_LEAF_NO_EXCEPTIONS
#define BOOST_LEAF_CFG_STD_SYSTEM_ERROR 0
#define BOOST_LEAF_CFG_STD_STRING 0
#ifdef BOOST_LEAF_CFG_DIAGNOSTICS
#   undef BOOST_LEAF_CFG_DIAGNOSTICS
#endif
#define BOOST_LEAF_CFG_DIAGNOSTICS 0
#define BOOST_LEAF_USE_TLS_ARRAY
#define BOOST_LEAF_CFG_TLS_ARRAY_SIZE 64
#define BOOST_LEAF_CFG_TLS_ARRAY_START_INDEX 10

#ifdef BOOST_LEAF_TEST_SINGLE_HEADER
#   include "leaf.hpp"
#else
#   include <boost/leaf/handle_errors.hpp>
#   include <boost/leaf/result.hpp>
#endif

#include <limits>
#include <vector>
#include <algorithm>
#include "lightweight_test.hpp"

namespace leaf = boost::leaf;

static void * tls_storage[BOOST_LEAF_CFG_TLS_ARRAY_SIZE];
static int min_tls_index = std::numeric_limits<int>::max();
static int max_tls_index = std::numeric_limits<int>::min();

namespace boost { namespace leaf {

namespace tls
{
    void * read_void_ptr( int tls_index ) noexcept
    {
        BOOST_TEST_GE(tls_index, (BOOST_LEAF_CFG_TLS_ARRAY_START_INDEX));
        BOOST_TEST_LT(tls_index, (BOOST_LEAF_CFG_TLS_ARRAY_SIZE));
        min_tls_index = std::min(min_tls_index, tls_index);
        max_tls_index = std::max(max_tls_index, tls_index);
        return tls_storage[tls_index];
    }

    void write_void_ptr( int tls_index, void * p ) noexcept
    {
        BOOST_TEST_GE(tls_index, (BOOST_LEAF_CFG_TLS_ARRAY_START_INDEX));
        BOOST_TEST_LT(tls_index, (BOOST_LEAF_CFG_TLS_ARRAY_SIZE));
        min_tls_index = std::min(min_tls_index, tls_index);
        max_tls_index = std::max(max_tls_index, tls_index);
        tls_storage[tls_index] = p;
    }
}

} }

template <int>
struct my_error_info
{
};

// Mirroring boost::leaf::leaf_detail::optional<T>, to verify correctness of TLS pointers
template <class T>
struct optional
{
    int key_;
    union { T value_; };
};
int const offset = offsetof(optional<my_error_info<1>>, value_);

int main()
{
    std::vector<void const *> used_ptrs;
    int r = leaf::try_handle_all(
        [&]() -> leaf::result<int>
        {
            BOOST_TEST_GE(min_tls_index, (BOOST_LEAF_CFG_TLS_ARRAY_START_INDEX));
            BOOST_TEST_GT(max_tls_index, min_tls_index);
            BOOST_TEST_LT(max_tls_index, (BOOST_LEAF_CFG_TLS_ARRAY_SIZE));
            for( int i=min_tls_index; i<=max_tls_index; ++i )
                used_ptrs.push_back((char const *)tls_storage[i] + offset);
            return leaf::new_error( my_error_info<1>{}, my_error_info<2>{} );
        },

        [&]( my_error_info<1> const & a, my_error_info<2> const & b )
        {
            BOOST_TEST(std::find(used_ptrs.begin(), used_ptrs.end(), &a) != used_ptrs.end());
            BOOST_TEST(std::find(used_ptrs.begin(), used_ptrs.end(), &b) != used_ptrs.end());
            return 1;
        },

        []
        {
            return 2;
        } );
    BOOST_TEST_EQ(r, 1);
    return boost::report_errors();
}
