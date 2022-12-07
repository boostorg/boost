///////////////////////////////////////////////////////////////
//  Copyright 2019 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../performance_test.hpp"
#if defined(TEST_MPFR)
#include <boost/multiprecision/mpfr.hpp>
#endif

void test35()
{
#ifdef TEST_MPFR
   test<boost::multiprecision::mpfr_float_50>("mpfr_float", 50);
#endif
}
