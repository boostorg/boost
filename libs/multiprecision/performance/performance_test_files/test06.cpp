///////////////////////////////////////////////////////////////
//  Copyright 2019 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../performance_test.hpp"
#if defined(TEST_MPZ)
#include <boost/multiprecision/gmp.hpp>
#endif

void test06()
{
#ifdef TEST_MPZ
   test<boost::multiprecision::mpz_int>("gmp_int", 256);
#endif
}
