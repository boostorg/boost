///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void bessel_tests_10()
{
#ifdef TEST_MPREAL
   time_proc("Bessel Functions (50 digit precision)", "mpfr::mpreal", test_bessel<mpfr::mpreal>, mpfr_buildopt_tls_p() ? 3 : 1);
#endif
}
