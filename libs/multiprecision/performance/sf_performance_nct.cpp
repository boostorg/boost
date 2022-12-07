///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "sf_performance.hpp"

void nct_tests_01();
void nct_tests_02();
void nct_tests_03();
void nct_tests_04();
void nct_tests_05();
void nct_tests_06();
void nct_tests_07();
void nct_tests_08();
void nct_tests_09();
void nct_tests_10();
void nct_tests_11();
void nct_tests_12();
void nct_tests_13();
void nct_tests_14();
void nct_tests_15();
void nct_tests_16();
void nct_tests_17();
void nct_tests_18();
void nct_tests_19();
void nct_tests_20();

void nct_tests()
{
   //
   // 50 digits first:
   //
   std::cout << "Testing Non-Central T at 50 digits....." << std::endl;
#ifdef TEST_MPFR_CLASS
   mpfr_set_default_prec(50 * 1000L / 301L);
#endif

   nct_tests_01();
   nct_tests_02();
   nct_tests_03();
   nct_tests_04();
   nct_tests_05();
   nct_tests_06();
   nct_tests_07();
   nct_tests_08();
   nct_tests_09();
   nct_tests_10();

   //
   // Then 100 digits:
   //
   std::cout << "Testing Non-Central T at 100 digits....." << std::endl;
#ifdef TEST_MPFR_CLASS
   mpfr_set_default_prec(100 * 1000L / 301L);
#endif
   nct_tests_11();
   nct_tests_12();
   nct_tests_13();
   nct_tests_14();
   nct_tests_15();
   nct_tests_16();
   nct_tests_17();
   nct_tests_18();
   nct_tests_19();
   nct_tests_20();
}
