///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void basic_tests_1()
{

   std::cout << "Allocation Counts for Horner Evaluation:\n";
#ifdef TEST_MPFR
   basic_allocation_test("mpfr_float_50", mpfr_float_50(2));
#endif
}
