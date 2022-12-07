///////////////////////////////////////////////////////////////
//  Copyright 2019 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../performance_test.hpp"
#if defined(TEST_CPP_INT)
#include <boost/multiprecision/cpp_int.hpp>
#endif

void test41()
{
#if defined(TEST_CPP_INT) && defined(TEST_KARATSUBA)
   test<boost::multiprecision::cpp_int>("cpp_int", (boost::multiprecision::backends::karatsuba_cutoff + 2) * sizeof(boost::multiprecision::limb_type) * CHAR_BIT);
#endif
}
