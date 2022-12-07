///////////////////////////////////////////////////////////////
//  Copyright 2019 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../miller_rabin_performance.hpp"

void test10()
{
   using namespace boost::multiprecision;
#ifdef TEST_MPZ
   test_miller_rabin<mpz_int>("mpz_int");
   std::cout << "Time for mpz_int (native Miller Rabin Test) = " << test_miller_rabin_gmp() << std::endl;
#endif
}
