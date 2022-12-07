// Copyright 2019 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

#include <boost/atomic.hpp>

struct X
{
    double v[ 8 ];
};

int main()
{
    boost::atomic<X> a;
    a.store( X() );
}
