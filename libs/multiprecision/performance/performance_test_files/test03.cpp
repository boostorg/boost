///////////////////////////////////////////////////////////////
//  Copyright 2019 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../performance_test.hpp"
#if defined(TEST_MPF)
#include <boost/multiprecision/gmp.hpp>
#endif

void test03()
{
#ifdef TEST_MPF
   test<boost::multiprecision::mpf_float_100>("gmp_float", 100);
#endif
}
