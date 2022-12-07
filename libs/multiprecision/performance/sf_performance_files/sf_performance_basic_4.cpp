///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../sf_performance.hpp"

void basic_tests_4()
{
#ifdef TEST_MPREAL
   basic_allocation_test("mpfr::mpreal", mpfr::mpreal(2));
#endif
}
