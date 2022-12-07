// Copyright (c) 2022 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


// Disable autolinking for unit tests.
#if !defined(BOOST_ALL_NO_LIB)
#define BOOST_ALL_NO_LIB 1
#endif // !defined(BOOST_ALL_NO_LIB)

// Test that header file is self-contained.
#include <boost/process/v2/environment.hpp>

#include <boost/test/unit_test.hpp>

#include <unordered_map>

namespace bp2 = boost::process::v2;
namespace bpe = boost::process::v2::environment;


BOOST_AUTO_TEST_CASE(environment)
{

    for (auto && elem : bpe::get("PATH"))
        BOOST_CHECK(std::find(elem.begin(), elem.end(), bpe::delimiter) == elem.end());

    BOOST_CHECK(bpe::get("PATH").size() > 0);

    const auto key1 = "BP2_TEST_NAME_\320\240\320\230\320\221\320\220"; // РИБА
#if defined(BOOST_PROCESS_V2_WINDOWS)
    const auto key2 = "BP2_TeSt_NamE_\321\200\320\270\320\261\320\260"; // риба
#else
    const auto key2 = key1; 
#endif
    bp2::error_code ec;
    bpe::unset(key1, ec);
    bpe::unset(key2, ec);
    ec.clear();
    
    BOOST_CHECK_THROW(bpe::get(key1) , bp2::system_error);
    bpe::get(key2, ec);
    BOOST_CHECK(ec);
    ec.clear();

    bpe::set(key1, "some test string");
    BOOST_CHECK(bpe::get(key1) == "some test string");

    bpe::set(key2, "some test string");

    bpe::get(key1, ec);
    BOOST_CHECK(!ec);

    BOOST_CHECK(bpe::get(key2) == "some test string");
    bpe::unset(key2);

    BOOST_CHECK_THROW(bpe::set("invalid=", "blablubb") , bp2::system_error);
    BOOST_CHECK_THROW(bpe::get(key1) , bp2::system_error);
    bpe::get(key2, ec);
    BOOST_CHECK(ec);
    ec.clear();

    for (auto && ke : bpe::current())
      BOOST_CHECK_EQUAL(bpe::get(std::get<0>(ke)), std::get<1>(ke));


#if defined(BOOST_PROCESS_V2_POSIX)
    BOOST_CHECK_EQUAL(bpe::key("FOO"), bpe::key_view("FOO"));
    BOOST_CHECK_EQUAL(bpe::key("FOO"), std::string("FOO"));
    BOOST_CHECK_EQUAL(bpe::key_value_pair("FOO=BAR"), bpe::key_value_pair_view("FOO=BAR"));
    BOOST_CHECK_EQUAL(bpe::key_value_pair("FOO", "BAR"), bpe::key_value_pair_view("FOO=BAR"));

    using sv = bpe::value::string_type;
    std::string cmp = sv("FOO=X") + bpe::delimiter + sv("YY") + bpe::delimiter + sv("Z42");
    BOOST_CHECK_EQUAL(bpe::key_value_pair("FOO", {"X", "YY", "Z42"}), cmp);
#endif


#if defined(BOOST_PROCESS_V2_POSIX)
    std::unordered_map<std::string, std::string> custom_env = 
        {
            {"HOME", "/home/byzantium"},
            {"HOMEDRIVE", "X:"},
            {"HOMEPATH", "\\users\\theodora"}
        };
    std::vector<std::string> custom_env2 = 
        {
            "HOME=/home/byzantium",
            "HOMEDRIVE=X:",
            "HOMEPATH=\\users\\theodora"
        };
    BOOST_CHECK_EQUAL(bpe::home(custom_env), "/home/byzantium");
    BOOST_CHECK_EQUAL(bpe::home(custom_env2), "/home/byzantium");
#else
    std::unordered_map<std::wstring, std::wstring> custom_env = 
        {
            L"HOME", L"/home/byzantium",
            L"HOMEDRIVE", L"X:",
            L"HOMEPATH", L"\\users\\theodora"
        };

    std::vector<std::wstring> custom_env2 = 
        {
            {L"HOME=/home/byzantium"},
            {L"HOMEDRIVE=X:"},
            {L"HOMEPATH=\\users\\theodora"}
        };
    BOOST_CHECK_EQUAL(bpe::home(custom_env), L"X:\\Users\\theodora");
    BOOST_CHECK_EQUAL(bpe::home(custom_env2), L"X:\\Users\\theodora");

#endif

    bp2::process_environment env{custom_env };
    boost::ignore_unused(env);

    bp2::process_environment env2{custom_env2};
    boost::ignore_unused(env2);
}



BOOST_AUTO_TEST_CASE(wenvironment)
{
    for (auto && elem : bpe::get(L"PATH"))
        BOOST_CHECK(std::find(elem.begin(), elem.end(), bpe::delimiter) == elem.end());

    BOOST_CHECK(bpe::get(L"PATH").size() > 0);

    const auto key1 = L"BP2_TEST_NAME_W_\u0420\u0418\u0411\u0410";
#if defined(BOOST_PROCESS_V2_WINDOWS)
    const auto key2 = L"BP2_TeSt_NamE_W_\u0440\u0438\u0431\u0430";
#else
    const auto key2 = key1;
#endif

    BOOST_CHECK_THROW(bpe::get(key1) , bp2::system_error);
    bp2::error_code ec;
    bpe::get(key2, ec);
    BOOST_CHECK(ec);

    bpe::set(key1, L"some test string");
    BOOST_CHECK(bpe::get(key1) == L"some test string");
    BOOST_CHECK(bpe::get(key2) == L"some test string");
    bpe::unset(key2);

    BOOST_CHECK_THROW(bpe::set(L"invalid=", L"blablubb") , bp2::system_error);
    BOOST_CHECK_THROW(bpe::get(key1) , bp2::system_error);
    bpe::get(key2, ec);
    BOOST_CHECK(ec);

    for (const auto ke : bpe::current())
        BOOST_CHECK_EQUAL(bpe::get(std::get<0>(ke)), std::get<1>(ke));

#if defined(BOOST_PROCESS_V2_WINDOWS)
    BOOST_CHECK_EQUAL(bpe::key(L"FOO"), bpe::key_view(L"Foo"));
    BOOST_CHECK(bpe::key(L"FOO") == std::wstring(L"Foo"));
    BOOST_CHECK_EQUAL(bpe::key_value_pair(L"Foo=BAR"), bpe::key_value_pair_view(L"FOO=BAR"));
    BOOST_CHECK_EQUAL(bpe::key_value_pair(L"Foo=BAR"), bpe::key_value_pair(L"FOO=BAR"));
    BOOST_CHECK_EQUAL(bpe::key_value_pair_view(L"Foo=BAR"), bpe::key_value_pair_view(L"FOO=BAR"));
    BOOST_CHECK_EQUAL(bpe::key_value_pair(L"Foo", L"BAR"), bpe::key_value_pair_view(L"FOO=BAR"));

    BOOST_CHECK_NE(bpe::key_value_pair(L"FOO=BAR"), bpe::key_value_pair_view(L"FOO=Bar"));
    BOOST_CHECK_LT(bpe::key_value_pair(L"FOO=BAR"), bpe::key_value_pair_view(L"goo=Bar"));
    BOOST_CHECK_NE(bpe::key_value_pair(L"FOO", L"BAR"), bpe::key_value_pair_view(L"FOO=Bar"));

    using sv = bpe::value::string_type;
    std::wstring cmp = sv(L"FOO=X") + bpe::delimiter + sv(L"YY") + bpe::delimiter + sv(L"Z42");
    BOOST_CHECK(bpe::key_value_pair(L"FOO", {L"X", L"YY", L"Z42"}) == cmp);
#endif
}
