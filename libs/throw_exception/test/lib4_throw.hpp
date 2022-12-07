#ifndef LIB4_THROW_HPP_INCLUDED
#define LIB4_THROW_HPP_INCLUDED

// Copyright 2018, 2022 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/config.hpp>
#include <exception>

#if defined(LIB4_DYN_LINK)
# if defined(LIB4_SOURCE)
#  define LIB4_DECL BOOST_SYMBOL_EXPORT
# else
#  define LIB4_DECL BOOST_SYMBOL_IMPORT
# endif
#else
# define LIB4_DECL
#endif

namespace lib4
{

struct BOOST_SYMBOL_VISIBLE exception: public std::exception
{
};

LIB4_DECL void f();

} // namespace lib4

#endif // #ifndef LIB4_THROW_HPP_INCLUDED
