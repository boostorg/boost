//  Copyright 2021 John Maddock. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

#include <iostream>
#include <benchmark/benchmark.h>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/multiprecision/mpfr.hpp>
#include <boost/random.hpp>
#include <boost/math/special_functions/ellint_2.hpp>
#include <cmath>


using namespace boost::multiprecision;
using namespace boost::random;

unsigned allocation_count = 0;

void* operator new(std::size_t count)
{
   ++allocation_count;
   return std::malloc(count);
}
void* operator new[](std::size_t count)
{
   ++allocation_count;
   return std::malloc(count);
}
void operator delete(void* ptr)noexcept
{
   std::free(ptr);
}
void operator delete[](void* ptr) noexcept
{
   std::free(ptr);
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
         b.push_back(ui(mt));
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

using namespace boost::multiprecision;

//[dot_prod_1
cpp_int dot_product_1(const std::vector<cpp_int>& v1, const std::vector<cpp_int>& v2)
{
   if (v1.size() != v2.size())
      throw std::domain_error("Mismatched arguments");

   cpp_int result;
   for (std::size_t i = 0; i < v1.size(); ++i)
      result += v1[i] * v2[i];
   //
   // Named-return value optimisation (even better than a move):
   //
   return result;
}
//]
//[dot_prod_2
cpp_int dot_product_2(const std::vector<cpp_int>& v1, const std::vector<cpp_int>& v2)
{
   if (v1.size() != v2.size())
      throw std::domain_error("Mismatched arguments");
   //
   // If we know that most of our data is of a certain range of values, then we can use a cpp_int type
   // with an internal cache large enough to *probably* not require an allocation:
   //
   number<cpp_int_backend<1024> > result;
   for (std::size_t i = 0; i < v1.size(); ++i)
      result += v1[i] * v2[i];
   //
   // We can't rely on the named-return-value optimisation here, since the variable being returned
   // is a different type to the return value.  However, since these are "equivalent" types we
   // can move the result to the return value and get all the expected move-optimisations should
   // variable result have dynamically allocated:
   //
   return std::move(result);
}
//]
//[dot_prod_3
cpp_int dot_product_3(const std::vector<cpp_int>& v1, const std::vector<cpp_int>& v2)
{
   if (v1.size() != v2.size())
      throw std::domain_error("Mismatched arguments");

   cpp_int result, term;
   for (std::size_t i = 0; i < v1.size(); ++i)
   {
      //
      // Re-use the same variable for the result of the multiplications, rather than rely on 
      // an internally generated temporary.  Depending on the input data, this may allocate
      // a few times depending how soon in the input vector's we encounter the largest values.
      // In the best case though, or for fairly uniformly sized input data, we will allocate 
      // only once:
      //
      term = v1[i] * v2[i];
      result += term;
   }
   //
   // Named-return value optimisation (even better than a move):
   //
   return result;
}
//]

template <typename T>
static void BM_dot_product_1(benchmark::State& state)
{
   int bits = state.range(0);

   std::vector<T>& a = get_test_vector_a<T>(bits);
   std::vector<T>& b = get_test_vector_b<T>(bits);

   static int done = 0;
   if (done != bits)
   {
      allocation_count = 0;
      benchmark::DoNotOptimize(dot_product_1(a, b));
      std::cout << allocation_count << std::endl;
      done = bits;
   }

   for (auto _ : state)
   {
      benchmark::DoNotOptimize(dot_product_1(a, b));
   }
   state.SetComplexityN(bits);
}
template <typename T>
static void BM_dot_product_2(benchmark::State& state)
{
   int                         bits = state.range(0);

   std::vector<T>& a = get_test_vector_a<T>(bits);
   std::vector<T>& b = get_test_vector_b<T>(bits);

   static int done = 0;
   if (done != bits)
   {
      allocation_count = 0;
      benchmark::DoNotOptimize(dot_product_2(a, b));
      std::cout << allocation_count << std::endl;
      done = bits;
   }

   for (auto _ : state)
   {
      benchmark::DoNotOptimize(dot_product_2(a, b));
   }
   state.SetComplexityN(bits);
}
template <typename T>
static void BM_dot_product_3(benchmark::State& state)
{
   int                         bits = state.range(0);

   std::vector<T>& a = get_test_vector_a<T>(bits);
   std::vector<T>& b = get_test_vector_b<T>(bits);

   static int done = 0;
   if (done != bits)
   {
      allocation_count = 0;
      benchmark::DoNotOptimize(dot_product_3(a, b));
      std::cout << allocation_count << std::endl;
      done = bits;
   }

   for (auto _ : state)
   {
      benchmark::DoNotOptimize(dot_product_3(a, b));
   }
   state.SetComplexityN(bits);
}
//[elliptic_arc1
template <unsigned N>
number<mpfr_float_backend<N> > elliptic_arc_length_1(const number<mpfr_float_backend<N> >& a, const number<mpfr_float_backend<N> >& b)
{
   number<mpfr_float_backend<N> > k = sqrt(1 - b * b / (a * a));
   return 4 * a * boost::math::ellint_2(k);
}
//]
//[elliptic_arc2
template <unsigned N>
number<mpfr_float_backend<N> > elliptic_arc_length_2(const number<mpfr_float_backend<N> >& a, const number<mpfr_float_backend<N> >& b)
{
   number<mpfr_float_backend<N, allocate_stack> > k = sqrt(1 - b * b / (a * a));
   return 4 * a * boost::math::ellint_2(k);
}
//]

template <typename T>
static void BM_elliptic_1(benchmark::State& state)
{
   T a = 25.25 * boost::math::constants::e<T>();
   T b = 3.75 * boost::math::constants::e<T>();

   //std::cout << elliptic_arc_length_1(a, b) << std::endl;

   for (auto _ : state)
   {
      benchmark::DoNotOptimize(elliptic_arc_length_1(a, b));
   }
}
template <typename T>
static void BM_elliptic_2(benchmark::State& state)
{
   T a = 25.25 * boost::math::constants::e<T>();
   T b = 3.75 * boost::math::constants::e<T>();

   //std::cout << elliptic_arc_length_2(a, b) << std::endl;

   for (auto _ : state)
   {
      benchmark::DoNotOptimize(elliptic_arc_length_2(a, b));
   }
}

constexpr unsigned lower_range = 32;
constexpr unsigned upper_range = 1024;

BENCHMARK_TEMPLATE(BM_dot_product_1, cpp_int)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond)->Complexity();
BENCHMARK_TEMPLATE(BM_dot_product_1, cpp_int)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond)->Complexity();
BENCHMARK_TEMPLATE(BM_dot_product_2, cpp_int)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond)->Complexity();
BENCHMARK_TEMPLATE(BM_dot_product_3, cpp_int)->RangeMultiplier(2)->Range(lower_range, upper_range)->Unit(benchmark::kMillisecond)->Complexity();

BENCHMARK_TEMPLATE(BM_elliptic_1, number<mpfr_float_backend<30> >);
BENCHMARK_TEMPLATE(BM_elliptic_1, number<mpfr_float_backend<40> >);
BENCHMARK_TEMPLATE(BM_elliptic_1, number<mpfr_float_backend<50> >);
BENCHMARK_TEMPLATE(BM_elliptic_1, number<mpfr_float_backend<60> >);
BENCHMARK_TEMPLATE(BM_elliptic_1, number<mpfr_float_backend<70> >);
BENCHMARK_TEMPLATE(BM_elliptic_1, number<mpfr_float_backend<80> >);

BENCHMARK_TEMPLATE(BM_elliptic_2, number<mpfr_float_backend<30> >);
BENCHMARK_TEMPLATE(BM_elliptic_2, number<mpfr_float_backend<40> >);
BENCHMARK_TEMPLATE(BM_elliptic_2, number<mpfr_float_backend<50> >);
BENCHMARK_TEMPLATE(BM_elliptic_2, number<mpfr_float_backend<60> >);
BENCHMARK_TEMPLATE(BM_elliptic_2, number<mpfr_float_backend<70> >);
BENCHMARK_TEMPLATE(BM_elliptic_2, number<mpfr_float_backend<80> >);

BENCHMARK_MAIN();
