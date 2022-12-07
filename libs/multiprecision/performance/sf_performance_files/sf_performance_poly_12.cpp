///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void poly_tests_12()
{
#ifdef TEST_MPFR
   time_proc("Polynomial Evaluation (100 digit precision)", "static_mpfr_float_100", test_polynomial<static_mpfr_float_100>);
#endif
}
