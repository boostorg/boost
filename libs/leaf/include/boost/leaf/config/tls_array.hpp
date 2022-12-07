#ifndef BOOST_LEAF_CONFIG_TLS_ARRAY_HPP_INCLUDED
#define BOOST_LEAF_CONFIG_TLS_ARRAY_HPP_INCLUDED

// Copyright 2018-2022 Emil Dotchevski and Reverge Studios, Inc.
// Copyright (c) 2022 Khalil Estell

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// LEAF requires thread local storage support for pointers and for uin32_t values.

// This header implements thread local storage for pointers and for uint32_t
// values for platforms that support thread local pointers by index.

namespace boost { namespace leaf {

namespace tls
{
    // The TLS support defined in this header requires the following two
    // functions to be defined elsewhere:
    void * read_void_ptr( int tls_index ) noexcept;
    void write_void_ptr( int tls_index, void * ) noexcept;
}

} }

////////////////////////////////////////

#include <limits>
#include <atomic>
#include <cstdint>
#include <type_traits>

#ifndef BOOST_LEAF_CFG_TLS_INDEX_TYPE
#   define BOOST_LEAF_CFG_TLS_INDEX_TYPE unsigned char
#endif

#ifndef BOOST_LEAF_CFG_TLS_ARRAY_START_INDEX
#   define BOOST_LEAF_CFG_TLS_ARRAY_START_INDEX 0
#endif

static_assert((BOOST_LEAF_CFG_TLS_ARRAY_START_INDEX) >= 0,
    "Bad BOOST_LEAF_CFG_TLS_ARRAY_START_INDEX");

#ifdef BOOST_LEAF_CFG_TLS_ARRAY_SIZE
    static_assert((BOOST_LEAF_CFG_TLS_ARRAY_SIZE) > (BOOST_LEAF_CFG_TLS_ARRAY_START_INDEX),
        "Bad BOOST_LEAF_CFG_TLS_ARRAY_SIZE");
    static_assert((BOOST_LEAF_CFG_TLS_ARRAY_SIZE) - 1 <= std::numeric_limits<BOOST_LEAF_CFG_TLS_INDEX_TYPE>::max(),
        "Bad BOOST_LEAF_CFG_TLS_ARRAY_SIZE");
#endif

////////////////////////////////////////

namespace boost { namespace leaf {

namespace leaf_detail
{
    using atomic_unsigned_int = std::atomic<unsigned int>;
}

namespace tls
{
    template <class=void>
    class BOOST_LEAF_SYMBOL_VISIBLE index_counter
    {
        static int c_;

    public:

        static BOOST_LEAF_CFG_TLS_INDEX_TYPE next()
        {
            int idx = ++c_;
            BOOST_LEAF_ASSERT(idx > (BOOST_LEAF_CFG_TLS_ARRAY_START_INDEX));
            BOOST_LEAF_ASSERT(idx < (BOOST_LEAF_CFG_TLS_ARRAY_SIZE));
            return idx;
        }
    };

    template <class T>
    struct BOOST_LEAF_SYMBOL_VISIBLE tls_index
    {
        static BOOST_LEAF_CFG_TLS_INDEX_TYPE idx;
    };

    template <class T>
    struct BOOST_LEAF_SYMBOL_VISIBLE alloc_tls_index
    {
        static BOOST_LEAF_CFG_TLS_INDEX_TYPE const idx;
    };

    template <class T>
    int index_counter<T>::c_ = BOOST_LEAF_CFG_TLS_ARRAY_START_INDEX;

    template <class T>
    BOOST_LEAF_CFG_TLS_INDEX_TYPE tls_index<T>::idx = BOOST_LEAF_CFG_TLS_ARRAY_START_INDEX;

    template <class T>
    BOOST_LEAF_CFG_TLS_INDEX_TYPE const alloc_tls_index<T>::idx = tls_index<T>::idx = index_counter<>::next();

    ////////////////////////////////////////

    template <class T>
    T * read_ptr() noexcept
    {
        int tls_idx = tls_index<T>::idx;
        if( tls_idx == (BOOST_LEAF_CFG_TLS_ARRAY_START_INDEX) )
            return nullptr;
        --tls_idx;
        return reinterpret_cast<T *>(read_void_ptr(tls_idx));
    }

    template <class T>
    void write_ptr( T * p ) noexcept
    {
        int tls_idx = alloc_tls_index<T>::idx;
        --tls_idx;
        write_void_ptr(tls_idx, p);
        BOOST_LEAF_ASSERT(read_void_ptr(tls_idx) == p);
    }

    ////////////////////////////////////////

    template <class Tag>
    std::uint32_t read_uint32() noexcept
    {
        static_assert(sizeof(std::intptr_t) >= sizeof(std::uint32_t), "Incompatible tls_array implementation");
        return (std::uint32_t) (std::intptr_t) (void *) read_ptr<Tag>();
    }

    template <class Tag>
    void write_uint32( std::uint32_t x ) noexcept
    {
        static_assert(sizeof(std::intptr_t) >= sizeof(std::uint32_t), "Incompatible tls_array implementation");
        write_ptr<Tag>((Tag *) (void *) (std::intptr_t) x);
    }

    template <class Tag>
    void uint32_increment() noexcept
    {
        write_uint32<Tag>(read_uint32<Tag>() + 1);
    }

    template <class Tag>
    void uint32_decrement() noexcept
    {
        write_uint32<Tag>(read_uint32<Tag>() - 1);
    }
}

} }

#endif
