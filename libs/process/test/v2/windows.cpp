// Copyright (c) 2022 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


// Disable autolinking for unit tests.
#if !defined(BOOST_ALL_NO_LIB)
#define BOOST_ALL_NO_LIB 1
#endif // !defined(BOOST_ALL_NO_LIB)

#if defined(BOOST_FILESYSTEM_DYN_LINK)
#undef BOOST_FILESYSTEM_DYN_LINK
#endif

// Test that header file is self-contained.
#include <boost/process/v2/process.hpp>


#include <boost/process/v2/windows/as_user_launcher.hpp>
#include <boost/process/v2/windows/creation_flags.hpp>
#include <boost/process/v2/windows/show_window.hpp>
#include <boost/process/v2/windows/with_logon_launcher.hpp>
#include <boost/process/v2/windows/with_token_launcher.hpp>


#include <boost/test/unit_test.hpp>
#include <boost/asio/io_context.hpp>

#include <fstream>
#include <thread>

namespace bpv = boost::process::v2;
namespace asio = boost::asio;

BOOST_AUTO_TEST_SUITE(windows);


BOOST_AUTO_TEST_CASE(show_window)
{
    using boost::unit_test::framework::master_test_suite;
    asio::io_context ctx;
    const auto pth = master_test_suite().argv[1];
    bpv::process proc{ctx, pth, {"showwindow"}};

    BOOST_CHECK_EQUAL(proc.wait(), 0);

    proc = bpv::process{ctx, pth, {"showwindow"}, bpv::windows::show_window_minimized_not_active};
    BOOST_CHECK_EQUAL(proc.wait(), SW_SHOWMINNOACTIVE);

}

BOOST_AUTO_TEST_CASE(creation_flags)
{
    using boost::unit_test::framework::master_test_suite;
    const auto pth = master_test_suite().argv[1];
    asio::io_context ctx;
    bpv::process proc{ctx, pth, {"creation-flags"}};

    BOOST_CHECK_EQUAL(proc.wait() & ~EXTENDED_STARTUPINFO_PRESENT, 0);

    proc = bpv::process{ctx, master_test_suite().argv[1], {"creation-flags"}, bpv::windows::process_creation_flags<STARTF_TITLEISAPPID>()};
    BOOST_CHECK(proc);
    BOOST_CHECK_EQUAL(proc.wait() & ~EXTENDED_STARTUPINFO_PRESENT, STARTF_TITLEISAPPID);
}

BOOST_AUTO_TEST_CASE(as_user_launcher)
{
    using boost::unit_test::framework::master_test_suite;
    const auto pth = master_test_suite().argv[1];

    asio::io_context ctx;
    std::vector<std::string> args = {"exit-code", "2"};

    HANDLE token_handle = INVALID_HANDLE_VALUE;
    BOOST_CHECK(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY, &token_handle) != 0);
    bpv::error_code ec;
    auto proc = bpv::windows::as_user_launcher(token_handle)(ctx, ec, pth, args);
    BOOST_CHECK_EQUAL(proc.wait(), 2);
}

BOOST_AUTO_TEST_CASE(with_logon_launcher)
{
    using boost::unit_test::framework::master_test_suite;
    const auto pth = master_test_suite().argv[1];

    asio::io_context ctx;
    std::vector<std::string> args = {"exit-code", "42"};

    bpv::error_code ec;
    auto proc = bpv::windows::with_logon_launcher(L"idiot", L"changeme")(ctx, ec, pth, args);
    BOOST_CHECK_EQUAL(ec.value(), ERROR_INVALID_PARAMETER);
}


BOOST_AUTO_TEST_CASE(with_token_launcher)
{
    using boost::unit_test::framework::master_test_suite;
    const auto pth = master_test_suite().argv[1];

    asio::io_context ctx;
    std::vector<std::string> args = {"exit-code", "2"};

    HANDLE token_handle = INVALID_HANDLE_VALUE;
    BOOST_CHECK(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE | TOKEN_ASSIGN_PRIMARY, &token_handle) != 0);
    bpv::error_code ec;
    auto proc = bpv::windows::with_token_launcher(nullptr)(ctx, ec, pth, args);
    BOOST_CHECK_EQUAL(ec.value(), ERROR_INVALID_PARAMETER);
}


BOOST_AUTO_TEST_SUITE_END();

