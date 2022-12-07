///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void bessel_tests_01()
{
#ifdef TEST_MPFR
#if MPFR_VERSION < MPFR_VERSION_NUM(3, 0, 0)
   time_proc("Bessel Functions (50 digit precision)", "mpfr_float_50", test_bessel<boost::multiprecision::mpfr_float_50>, 1);
#else
   time_proc("Bessel Functions (50 digit precision)", "mpfr_float_50", test_bessel<boost::multiprecision::mpfr_float_50>, mpfr_buildopt_tls_p() ? 3 : 1);
#endif
#endif
}
