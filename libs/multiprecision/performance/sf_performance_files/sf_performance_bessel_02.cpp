///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void bessel_tests_02()
{
#ifdef TEST_MPFR
#if MPFR_VERSION < MPFR_VERSION_NUM(3, 0, 0)
   time_proc("Bessel Functions (50 digit precision)", "mpfr_float_50 (no expression templates)", test_bessel<number<mpfr_float_backend<50>, et_off> >, 1);
#else
   time_proc("Bessel Functions (50 digit precision)", "mpfr_float_50 (no expression templates", test_bessel<number<mpfr_float_backend<50>, et_off> >, mpfr_buildopt_tls_p() ? 3 : 1);
#endif
#endif
}
