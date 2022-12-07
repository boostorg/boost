//
// promise.cpp
// ~~~~~~~~~~~
//
// Copyright (c) 2021-2022 Klemens D. Morgenstern
//                         (klemens dot morgenstern at gmx dot net)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Disable autolinking for unit tests.
#if !defined(BOOST_ALL_NO_LIB)
#define BOOST_ALL_NO_LIB 1
#endif // !defined(BOOST_ALL_NO_LIB)

// Test that header file is self-contained.
#include <boost/asio/experimental/promise.hpp>

#include <array>
#include <vector>
#include <boost/asio/redirect_error.hpp>
#include <boost/asio/steady_timer.hpp>
#include "../unit_test.hpp"

namespace promise {

void promise_tester()
{
  using namespace boost::asio;
  using namespace std::chrono;

  io_context ctx;

  steady_timer timer1{ctx}, timer2{ctx};

  const auto started_when = steady_clock::now();
  timer1.expires_at(started_when + milliseconds(2000));
  timer2.expires_at(started_when + milliseconds(1000));
  auto p = timer1.async_wait(experimental::use_promise);

  steady_clock::time_point completed_when;
  boost::system::error_code ec;
  bool called = false;

  p.async_wait(
      [&](auto ec_)
      {
        ec = ec_;
        called = true;
        completed_when = steady_clock::now();
      });

  steady_clock::time_point timer2_done;
  timer2.async_wait(
      [&](auto)
      {
        timer2_done = steady_clock::now();;
        p.cancel();
      });

  ctx.run();

  BOOST_ASIO_CHECK(timer2_done + milliseconds(1) > completed_when);
  BOOST_ASIO_CHECK(called);
  BOOST_ASIO_CHECK(ec == error::operation_aborted);
}

void test_cancel()
{
  boost::asio::io_context ctx;
  boost::asio::steady_timer tim{ctx, std::chrono::seconds(10)};
  boost::system::error_code ec;

  {
    auto p = tim.async_wait(
        boost::asio::redirect_error(
          boost::asio::experimental::use_promise, ec));
  }

  ctx.run();

  BOOST_ASIO_CHECK_MESSAGE(
      ec == boost::asio::error::operation_aborted,
      ec.message());
}

} // namespace promise

BOOST_ASIO_TEST_SUITE
(
    "promise",
    BOOST_ASIO_TEST_CASE(promise::promise_tester)
)
