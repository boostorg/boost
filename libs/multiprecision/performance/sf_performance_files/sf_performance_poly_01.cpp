///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void poly_tests_01()
{
#ifdef TEST_MPFR
   time_proc("Polynomial Evaluation (50 digit precision)", "mpfr_float_50", test_polynomial<mpfr_float_50>);
#endif
}
