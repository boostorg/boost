///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void nct_tests_05()
{
#ifdef TEST_MPF
   time_proc("Non-central T Distribution (50 digit precision)", "mpf_float_50 (no expression templates", test_nct<number<gmp_float<50>, et_off> >);
#endif
}
