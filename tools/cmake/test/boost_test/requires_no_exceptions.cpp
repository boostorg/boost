// Copyright 2019 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

#include <boost/config.hpp>

int main()
{
#if defined(BOOST_NO_EXCEPTIONS)

    return 0;

#else

    return 1;

#endif
}
