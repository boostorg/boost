// Copyright (c) 2022 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


// Disable autolinking for unit tests.
#if !defined(BOOST_ALL_NO_LIB)
#define BOOST_ALL_NO_LIB 1
#endif // !defined(BOOST_ALL_NO_LIB)

// Test that header file is self-contained.
#include <boost/process/v2/cstring_ref.hpp>

#include <boost/test/unit_test.hpp>

namespace bp2 = boost::process::v2;

template struct bp2::basic_cstring_ref<char, std::char_traits<char>>;

using char_type = bp2::basic_cstring_ref<wchar_t, std::char_traits<wchar_t>>::const_pointer;

BOOST_AUTO_TEST_CASE(cstring_view_test)
{
    auto null = bp2::cstring_ref();
    BOOST_CHECK(null.empty());
    BOOST_CHECK_NE(null.c_str(), nullptr);
    BOOST_CHECK_EQUAL(null.c_str()[0], '\0');
    BOOST_CHECK_EQUAL(null, "");
    auto *c = "foobar";
    bp2::cstring_ref cv = c;

    BOOST_CHECK_EQUAL(cv.c_str(), c);

    std::string s = "barfoo";

    bp2::cstring_ref sv = s;
    BOOST_CHECK(!s.empty());
    BOOST_CHECK_EQUAL(sv.c_str(), s.c_str());

    BOOST_CHECK_EQUAL_COLLECTIONS(s.begin(), s.end(), sv.begin(), sv.end());
    BOOST_CHECK_EQUAL_COLLECTIONS(s.cbegin(), s.cend(), sv.cbegin(), sv.cend());
    BOOST_CHECK_EQUAL_COLLECTIONS(s.rbegin(), s.rend(), sv.rbegin(), sv.rend());
    BOOST_CHECK_EQUAL_COLLECTIONS(s.crbegin(), s.crend(), sv.crbegin(), sv.crend());

    BOOST_CHECK_EQUAL(sv.size(), 6);
    sv.remove_prefix(1);
    BOOST_CHECK_EQUAL(sv.at(0), 'a');
    BOOST_CHECK_EQUAL(sv.at(4), 'o');
    BOOST_CHECK_THROW(sv.at(5), std::out_of_range);
    BOOST_CHECK_EQUAL(sv.front(), 'a');
    BOOST_CHECK_EQUAL(sv.back(), 'o');

    BOOST_CHECK_EQUAL(sv.length(), 5);
    BOOST_CHECK_EQUAL(sv.c_str(), s.c_str() + 1);
    BOOST_CHECK_EQUAL(sv.substr(2).c_str(), s.c_str() + 3);

    bp2::string_view ssv = sv;
    BOOST_CHECK_EQUAL(ssv, "arfoo");

    ssv = sv.substr(1, 3);
    BOOST_CHECK_EQUAL(ssv, "rfo");

    sv.swap(null);

    const bp2::cstring_ref cc = null;
    BOOST_CHECK_EQUAL(cc.front(), 'a');
    BOOST_CHECK_EQUAL(cc.back(), 'o');
    BOOST_CHECK_GE(cc.max_size(), cc.size());

    char out[2];
    cc.copy(out, 2, 1);
    BOOST_CHECK_EQUAL(out[0], 'r');
    BOOST_CHECK_EQUAL(out[1], 'f');
    BOOST_CHECK(cc.starts_with('a'));
    BOOST_CHECK(cc.starts_with("arf"));
    BOOST_CHECK(cc == cc);
    BOOST_CHECK(cc == null);
    BOOST_CHECK(!(cc == sv));

    BOOST_CHECK(!(cc != cc));
    BOOST_CHECK(!(cc != null));
    BOOST_CHECK(cc != sv);

    null.clear();
    BOOST_CHECK_EQUAL(null.to_string(), "");
    BOOST_CHECK_EQUAL(null.to_string(std::allocator<char>()), "");

    bp2::cstring_ref av = "aa", bv = "bb";
    BOOST_CHECK_LT(av, bv);
    BOOST_CHECK_GT(bv, av);

    BOOST_CHECK_EQUAL(av.compare(av), 0);
    BOOST_CHECK_LT(av.compare(bv), 0);
    BOOST_CHECK_GT(bv.compare(av), 0);

}
