//  (C) Copyright Ivan Matek, Marshall Clow 2021.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/algorithm/is_clamped.hpp>
#include <boost/algorithm/clamp.hpp>
#include <cmath>
#include <cstdint>
#ifdef __cpp_impl_three_way_comparison
#if __has_include(<compare>)
#define BOOST_IS_CLAMPED_TEST_SPACESHIP
#endif
#ifdef BOOST_IS_CLAMPED_TEST_SPACESHIP
#include <compare>
#endif
#endif
#include <limits>
#include <string>
#include <tuple>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

namespace ba = boost::algorithm;

BOOST_CONSTEXPR bool intGreater(int lhs, int rhs) { return lhs > rhs; }

class custom {
 public:
  custom(int x) : v(x) {}
  custom(const custom &rhs) : v(rhs.v) {}

  bool operator<(const custom &rhs) const { return v < rhs.v; }
  bool operator==(const custom &rhs) const {
    return v == rhs.v;
  }  // need this for the test
  int v;
};

static bool customLess(const custom &lhs, const custom &rhs) { return lhs.v < rhs.v; }

void test_ints() {
  //  Inside the range, equal to the endpoints, and outside the endpoints.
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 3, 1, 6 ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 1, 1, 6 ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 6, 1, 6 ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( 0, 1, 6 ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( 7, 1, 6 ));

  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 3, 6, 1, intGreater ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 1, 6, 1, intGreater ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 6, 6, 1, intGreater ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( 0, 6, 1, intGreater ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( 7, 6, 1, intGreater ));

  //  Negative numbers
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( -3, -6, -1 ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( -1, -6, -1 ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( -6, -6, -1 ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped (  0, -6, -1 ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( -7, -6, -1 ));

  //  Mixed positive and negative numbers
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped (  1, -5, 5 ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( -5, -5, 5 ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped (  5, -5, 5 ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( -6, -5, 5 ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped (  6, -5, 5 ));

  //  Unsigned
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 5U, 1U, 6U ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 1U, 1U, 6U ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 6U, 1U, 6U ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( 0U, 1U, 6U ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( 8U, 1U, 6U ));

  //  Mixed (1)
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 5U, 1, 6 ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 1U, 1, 6 ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 6U, 1, 6 ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( 0U, 1, 6 ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( 8U, 1, 6 ));

  //  Mixed (2)
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 5U, 1, 6. ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 1U, 1, 6. ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 6U, 1, 6. ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( 0U, 1, 6. ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( 8U, 1, 6. ));

  // float->short conversion does not round
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( 50, 50.999, 100 ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( 50, 51.001, 100 ));
}

void test_floats() {
  //  If floats are IEEE754 certain floats have exact representations.
  if (std::numeric_limits<float>::is_iec559) {
    const float lo = 0.125;
    const float hi = 0.625;
    BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( lo, lo, hi ));
    BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( hi, lo, hi ));
    BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( lo + 0.01, lo, hi ));
    BOOST_CHECK_EQUAL ( false, ba::is_clamped ( lo - 0.01, lo, hi ));
    BOOST_CHECK_EQUAL ( false, ba::is_clamped ( hi + 0.01, lo, hi ));
    // If we have nextafterf we can be more precise.
    #if __cplusplus >= 201103L
    assert(lo < hi);
    BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( hi, lo, hi ));
    BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( std::nextafterf( lo, hi ), lo, hi ));
    BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( std::nextafterf( hi, lo ), lo, hi ));
    BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( (lo + hi) / 2, lo, hi ));
    // 1.0 is just for direction of nextafterf, value of 1.0 is not significant.
    BOOST_CHECK_EQUAL ( false, ba::is_clamped ( std::nextafterf( lo, lo - 1.0f ), lo, hi ));
    BOOST_CHECK_EQUAL ( false, ba::is_clamped ( std::nextafterf( hi, hi + 1.0f ), lo, hi ));
    #endif
  }
}

void test_std_string() {
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( std::string("a3"), std::string("a1"), std::string("a6") ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( std::string("a1"), std::string("a1"), std::string("a6") ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( std::string("a6"), std::string("a1"), std::string("a6") ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( std::string("a7"), std::string("a1"), std::string("a6") ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( std::string("a0"), std::string("a1"), std::string("a6") ));
}

void test_custom() {
  //  Inside the range, equal to the endpoints, and outside the endpoints.
  const custom c0(0);
  const custom c1(1);
  const custom c3(3);
  const custom c6(6);
  const custom c7(7);
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( c3, c1, c6 ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( c1, c1, c6 ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( c6, c1, c6 ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( c0, c1, c6 ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( c7, c1, c6 ));

  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( c3, c1, c6, customLess ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( c1, c1, c6, customLess ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped ( c6, c1, c6, customLess ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( c0, c1, c6, customLess ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped ( c7, c1, c6, customLess ));
}

void test_point_interval() {
    BOOST_CHECK_EQUAL(true, ba::is_clamped(1, 1, 1));
    BOOST_CHECK_EQUAL(false, ba::is_clamped(0, 1, 1));
    BOOST_CHECK_EQUAL(false, ba::is_clamped(2, 1, 1));
}

void test_first_argument_determines_types() {
    // all arguments are int
    BOOST_CHECK_EQUAL(true, ba::is_clamped(5, 5.9, 6.1));
    BOOST_CHECK_EQUAL(true, ba::is_clamped(6, 5.9, 6.1));
    // all arguments are double
    BOOST_CHECK_EQUAL(false, ba::is_clamped(5.0, 5.9, 6.1));
    BOOST_CHECK_EQUAL(false, ba::is_clamped(6.2, 5.9, 6.1));
}

void test_constexpr() {
  #if __cplusplus >= 201402L
  {
    constexpr bool is_clamped = (ba::is_clamped ( 3, 1, 6 ));
    BOOST_CHECK_EQUAL (true, is_clamped );
  }
  {
    constexpr bool is_clamped = (ba::is_clamped ( 1, 1, 6 ));
    BOOST_CHECK_EQUAL (true, is_clamped );
  }
  {
    constexpr bool is_clamped = (ba::is_clamped ( 6, 1, 6 ));
    BOOST_CHECK_EQUAL (true, is_clamped );
  }
  {
    constexpr bool is_clamped = (ba::is_clamped ( 0, 1, 6 ));
    BOOST_CHECK_EQUAL(false, is_clamped );
  }
  {
    constexpr bool is_clamped = (ba::is_clamped ( 7, 1, 6 ));
    BOOST_CHECK_EQUAL(false, is_clamped );
  }
  #endif
}


#ifdef BOOST_IS_CLAMPED_TEST_SPACESHIP
struct custom_with_spaceship {
  int v;
  auto operator<=>(const custom_with_spaceship&) const = default;
};
#endif

void test_spaceship() {
  #ifdef BOOST_IS_CLAMPED_TEST_SPACESHIP
  //  Inside the range, equal to the endpoints, and outside the endpoints.
  const custom_with_spaceship c0(0);
  const custom_with_spaceship c1(1);
  const custom_with_spaceship c3(3);
  const custom_with_spaceship c6(6);
  const custom_with_spaceship c7(7);
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped (c3, c1, c6 ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped (c1, c1, c6 ));
  BOOST_CHECK_EQUAL ( true,  ba::is_clamped (c6, c1, c6 ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped (c0, c1, c6 ));
  BOOST_CHECK_EQUAL ( false, ba::is_clamped (c7, c1, c6 ));
  {
    constexpr custom_with_spaceship c1(1);
    constexpr custom_with_spaceship c3(3);
    constexpr custom_with_spaceship c6(6);
    constexpr bool clamped = ba::is_clamped (c3, c1, c6 );
    BOOST_CHECK_EQUAL ( true, clamped );
  }
  #endif
}

BOOST_AUTO_TEST_CASE(test_main) {
  test_ints();
  test_floats();
  test_std_string();
  test_custom();
  test_point_interval();
  test_first_argument_determines_types();
  test_constexpr();
  test_spaceship();
}

#if __cplusplus >= 201103L
typedef std::tuple<std::uint8_t, std::int8_t, std::uint16_t, std::int16_t,
                   std::int32_t, std::uint32_t, std::int64_t, std::uint64_t> test_types_tuple;

BOOST_AUTO_TEST_CASE_TEMPLATE(test_extremes, T, test_types_tuple) {
  const T max = std::numeric_limits<T>::max();
  BOOST_CHECK_EQUAL(true,  ba::is_clamped( max, max, max ));
  BOOST_CHECK_EQUAL(true,  ba::is_clamped( max, max - 1, max ));
  BOOST_CHECK_EQUAL(false, ba::is_clamped( max - 1, max, max ));

  const T min = std::numeric_limits<T>::min();
  BOOST_CHECK_EQUAL(true,  ba::is_clamped( min, min, min ));
  BOOST_CHECK_EQUAL(true,  ba::is_clamped( min, min, min + 1 ));
  BOOST_CHECK_EQUAL(false, ba::is_clamped( min, min + 1, min + 1 ));
}
#endif
