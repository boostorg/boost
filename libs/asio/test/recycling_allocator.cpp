//
// recycling_allocator.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2022 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Disable autolinking for unit tests.
#if !defined(BOOST_ALL_NO_LIB)
#define BOOST_ALL_NO_LIB 1
#endif // !defined(BOOST_ALL_NO_LIB)

// Test that header file is self-contained.
#include <boost/asio/recycling_allocator.hpp>

#include "unit_test.hpp"
#include <vector>
#include <boost/asio/detail/type_traits.hpp>

void recycling_allocator_test()
{
  BOOST_ASIO_CHECK((
      boost::asio::is_same<
        boost::asio::recycling_allocator<int>::value_type,
        int
      >::value));

  BOOST_ASIO_CHECK((
      boost::asio::is_same<
        boost::asio::recycling_allocator<void>::value_type,
        void
      >::value));

  BOOST_ASIO_CHECK((
      boost::asio::is_same<
        boost::asio::recycling_allocator<int>::rebind<char>::other,
        boost::asio::recycling_allocator<char>
      >::value));

  BOOST_ASIO_CHECK((
      boost::asio::is_same<
        boost::asio::recycling_allocator<void>::rebind<char>::other,
        boost::asio::recycling_allocator<char>
      >::value));

  boost::asio::recycling_allocator<int> a1;
  boost::asio::recycling_allocator<int> a2(a1);

  BOOST_ASIO_CHECK(a1 == a2);
  BOOST_ASIO_CHECK(!(a1 != a2));

  boost::asio::recycling_allocator<void> a3;
  boost::asio::recycling_allocator<void> a4(a3);

  BOOST_ASIO_CHECK(a3 == a4);
  BOOST_ASIO_CHECK(!(a3 != a4));

  boost::asio::recycling_allocator<int> a5(a4);
  (void)a5;

  boost::asio::recycling_allocator<void> a6(a5);
  (void)a6;

  int* p = a1.allocate(42);
  BOOST_ASIO_CHECK(p != 0);

  a1.deallocate(p, 42);

#if defined(BOOST_ASIO_HAS_CXX11_ALLOCATORS)
  std::vector<int, boost::asio::recycling_allocator<int> > v(42);
  BOOST_ASIO_CHECK(v.size() == 42);
#endif // defined(BOOST_ASIO_HAS_CXX11_ALLOCATORS)
}

BOOST_ASIO_TEST_SUITE
(
  "recycling_allocator",
  BOOST_ASIO_TEST_CASE(recycling_allocator_test)
)
