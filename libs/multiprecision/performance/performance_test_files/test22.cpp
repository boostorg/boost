///////////////////////////////////////////////////////////////
//  Copyright 2019 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../performance_test.hpp"
#if defined(TEST_MPQ)
#include <boost/multiprecision/gmp.hpp>
#endif

void test22()
{
#ifdef TEST_MPQ
   test<boost::multiprecision::mpq_rational>("mpq_rational", 256);
#endif
}
