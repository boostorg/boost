//  Copyright 2020 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include <iostream>
#include <benchmark/benchmark.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/gmp.hpp>
#include <boost/multiprecision/integer.hpp>
#include <boost/random.hpp>
#include <cmath>

#include <immintrin.h>

using namespace boost::multiprecision;
using namespace boost::random;

template <class Integer>
BOOST_MP_CXX14_CONSTEXPR Integer sqrt_old(const Integer& x, Integer& r)
{
   //
   // This is slow bit-by-bit integer square root, see for example
   // http://en.wikipedia.org/wiki/Methods_of_computing_square_roots#Binary_numeral_system_.28base_2.29
   // There are better methods such as http://hal.inria.fr/docs/00/07/28/54/PDF/RR-3805.pdf
   // and http://hal.inria.fr/docs/00/07/21/13/PDF/RR-4475.pdf which should be implemented
   // at some point.
   //
   Integer s = 0;
   if (x == 0)
   {
      r = 0;
      return s;
   }
   int g = msb(x);
   if (g == 0)
   {
      r = 1;
      return s;
   }

   Integer t = 0;
   r         = x;
   g /= 2;
   bit_set(s, g);
   bit_set(t, 2 * g);
   r = x - t;
   --g;
   do
   {
      t = s;
      t <<= g + 1;
      bit_set(t, 2 * g);
      if (t <= r)
      {
         bit_set(s, g);
         r -= t;
      }
      --g;
   } while (g >= 0);
   return s;
}

template <class Integer>
BOOST_MP_CXX14_CONSTEXPR Integer sqrt_old(const Integer& x)
{
   Integer r(0);
   return sqrt_old(x, r);
}

template <class T>
std::tuple<std::vector<T>, std::vector<T> >& get_test_vector(unsigned bits)
{
   static std::map<unsigned, std::tuple<std::vector<T>, std::vector<T> > > data;

   std::tuple<std::vector<T>, std::vector<T> >& result = data[bits];

   if (std::get<0>(result).size() == 0)
   {
      mt19937                     mt;
      uniform_int_distribution<T> ui(T(1) << (bits - 1), T(1) << bits);

      std::vector<T>& a = std::get<0>(result);
      std::vector<T>& b = std::get<1>(result);

      for (unsigned i = 0; i < 1000; ++i)
      {
         a.push_back(ui(mt));
         b.push_back(0);
      }
   }
   return result;
}

template <class T>
std::vector<T>& get_test_vector_a(unsigned bits)
{
   return std::get<0>(get_test_vector<T>(bits));
}
template <class T>
std::vector<T>& get_test_vector_b(unsigned bits)
{
   return std::get<1>(get_test_vector<T>(bits));
}

template <typename T>
static void BM_sqrt_old(benchmark::State& state)
{
   int                         bits = state.range(0);

   std::vector<T>& a = get_test_vector_a<T>(bits);
   std::vector<T>& b = get_test_vector_b<T>(bits);

   for (auto _ : state)
   {
      for (unsigned i = 0; i < a.size(); ++i)
         b[i] = sqrt_old(a[i]);
   }
   state.SetComplexityN(bits);
}

template <typename T>
static void BM_sqrt_current(benchmark::State& state)
{
   int bits = state.range(0);

   std::vector<T>& a = get_test_vector_a<T>(bits);
   std::vector<T>& b = get_test_vector_b<T>(bits);

   for (auto _ : state)
   {
      for (unsigned i = 0; i < a.size(); ++i)
         b[i] = sqrt(a[i]);
   }
   state.SetComplexityN(bits);
}

constexpr unsigned lower_range = 512;
constexpr unsigned upper_range = 1 << 15;

BENCHMARK_TEMPLATE(BM_sqrt_old, cpp_int)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond)->Complexity();
BENCHMARK_TEMPLATE(BM_sqrt_current, cpp_int)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond)->Complexity();
BENCHMARK_TEMPLATE(BM_sqrt_current, mpz_int)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond)->Complexity();

BENCHMARK_MAIN();
