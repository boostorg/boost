//
// Copyright (c) 2022 Dmitry Arkhipov (grisumbras@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/json
//

#include <boost/json/value.hpp>

#include "test_suite.hpp"

BOOST_JSON_NS_BEGIN


class pointer_test
{
    bool
    hasLocation(error_code const& ec)
    {
        return ec.has_location();
    }

    bool
    hasLocation(std::error_code const&)
    {
        return true;
    }

    value
    testValue() const
    {
        return value{
            {"foo", {"bar", "baz", "baf"}},
            {"", 0},
            {"a/b", 1},
            {"c%d", 2},
            {"e^f", 3},
            {"g|h", 4},
            {"i\\j", 5},
            {"k\"l", 6},
            {" ", 7},
            {"m~n", 8},
            {"x", object{{"y", "z"}}},
        };
    }

    value
    bigObject() const
    {
        object result;
        for (int i = 0; i < 50; ++i)
        {
            result.emplace(std::to_string(i), i);
        }
        return result;
    }

public:
    void
    testRootPointer()
    {
        value const jv = testValue();
        BOOST_TEST(&jv.at_pointer("") == &jv);
    }

    void
    testChildPointer()
    {
        value const jv = testValue();
        BOOST_TEST(&jv.at_pointer("/foo")== &jv.at("foo"));
        BOOST_TEST(&jv.at_pointer("/c%d")== &jv.at("c%d"));
        BOOST_TEST(&jv.at_pointer("/e^f")== &jv.at("e^f"));
        BOOST_TEST(&jv.at_pointer("/g|h")== &jv.at("g|h"));
        BOOST_TEST(&jv.at_pointer("/")== &jv.at(""));
        BOOST_TEST(&jv.at_pointer("/i\\j")== &jv.at("i\\j"));
        BOOST_TEST(&jv.at_pointer("/k\"l")== &jv.at("k\"l"));
        BOOST_TEST(&jv.at_pointer("/ ")== &jv.at(" "));
    }

    void
    testEscaped()
    {
        value const jv = testValue();
        BOOST_TEST(&jv.at_pointer("/a~1b")== &jv.at("a/b"));
        BOOST_TEST(&jv.at_pointer("/m~0n")== &jv.at("m~n"));
    }

    void
    testNested()
    {
        value const jv = testValue();
        BOOST_TEST(&jv.at_pointer("/foo/0") == &jv.at("foo").at(0));
        BOOST_TEST(&jv.at_pointer("/foo/1") == &jv.at("foo").at(1));
        BOOST_TEST(&jv.at_pointer("/x/y") == &jv.at("x").at("y"));

        {
            value v1;
            object& o1 = v1.emplace_object();
            object& o2 = (o1["very"] = value()).emplace_object();
            object& o3 = (o2["deep"] = value()).emplace_object();

            array& a1 = (o3["path"] = value()).emplace_array();
            a1.emplace_back(value());

            array& a2 = a1.emplace_back(value()).emplace_array();
            a2.emplace_back(value());
            a2.emplace_back(value());

            array& a3 = a2.emplace_back(value()).emplace_array();
            object& o4 = a3.emplace_back(value()).emplace_object();
            object& o5 = (o4["0"] = value()).emplace_object();
            value& v2 = o5["fin"] = value();

            BOOST_TEST(&v1.at_pointer("/very/deep/path/1/2/0/0/fin") == &v2);
        }
    }

    void
    testErrors()
    {
        value const jv = testValue();
        BOOST_TEST_THROWS(jv.at_pointer("foo"), system_error);
        BOOST_TEST_THROWS(jv.at_pointer("/fo"), system_error);
        BOOST_TEST_THROWS(jv.at_pointer("/m~"), system_error);
        BOOST_TEST_THROWS(jv.at_pointer("/m~n"), system_error);
        BOOST_TEST_THROWS(jv.at_pointer("/foo/bar"), system_error);
        BOOST_TEST_THROWS(jv.at_pointer("/foo/"), system_error);
        BOOST_TEST_THROWS(jv.at_pointer("/foo/01"), system_error);
        BOOST_TEST_THROWS(jv.at_pointer("/foo/2b"), system_error);
        BOOST_TEST_THROWS(jv.at_pointer("/x/y/z"), system_error);
    }

    template<class ErrorCode>
    void
    testNonThrowing()
    {
        value jv = testValue();
        ErrorCode ec = error::syntax;
        BOOST_TEST(jv.find_pointer("/foo/0", ec) == &jv.at("foo").at(0));
        BOOST_TEST(!ec);

        auto const& cjv = jv;
        ec = error::syntax;
        BOOST_TEST(cjv.find_pointer("/foo/1", ec) == &jv.at("foo").at(1));
        BOOST_TEST(!ec);

        jv.find_pointer("foo", ec);
        BOOST_TEST(ec == error::missing_slash);
        BOOST_TEST(hasLocation(ec));

        jv.find_pointer("/fo", ec);
        BOOST_TEST(ec == error::not_found);
        BOOST_TEST(hasLocation(ec));

        jv.find_pointer("/foo/25", ec);
        BOOST_TEST(ec == error::not_found);
        BOOST_TEST(hasLocation(ec));

        value(object()).find_pointer("/foo", ec);
        BOOST_TEST(ec == error::not_found);
        BOOST_TEST(hasLocation(ec));

        bigObject().find_pointer("/foo", ec);
        BOOST_TEST(ec == error::not_found);
        BOOST_TEST(hasLocation(ec));

        jv.find_pointer("/m~", ec);
        BOOST_TEST(ec == error::invalid_escape);
        BOOST_TEST(hasLocation(ec));

        jv.find_pointer("/m~n", ec);
        BOOST_TEST(ec == error::invalid_escape);
        BOOST_TEST(hasLocation(ec));

        jv.find_pointer("/foo/bar", ec);
        BOOST_TEST(ec == error::token_not_number);
        BOOST_TEST(hasLocation(ec));

        jv.find_pointer("/foo/", ec);
        BOOST_TEST(ec == error::token_not_number);
        BOOST_TEST(hasLocation(ec));

        jv.find_pointer("/foo/01", ec);
        BOOST_TEST(ec == error::token_not_number);
        BOOST_TEST(hasLocation(ec));

        jv.find_pointer("/foo/2b", ec);
        BOOST_TEST(ec == error::token_not_number);
        BOOST_TEST(hasLocation(ec));

        jv.find_pointer("/foo/2.", ec);
        BOOST_TEST(ec == error::token_not_number);
        BOOST_TEST(hasLocation(ec));

        jv.find_pointer("/foo/-", ec);
        BOOST_TEST(ec == error::past_the_end);
        BOOST_TEST(hasLocation(ec));

        jv.find_pointer("/foo/-/x", ec);
        BOOST_TEST(ec == error::past_the_end);
        BOOST_TEST(hasLocation(ec));

        jv.find_pointer("/foo/-1", ec);
        BOOST_TEST(ec == error::token_not_number);
        BOOST_TEST(hasLocation(ec));

        jv.find_pointer("/x/y/z", ec);
        BOOST_TEST(ec == error::value_is_scalar);
        BOOST_TEST(hasLocation(ec));

        string s = "/foo/";
        s += std::to_string((std::numeric_limits<std::size_t>::max)());
        if ( '9' == s[s.size() - 1] )
        {
            for (std::size_t i = 6; i < s.size(); ++i)
            {
                s[i] = '0';
            }
            s[5] = '1';
            s += "0";
        }
        else
        {
            ++s[s.size() - 1];
        }
        jv.find_pointer(s, ec);
        BOOST_TEST(ec == error::token_overflow);
        BOOST_TEST(hasLocation(ec));
    }

    void
    run()
    {
        testRootPointer();
        testChildPointer();
        testEscaped();
        testNested();
        testErrors();
        testNonThrowing<error_code>();
        testNonThrowing<std::error_code>();
    }
};

TEST_SUITE(pointer_test, "boost.json.pointer");

BOOST_JSON_NS_END
