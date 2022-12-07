//  Copyright 2020 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include <iostream>
#include <benchmark/benchmark.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/gmp.hpp>

#include <gmpxx.h>

template <class Integer>
inline Integer factorial(unsigned n)
{
   Integer result = 1;
   for (unsigned k = 1; k <= n; ++k)
      result *= k;
   return result;
}

template <class Rational, class Integer = typename Rational::value_type>
inline Rational binomial(unsigned n, unsigned k)
{
   return Rational(factorial<Integer>(n), factorial<Integer>(k) * factorial<Integer>(n - k));
}

inline mpz_class pow(mpz_class i, unsigned p)
{
   mpz_class result;
   mpz_pow_ui(result.get_mpz_t(), i.get_mpz_t(), p);
   return result;
}

template <class Rational, class Integer = typename Rational::value_type>
Rational Bernoulli(unsigned m)
{
   Rational result = 0;

   for (unsigned k = 0; k <= m; ++k)
   {
      Rational inner = 0;
      for (unsigned v = 0; v <= k; ++v)
      {
         Rational term = binomial<Rational, Integer>(k, v) * Rational(pow(Integer(v), m), k + 1);
         if (v & 1)
            term = -term;
         inner += term;
      }
      result += inner;
   }
   return result;
}

template <class Rational, class Integer = typename Rational::value_type>
static void BM_bernoulli(benchmark::State& state)
{
   int m = state.range(0);
   for (auto _ : state)
   {
      benchmark::DoNotOptimize(Bernoulli<Rational, Integer>(m));
   }
}


BENCHMARK_TEMPLATE(BM_bernoulli, boost::multiprecision::cpp_rational)->DenseRange(50, 200, 4);
BENCHMARK_TEMPLATE(BM_bernoulli, boost::multiprecision::mpq_rational)->DenseRange(50, 200, 4);
BENCHMARK_TEMPLATE(BM_bernoulli, boost::multiprecision::number<boost::multiprecision::rational_adaptor<boost::multiprecision::gmp_int> >)->DenseRange(50, 200, 4);
BENCHMARK_TEMPLATE(BM_bernoulli, mpq_class, mpz_class)->DenseRange(50, 200, 4);

BENCHMARK_MAIN();
