//  Copyright 2020 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include <iostream>
#include <vector>
#include <benchmark/benchmark.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/gmp.hpp>

#include <boost/math/special_functions/prime.hpp>
#include <boost/math/special_functions/pow.hpp>

#include <gmpxx.h>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int.hpp>

template <class T>
T generate_random(unsigned bits_wanted)
{
   static boost::random::mt19937               gen;
   typedef boost::random::mt19937::result_type random_type;

   T        max_val;
   unsigned digits;
   if (std::numeric_limits<T>::is_bounded && (bits_wanted == (unsigned)std::numeric_limits<T>::digits))
   {
      max_val = (std::numeric_limits<T>::max)();
      digits  = std::numeric_limits<T>::digits;
   }
   else
   {
      max_val = T(1) << bits_wanted;
      digits  = bits_wanted;
   }

   unsigned bits_per_r_val = std::numeric_limits<random_type>::digits - 1;
   while ((random_type(1) << bits_per_r_val) > (gen.max)())
      --bits_per_r_val;

   unsigned terms_needed = digits / bits_per_r_val + 1;

   T val = 0;
   for (unsigned i = 0; i < terms_needed; ++i)
   {
      val *= (gen.max)();
      val += gen();
   }
   val %= max_val;
   return val;
}

template <class T>
const std::vector<std::vector<T> >& get_matrix_data(unsigned bits);

template <>
const std::vector<std::vector<boost::multiprecision::cpp_rational> >& get_matrix_data(unsigned bits)
{
   static std::map<unsigned, std::vector<std::vector<boost::multiprecision::cpp_rational> > > data;
   if (data[bits].size() == 0)
   {
      for (unsigned i = 0; i < 100; ++i)
      {
         std::vector<boost::multiprecision::cpp_rational> matrix;
         for (unsigned j = 0; j < 9; ++j)
         {
            boost::multiprecision::cpp_int a(generate_random<boost::multiprecision::cpp_int>(bits)), b(generate_random<boost::multiprecision::cpp_int>(bits));
            matrix.push_back(boost::multiprecision::cpp_rational(a, b));
         }
         data[bits].push_back(matrix);
      }
   }
   return data[bits];
}

template <class T>
const std::vector<std::vector<T> >& get_matrix_data(unsigned bits)
{
   static std::map<unsigned, std::vector<std::vector<T> > > data;
   if (data[bits].empty())
   {
      const std::vector<std::vector<boost::multiprecision::cpp_rational> >& d = get_matrix_data<boost::multiprecision::cpp_rational>(bits);
      for (unsigned i = 0; i < 100; ++i)
      {
         std::vector<T> matrix;
         for (unsigned j = 0; j < 9; ++j)
         {
            matrix.push_back(T(d[i][j].str()));
         }
         data[bits].push_back(matrix);
      }
   }
   return data[bits];
}

template <class T>
T determinant(const std::vector<T>& data)
{
   const T m01 = data[0] * data[4] - data[3] * data[1];
   const T m02 = data[0] * data[7] - data[6] * data[1];
   const T m12 = data[3] * data[7] - data[6] * data[4];
   return m01 * data[8] - m02 * data[5] + m12 * data[2];
}

template <class Rational>
static void BM_determinant(benchmark::State& state)
{
   int                         bits = state.range(0);
   const std::vector<std::vector<Rational> >& data = get_matrix_data<Rational>(bits);
   for (auto _ : state)
   {
      for(unsigned i = 0; i < data.size(); ++i)
         benchmark::DoNotOptimize(determinant(data[i]));
   }
}


constexpr unsigned lower_range = 512;
constexpr unsigned upper_range = 1 << 15;

BENCHMARK_TEMPLATE(BM_determinant, boost::multiprecision::cpp_rational)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_determinant, boost::multiprecision::mpq_rational)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond);
BENCHMARK_TEMPLATE(BM_determinant, mpq_class)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
