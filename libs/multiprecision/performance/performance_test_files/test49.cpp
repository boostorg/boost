///////////////////////////////////////////////////////////////
//  Copyright 2019 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../performance_test.hpp"
#if defined(TEST_TOMMATH)
#include <boost/multiprecision/tommath.hpp>
#endif

void test49()
{
#ifdef TEST_TOMMATH
   test<boost::multiprecision::tom_int>("tommath_int", 1024*16);
   /*
   //
   // These are actually too slow to test!!!
   //
   test<boost::multiprecision::tom_rational>("tom_rational", 128);
   test<boost::multiprecision::tom_rational>("tom_rational", 256);
   test<boost::multiprecision::tom_rational>("tom_rational", 512);
   test<boost::multiprecision::tom_rational>("tom_rational", 1024);
   */
#endif
}
