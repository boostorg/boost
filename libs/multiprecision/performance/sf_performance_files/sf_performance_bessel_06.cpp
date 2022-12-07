///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void bessel_tests_06()
{
#ifdef TEST_MPF
   time_proc("Bessel Functions (50 digit precision)", "mpf_float_50 (no expression templates", test_bessel<number<gmp_float<50>, et_off> >, 3);
#endif
}
