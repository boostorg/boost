// Copyright 2022 Christian Mazakas.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/unordered/detail/prime_fmod.hpp>

#include <boost/core/detail/splitmix64.hpp>
#include <boost/core/lightweight_test.hpp>

#include <limits>

#if defined(BOOST_MSVC)
// conditional expression is constant
#pragma warning(disable : 4127)
#endif

void macros_test()
{
  if (std::numeric_limits<std::size_t>::digits >= 64) {
#if !defined(BOOST_UNORDERED_FCA_HAS_64B_SIZE_T)
    BOOST_ERROR("std::numeric_limits<size_t>::digits >= 64, but "
                "BOOST_UNORDERED_FCA_HAS_64B_SIZE_T is not defined");
#endif
  }
  else {
#if defined(BOOST_UNORDERED_FCA_HAS_64B_SIZE_T)
    BOOST_ERROR("std::numeric_limits<size_t>::digits < 64, but "
                "BOOST_UNORDERED_FCA_HAS_64B_SIZE_T is defined");
#endif
  }
}

// Pretty inefficient, but the test is fast enough.
// Might be too slow if we had larger primes?
bool is_prime(std::size_t x)
{
  if (x == 2) {
    return true;
  }

  if (x == 1 || x % 2 == 0) {
    return false;
  }

  // y*y <= x is susceptible to overflow, so instead make sure to use y <= (x/y)
  for (std::size_t y = 3; y <= (x / y); y += 2) {
    if (x % y == 0) {
      return false;
    }
  }

  return true;
}

void prime_sizes_test()
{
  // just some basic sanity checks
  //
  BOOST_TEST(!is_prime(0));
  BOOST_TEST(!is_prime(1));
  BOOST_TEST(is_prime(2));
  BOOST_TEST(is_prime(3));
  BOOST_TEST(is_prime(13));
  BOOST_TEST(!is_prime(4));
  BOOST_TEST(!is_prime(100));
  BOOST_TEST(!is_prime(49));

  std::size_t* sizes = boost::unordered::detail::prime_fmod_size<>::sizes;
  std::size_t sizes_len =
    boost::unordered::detail::prime_fmod_size<>::sizes_len;

  // prove every number in our sizes array is prime
  //
  BOOST_TEST_GT(sizes_len, 0u);

  for (std::size_t i = 0; i < sizes_len; ++i) {
    BOOST_TEST(is_prime(sizes[i]));
  }

  // prove that every subsequent number in the sequence is larger than the
  // previous
  //
  for (std::size_t i = 1; i < sizes_len; ++i) {
    BOOST_TEST_GT(sizes[i], sizes[i - 1]);
  }

#if defined(BOOST_UNORDERED_FCA_HAS_64B_SIZE_T)
  // now we wish to prove that if we do have the reciprocals stored, we have the
  // correct amount of them, i.e. one for every entry in sizes[] that fits in 32
  // bits
  //
  boost::uint64_t* inv_sizes32 =
    boost::unordered::detail::prime_fmod_size<>::inv_sizes32;

  std::size_t inv_sizes32_len =
    boost::unordered::detail::prime_fmod_size<>::inv_sizes32_len;

  std::size_t count = 0;
  for (std::size_t i = 0; i < sizes_len; ++i) {
    if (sizes[i] <= UINT32_MAX) {
      ++count;
    }
  }

  BOOST_TEST_GT(inv_sizes32_len, 0u);
  BOOST_TEST_EQ(inv_sizes32_len, count);

  // these values should also be monotonically decreasing
  //
  for (std::size_t i = 1; i < inv_sizes32_len; ++i) {
    BOOST_TEST_LT(inv_sizes32[i], inv_sizes32[i - 1]);
  }

  // now make sure the values in inv_sizes32 are what they should be as derived
  // from the paper
  //
  for (std::size_t i = 0; i < inv_sizes32_len; ++i) {
    std::size_t const size = sizes[i];
    BOOST_TEST_LE(size, UINT_MAX);

    boost::uint32_t d = static_cast<boost::uint32_t>(sizes[i]);
    boost::uint64_t M = ((boost::ulong_long_type(0xffffffff) << 32) +
                          boost::ulong_long_type(0xffffffff)) /
                          d +
                        1;

    BOOST_TEST_EQ(inv_sizes32[i], M);
  }
#endif
}

void get_remainder_test()
{
#if defined(BOOST_UNORDERED_FCA_HAS_64B_SIZE_T)
  struct
  {
    // boost::unordered::detail::prime_fmod_size<>::get_remainder
    // uses several internal implementations depending on the availability of
    // certain intrinsics or 128 bit integer support, defaulting to a slow,
    // portable routine. The following is a transcription of the portable
    // routine used here for verification purposes.
    //
    boost::uint64_t operator()(boost::uint64_t f, boost::uint32_t d)
    {
      boost::uint64_t r1 = (f & UINT32_MAX) * d;
      boost::uint64_t r2 = (f >> 32) * d;

      r2 += r1 >> 32;

      return r2 >> 32;
    }
  } get_remainder;

  boost::detail::splitmix64 rng;

  for (std::size_t i = 0; i < 1000000u; ++i) {
    boost::uint64_t f = rng();
    boost::uint32_t d = static_cast<uint32_t>(rng());

    boost::uint64_t r1 =
      boost::unordered::detail::prime_fmod_size<>::get_remainder(f, d);

    boost::uint64_t r2 = get_remainder(f, d);

    if (!BOOST_TEST_EQ(r1, r2)) {
      std::cerr << "f: " << f << ", d: " << d << std::endl;
      return;
    }
  }
#endif
}

void modulo_test()
{
  std::size_t* sizes = boost::unordered::detail::prime_fmod_size<>::sizes;

  std::size_t const sizes_len =
    boost::unordered::detail::prime_fmod_size<>::sizes_len;

  boost::detail::splitmix64 rng;

  for (std::size_t i = 0; i < 1000000u; ++i) {
    std::size_t hash = static_cast<std::size_t>(rng());

    for (std::size_t j = 0; j < sizes_len; ++j) {
      std::size_t h = hash;

#if defined(BOOST_UNORDERED_FCA_HAS_64B_SIZE_T)
      if (sizes[j] <= UINT_MAX) {
        h = boost::uint32_t(h) + boost::uint32_t(h >> 32);
      }
#endif
      std::size_t p1 =
        boost::unordered::detail::prime_fmod_size<>::position(hash, j);

      std::size_t p2 = h % sizes[j];

      if (!BOOST_TEST_EQ(p1, p2)) {
        std::cerr << "hash: " << hash << ", j: " << j << ", h: " << h
                  << ", sizes[" << j << "]: " << sizes[j] << std::endl;
        return;
      }
    }
  }
}

int main()
{
  macros_test();
  prime_sizes_test();
  get_remainder_test();
  modulo_test();

  return boost::report_errors();
}
