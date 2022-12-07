// Copyright (C) 2021 Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/lib/optional for documentation.
//
// You are welcome to contact the author at:
//  akrzemi1@gmail.com

#include "boost/optional/optional.hpp"

#include "boost/core/lightweight_test.hpp"
#include "boost/none.hpp"

//#ifndef BOOST_OPTIONAL_NO_CONVERTING_COPY_CTOR

using boost::optional;

struct implicit_bool_conv
{
  operator bool() const { return false; }
};

struct explicit_bool_conv
{
  bool operator!() const BOOST_NOEXCEPT { return false; }
  BOOST_EXPLICIT_OPERATOR_BOOL_NOEXCEPT()
};

template <typename To, typename From>
void test_convert_assign()
{
  optional<To> oi;
  oi = From();
  BOOST_TEST(oi);
}

void test_no_bad_assignment()
{
#if !defined(BOOST_NO_CXX11_DECLTYPE) && !BOOST_WORKAROUND(BOOST_MSVC, < 1800)
  // this means that type trait `boost::is_assignable` works.
  BOOST_STATIC_ASSERT((boost::is_assignable<optional<bool>&, bool>::value));
  BOOST_STATIC_ASSERT((boost::is_assignable<optional<bool>&, implicit_bool_conv>::value));
  BOOST_STATIC_ASSERT((! boost::is_assignable<optional<bool>&, explicit_bool_conv>::value));
#endif
}

int main()
{
  test_convert_assign<int, short>();
  test_convert_assign<bool, implicit_bool_conv>();
  test_no_bad_assignment();

  return boost::report_errors();
}
