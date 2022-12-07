///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void nct_tests_01()
{
#ifdef TEST_MPFR
   time_proc("Non-central T Distribution (50 digit precision)", "mpfr_float_50", test_nct<mpfr_float_50>);
#endif
}
