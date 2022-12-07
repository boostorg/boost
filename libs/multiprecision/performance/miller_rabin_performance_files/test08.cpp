///////////////////////////////////////////////////////////////
//  Copyright 2019 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../miller_rabin_performance.hpp"

void test08()
{
   using namespace boost::multiprecision;
#ifdef TEST_CPP_INT
   test_miller_rabin<checked_uint1024_t>("checked_uint1024_t");
#endif
}
