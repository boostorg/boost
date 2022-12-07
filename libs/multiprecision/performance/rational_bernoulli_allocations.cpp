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

unsigned allocation_count = 0;

void* (*alloc_func_ptr)(size_t);
void* (*realloc_func_ptr)(void*, size_t, size_t);
void (*free_func_ptr)(void*, size_t);

void* alloc_func(size_t n)
{
   ++allocation_count;
   return (*alloc_func_ptr)(n);
}

void free_func(void* p, size_t n)
{
   (*free_func_ptr)(p, n);
}

void* realloc_func(void* p, size_t old, size_t n)
{
   ++allocation_count;
   return (*realloc_func_ptr)(p, old, n);
}

unsigned new_count = 0;

void* operator new(std::size_t n) throw(std::bad_alloc)
{
   ++new_count;
   return std::malloc(n);
}
void operator delete(void* p) throw()
{
   std::free(p);
}

void* operator new[](std::size_t n) throw(std::bad_alloc)
{
   ++new_count;
   return std::malloc(n);
}
void operator delete[](void* p) throw()
{
   std::free(p);
}


int main()
{
   using namespace boost::multiprecision;

   mp_get_memory_functions(&alloc_func_ptr, &realloc_func_ptr, &free_func_ptr);
   mp_set_memory_functions(&alloc_func, &realloc_func, &free_func);

   std::cout << "[table Total Allocation Counts for Bernoulli Number Calculation\n"
      "[[m][cpp_rational][mpq_rational][number<rational_adaptor<gmp_int>>][mpq_class]]\n";

   for (unsigned m = 2; m < 200; m += 2)
   {
      std::cout << "[[" << m << "][";
      new_count = 0;
      Bernoulli<cpp_rational>(m);
      std::cout << new_count << "][";
      allocation_count = 0;
      Bernoulli<mpq_rational>(m);
      std::cout << allocation_count << "][";
      allocation_count = 0;
      Bernoulli<number<rational_adaptor<gmp_int>>>(m);
      std::cout << allocation_count << "][";
      allocation_count = 0;
      Bernoulli<mpq_class, mpz_class>(m);
      std::cout << allocation_count << "]]\n";
   }

   std::cout << "]\n";
   return 0;
}
