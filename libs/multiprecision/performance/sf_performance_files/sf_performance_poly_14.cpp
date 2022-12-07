///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void poly_tests_14()
{
#ifdef TEST_MPF
   time_proc("Polynomial Evaluation (100 digit precision)", "mpf_float_100 (no expression templates", test_polynomial<number<gmp_float<100>, et_off> >);
#endif
}
