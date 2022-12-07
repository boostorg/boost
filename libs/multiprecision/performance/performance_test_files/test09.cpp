///////////////////////////////////////////////////////////////
//  Copyright 2019 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "../performance_test.hpp"
#if defined(TEST_CPP_INT)
#include <boost/multiprecision/cpp_int.hpp>
#endif

void test09()
{
#ifdef TEST_CPP_INT
   //test<boost::multiprecision::number<boost::multiprecision::cpp_int_backend<64, 64, boost::multiprecision::unsigned_magnitude, boost::multiprecision::unchecked, void>, boost::multiprecision::et_off> >("cpp_int(unsigned, fixed)", 64);
   //test<boost::multiprecision::number<boost::multiprecision::cpp_int_backend<64, 64, boost::multiprecision::signed_magnitude, boost::multiprecision::unchecked, void>, boost::multiprecision::et_off> >("cpp_int(fixed)", 64);
   test<boost::multiprecision::number<boost::multiprecision::cpp_int_backend<128, 128, boost::multiprecision::signed_magnitude, boost::multiprecision::unchecked, void>, boost::multiprecision::et_off> >("cpp_int(fixed)", 128);
#endif
}
