//  Copyright 2020 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include <iostream>
#include <benchmark/benchmark.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/gmp.hpp>

#include <boost/math/special_functions/prime.hpp>
#include <boost/math/special_functions/pow.hpp>

#include <gmpxx.h>

template <class Rational, class Integer = typename Rational::value_type>
Rational zeta18()
{
   Rational result = 1;

   for (unsigned i = 0; i < 10; ++i)
   {
      result /= 1 - Rational(1, boost::math::pow<18>(Integer(boost::math::prime(i))));
   }
   return result;
}

template <class Rational, class Integer = typename Rational::value_type>
static void BM_zeta18(benchmark::State& state)
{
   for (auto _ : state)
   {
      benchmark::DoNotOptimize(zeta18<Rational, Integer>());
   }
}


BENCHMARK_TEMPLATE(BM_zeta18, boost::multiprecision::cpp_rational);
BENCHMARK_TEMPLATE(BM_zeta18, boost::multiprecision::mpq_rational);
BENCHMARK_TEMPLATE(BM_zeta18, boost::multiprecision::number<boost::multiprecision::rational_adaptor<boost::multiprecision::gmp_int>>);
BENCHMARK_TEMPLATE(BM_zeta18, mpq_class, mpz_class);

BENCHMARK_MAIN();
