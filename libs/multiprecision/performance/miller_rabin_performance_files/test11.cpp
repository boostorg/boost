///////////////////////////////////////////////////////////////
//  Copyright 2019 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../miller_rabin_performance.hpp"

void test11()
{
   using namespace boost::multiprecision;
#ifdef TEST_TOMMATH
   test_miller_rabin<number<boost::multiprecision::tommath_int, et_off> >("tom_int (no Expression templates)");
#endif
}
