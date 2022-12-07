///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void basic_tests_5()
{
   std::cout << "Allocation Counts for boost::math::tools::evaluate_polynomial:\n";
#ifdef TEST_MPFR
   poly_allocation_test("mpfr_float_50", mpfr_float_50(2));
#endif
}
