///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void basic_tests_3()
{
#ifdef TEST_MPFR_CLASS
   basic_allocation_test("mpfr_class", mpfr_class(2));
#endif
}
