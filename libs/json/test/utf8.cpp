//
// Copyright (c) 2022 Dmitry Arkhipov (grisumbras@yandex.ru)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/json
//

#include <boost/json/detail/utf8.hpp>
#include <boost/json/string_view.hpp>

#include "test_suite.hpp"

BOOST_JSON_NS_BEGIN

class utf8_test
{
public:
    void
    testLoadLittleEndian()
    {
        BOOST_TEST(
            detail::load_little_endian<4>("\x01\x02\x03\x04\xFF")
            == 0x04030201);

        BOOST_TEST(
            detail::load_little_endian<4>("\x12\x34\x56\x78\xFF")
            == 0x78563412);

        BOOST_TEST(
            detail::load_little_endian<4>("\xFE\xDC\xBA\x98\xFF")
            == 0x98BADCFE);

        BOOST_TEST(
            detail::load_little_endian<3>("\x12\x45\xFE\xFF")
            == 0x00FE4512);

        BOOST_TEST(
            detail::load_little_endian<3>("\xE0\xA0\x80\xFF")
            == 0x0080A0E0);

        BOOST_TEST(
            detail::load_little_endian<2>("\x37\xFC\xFF")
            == 0x0000FC37);

        BOOST_TEST(detail::load_little_endian<1>("\xF1\xFF") == 0x000000F1);
    }

    void
    testClassifyUtf8()
    {
        BOOST_TEST((detail::classify_utf8('\x00') & 0xFF) == 0);
        // from code point U+0080 (0xC280 in UTF-8)
        BOOST_TEST((detail::classify_utf8('\xC2') & 0xFF) == 2);
        // from code point U+07FF (0xDFBF in UTF-8)
        BOOST_TEST((detail::classify_utf8('\xDF') & 0xFF) == 2);
        // from code point U+0800 (0xE0A080 in UTF-8)
        BOOST_TEST((detail::classify_utf8('\xE0') & 0xFF) == 3);
        // from code point U+0FFFF (0xEFBFBF in UTF-8)
        BOOST_TEST((detail::classify_utf8('\xEF') & 0xFF) == 3);
        // from code point U+010000 (0xF0908080 in UTF-8)
        BOOST_TEST((detail::classify_utf8('\xF0') & 0xFF) == 4);
        // from code point U+010000 (0xF0908080 in UTF-8)
        BOOST_TEST((detail::classify_utf8('\xF0') & 0xFF) == 4);
        // from code point U+010FFFF (0xF48FBFBF in UTF-8)
        BOOST_TEST((detail::classify_utf8('\xF4') & 0xFF) == 4);
    }

    void
    testIsValidUtf8()
    {
        auto is_valid_utf8 = [](char const* str) {
            std::uint16_t first = detail::classify_utf8(*str);
            return detail::is_valid_utf8(str, first);
        };

        BOOST_TEST(is_valid_utf8("\xC2\x80")); // code point U+0080
        BOOST_TEST(is_valid_utf8("\xDF\xBF")); // code point U+07FF
        BOOST_TEST(is_valid_utf8("\xE0\xA0\x80")); // code point U+0800
        BOOST_TEST(is_valid_utf8("\xEF\xBF\xBF")); // from code point U+0FFFF
        BOOST_TEST(is_valid_utf8("\xF0\x90\x80\x80")); // code point U+010000
        BOOST_TEST(is_valid_utf8("\xF4\x8F\xBF\xBF")); // code point U+010FFFF

        BOOST_TEST(! is_valid_utf8("\x80"));
        BOOST_TEST(! is_valid_utf8("\xBF"));

        BOOST_TEST(! is_valid_utf8("\xDF\x00"));
        BOOST_TEST(! is_valid_utf8("\xDF\x7F"));
        BOOST_TEST(! is_valid_utf8("\xDF\xFF"));

        BOOST_TEST(! is_valid_utf8("\xE0\x00\x80"));
        BOOST_TEST(! is_valid_utf8("\xE1\x7F\x80"));
        BOOST_TEST(! is_valid_utf8("\xE4\xFF\x80"));
        BOOST_TEST(! is_valid_utf8("\xE8\x80\x00"));
        BOOST_TEST(! is_valid_utf8("\xEC\x80\x7F"));
        BOOST_TEST(! is_valid_utf8("\xEF\x80\xFF"));

        BOOST_TEST(! is_valid_utf8("\xF0\x00\x80\x80"));
        BOOST_TEST(! is_valid_utf8("\xF1\x7F\x80\x80"));
        BOOST_TEST(! is_valid_utf8("\xF2\xFF\x80\x80"));
        BOOST_TEST(! is_valid_utf8("\xF3\x80\x00\x80"));
        BOOST_TEST(! is_valid_utf8("\xF4\x80\x7F\x80"));
        BOOST_TEST(! is_valid_utf8("\xF0\x80\xFF\x80"));
        BOOST_TEST(! is_valid_utf8("\xF1\x80\x80\x00"));
        BOOST_TEST(! is_valid_utf8("\xF2\x80\x80\x7F"));
        BOOST_TEST(! is_valid_utf8("\xF3\x80\x80\xFF"));
    }

    void
    testUtf8Sequence()
    {
        char const* str = "\xE0\xA0\x80\00\00";
        detail::utf8_sequence seq;
        seq.save(str, std::strlen(str));
        BOOST_TEST(seq.complete());
        BOOST_TEST(seq.length() == 3);
        BOOST_TEST(seq.needed() == 0);
        BOOST_TEST(string_view(seq.data(), 3) == str);
        BOOST_TEST(seq.valid());

        seq.save(str, 1);
        BOOST_TEST(!seq.complete());
        BOOST_TEST(seq.length() == 3);
        BOOST_TEST(seq.needed() == 2);
        BOOST_TEST(string_view(seq.data(), 1) == string_view(str, 1));

        seq.append(str + 1, 1);
        BOOST_TEST(!seq.complete());
        BOOST_TEST(seq.length() == 3);
        BOOST_TEST(seq.needed() == 1);
        BOOST_TEST(string_view(seq.data(), 2) == string_view(str, 2));

        seq.append(str + 2, 2);
        BOOST_TEST(seq.complete());
        BOOST_TEST(seq.length() == 3);
        BOOST_TEST(seq.needed() == 0);
        BOOST_TEST(string_view(seq.data(), 3) == str);
        BOOST_TEST(seq.valid());

        seq.append(str + 3, 1);
        BOOST_TEST(seq.complete());
        BOOST_TEST(seq.length() == 3);
        BOOST_TEST(seq.needed() == 0);
        BOOST_TEST(string_view(seq.data(), 3) == str);
        BOOST_TEST(seq.valid());

        str = "\xF0\x90\x80\x80";
        seq.save(str, std::strlen(str));
        BOOST_TEST(seq.complete());
        BOOST_TEST(seq.length() == 4);
        BOOST_TEST(seq.needed() == 0);
        BOOST_TEST(string_view(seq.data(), 4) == str);
        BOOST_TEST(seq.valid());

        str = "\xF0\x90\x80\xC0";
        seq.save(str, std::strlen(str));
        BOOST_TEST(seq.complete());
        BOOST_TEST(seq.length() == 4);
        BOOST_TEST(seq.needed() == 0);
        BOOST_TEST(string_view(seq.data(), 4) == str);
        BOOST_TEST(!seq.valid());
    }

    void
    run()
    {
        testLoadLittleEndian();
        testClassifyUtf8();
        testIsValidUtf8();
        testUtf8Sequence();
    }
};

TEST_SUITE(utf8_test, "boost.json.utf8");

BOOST_JSON_NS_END
