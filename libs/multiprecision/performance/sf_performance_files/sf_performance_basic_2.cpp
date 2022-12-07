///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void basic_tests_2()
{
#ifdef TEST_MPFR
   basic_allocation_test("mpfr_float_50 - no expression templates", number<mpfr_float_backend<50>, et_off>(2));
#endif
}
