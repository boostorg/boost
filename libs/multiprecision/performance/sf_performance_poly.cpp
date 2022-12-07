///////////////////////////////////////////////////////////////
//  Copyright 2011 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include "sf_performance.hpp"

void poly_tests_01();
void poly_tests_02();
void poly_tests_03();
void poly_tests_04();
void poly_tests_05();
void poly_tests_06();
void poly_tests_07();
void poly_tests_08();
void poly_tests_09();
void poly_tests_10();
void poly_tests_11();
void poly_tests_12();
void poly_tests_13();
void poly_tests_14();
void poly_tests_15();
void poly_tests_16();
void poly_tests_17();
void poly_tests_18();

void poly_tests()
{
   //
   // 50 digits first:
   //
   std::cout << "Testing Polynomial Evaluation at 50 digits....." << std::endl;
#if defined(TEST_MPFR) || defined(TEST_MPFR_CLASS)
   mpfr_set_default_prec(50 * 1000L / 301L);
#endif
#ifdef TEST_MPREAL
   mpfr::mpreal::set_default_prec(50 * 1000L / 301L);
#endif
   poly_tests_01();
   poly_tests_02();
   poly_tests_03();
   poly_tests_04();
   poly_tests_05();
   poly_tests_06();
   poly_tests_07();
   poly_tests_08();
   poly_tests_09();
   //
   // Then 100 digits:
   //
   std::cout << "Testing Polynomial Evaluation at 100 digits....." << std::endl;
#ifdef TEST_MPFR_CLASS
   mpfr_set_default_prec(100 * 1000L / 301L);
#endif
#ifdef TEST_MPREAL
   mpfr::mpreal::set_default_prec(100 * 1000L / 301L);
#endif
   poly_tests_10();
   poly_tests_11();
   poly_tests_12();
   poly_tests_13();
   poly_tests_14();
   poly_tests_15();
   poly_tests_16();
   poly_tests_17();
   poly_tests_18();
}
