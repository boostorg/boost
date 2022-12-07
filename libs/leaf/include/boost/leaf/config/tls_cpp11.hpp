#ifndef BOOST_LEAF_CONFIG_TLS_CPP11_HPP_INCLUDED
#define BOOST_LEAF_CONFIG_TLS_CPP11_HPP_INCLUDED

// Copyright 2018-2022 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// LEAF requires thread local storage support for pointers and for uin32_t values.

// This header implements thread local storage for pointers and for uint32_t
// values using the C++11 built-in thread_local storage class specifier.

#include <cstdint>
#include <atomic>

namespace boost { namespace leaf {

namespace leaf_detail
{
    using atomic_unsigned_int = std::atomic<unsigned int>;
}

namespace tls
{
    template <class T>
    struct BOOST_LEAF_SYMBOL_VISIBLE ptr
    {
        static thread_local T * p;
    };

    template <class T>
    thread_local T * ptr<T>::p;

    template <class T>
    T * read_ptr() noexcept
    {
        return ptr<T>::p;
    }

    template <class T>
    void write_ptr( T * p ) noexcept
    {
        ptr<T>::p = p;
    }

    ////////////////////////////////////////

    template <class Tag>
    struct BOOST_LEAF_SYMBOL_VISIBLE tagged_uint32
    {
        static thread_local std::uint32_t x;
    };

    template <class Tag>
    thread_local std::uint32_t tagged_uint32<Tag>::x;

    template <class Tag>
    std::uint32_t read_uint32() noexcept
    {
        return tagged_uint32<Tag>::x;
    }

    template <class Tag>
    void write_uint32( std::uint32_t x ) noexcept
    {
        tagged_uint32<Tag>::x = x;
    }

    template <class Tag>
    void uint32_increment() noexcept
    {
        ++tagged_uint32<Tag>::x;
    }

    template <class Tag>
    void uint32_decrement() noexcept
    {
        --tagged_uint32<Tag>::x;
    }
}

} }

#endif
