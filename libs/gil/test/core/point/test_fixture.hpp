//
// Copyright 2022 Marco Langer <langer.m86@gmail.com>
//
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/gil/point.hpp>

#include <ostream>

namespace boost { namespace gil {

// necessary to use point directly with boost test macros
template <typename T>
auto operator<<(std::ostream& os, point<T> const& p) -> std::ostream&
{
    return os << "{x=" << p.x << ", y=" << p.y << "}";
}

}} // namespace boost::gil
