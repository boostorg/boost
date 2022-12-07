///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void poly_tests_11()
{
#ifdef TEST_MPFR
   time_proc("Polynomial Evaluation (50 digit precision)", "mpfr_float_100 (no expression templates", test_polynomial<number<mpfr_float_backend<100>, et_off> >);
#endif
}
