//
// as_tuple.cpp
// ~~~~~~~~~~~~
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
#include <boost/asio/as_tuple.hpp>

#include <boost/asio/bind_executor.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/system_timer.hpp>
#include "unit_test.hpp"

void as_tuple_test()
{
#if defined(BOOST_ASIO_HAS_STD_TUPLE) \
  && defined(BOOST_ASIO_HAS_VARIADIC_TEMPLATES)
  boost::asio::io_context io1;
  boost::asio::io_context io2;
  boost::asio::system_timer timer1(io1);
  int count = 0;

  timer1.expires_after(boost::asio::chrono::seconds(0));
  timer1.async_wait(
      boost::asio::as_tuple(
        boost::asio::bind_executor(io2.get_executor(),
          [&count](std::tuple<boost::system::error_code>)
          {
            ++count;
          })));

  BOOST_ASIO_CHECK(count == 0);

  io1.run();

  BOOST_ASIO_CHECK(count == 0);

  io2.run();

  BOOST_ASIO_CHECK(count == 1);
#endif // defined(BOOST_ASIO_HAS_STD_TUPLE)
       //   && defined(BOOST_ASIO_HAS_VARIADIC_TEMPLATES)
}

BOOST_ASIO_TEST_SUITE
(
  "as_tuple",
  BOOST_ASIO_TEST_CASE(as_tuple_test)
)
