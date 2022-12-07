// Copyright 2018, 2022 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
//
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include "lib4_throw.hpp"
#include <boost/throw_exception.hpp>

void lib4::f()
{
    boost::throw_with_location( lib4::exception() );
}
