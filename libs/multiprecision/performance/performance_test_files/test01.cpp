///////////////////////////////////////////////////////////////
//  Copyright 2019 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../performance_test.hpp"

void test01()
{
#ifdef TEST_INT64
   test<std::uint64_t>("std::uint64_t", 64);
#endif
}
