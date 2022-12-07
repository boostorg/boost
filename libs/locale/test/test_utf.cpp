//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/locale/utf.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/static_assert.hpp>
#include <cstring>
#include "test_locale.hpp"
#include "test_locale_tools.hpp"

using namespace boost::locale::utf;

boost::uint32_t const *u32_seq(boost::uint32_t a)
{
    static boost::uint32_t buf[2];
    buf[0]=a;
    buf[1]=0;
    return buf;
}

boost::uint16_t const *u16_seq(boost::uint16_t a)
{
    static boost::uint16_t buf[2];
    buf[0]=a;
    buf[1]=0;
    return buf;
}

boost::uint16_t const *u16_seq(boost::uint16_t a,boost::uint16_t b)
{
    static boost::uint16_t buf[3];
    buf[0]=a;
    buf[1]=b;
    buf[2]=0;
    return buf;
}

#ifndef BOOST_NO_CXX11_CHAR16_T
char16_t const* c16_seq(boost::uint16_t a)
{
    static char16_t buf[2];
    buf[0] = static_cast<char16_t>(a);
    buf[1] = 0;
    return buf;
}
#endif
#ifndef BOOST_NO_CXX11_CHAR32_T
char32_t const* c32_seq(boost::uint32_t a)
{
    static char32_t buf[2];
    buf[0] = static_cast<char32_t>(a);
    buf[1] = 0;
    return buf;
}
#endif


// Get end of C-String, i.e. the NULL byte
template<typename CharType>
CharType const* str_end(CharType const *s)
{
    while(*s)
        s++;
    return s;
}

template<typename CharType>
void test_from_utf(CharType const * const s,unsigned codepoint)
{
    CharType const *cur = s;
    CharType const * const end = str_end(s);

    typedef utf_traits<CharType> tr;

    BOOST_STATIC_ASSERT(tr::max_width == 4 / sizeof(CharType));

    TEST(tr::template decode<CharType const *>(cur,end) == codepoint);

    if(codepoint != illegal)
        TEST(cur == end);

    if(codepoint == incomplete) {
        TEST(*s== 0 || tr::trail_length(*s) > 0);
        TEST(tr::trail_length(*s) >= end - s);
    }

    if(codepoint != incomplete && codepoint != illegal) {
        TEST(tr::is_lead(*s));
        TEST(!tr::is_trail(*s));
        cur = s;
        while(++cur != end) {
            TEST(tr::is_trail(*cur));
            TEST(!tr::is_lead(*cur));
        }
        TEST(tr::width(codepoint)==end - s);
        TEST(tr::trail_length(*s) == tr::width(codepoint) - 1);
        cur = s;
        TEST(tr::decode_valid(cur) == codepoint);
        TEST(cur == end);
    }
}

template<typename CharType>
void test_to_utf(CharType const *str,unsigned codepoint)
{
    CharType buf[5] = {1,1,1,1,1};
    CharType *p=buf;
    p = utf_traits<CharType>::template encode<CharType *>(codepoint,p);
    CharType const * const end = str_end(str);
    TEST(end - str == p - buf);
    TEST(*p);
    *p=0;
    TEST(memcmp(str,buf,sizeof(CharType) * (end-str))==0);
}

template<typename CharType>
void test_valid_utf(CharType const* str, unsigned codepoint)
{
    test_from_utf(str, codepoint);
    test_to_utf(str, codepoint);
}

void test_utf8()
{
    std::cout << "- Test UTF-8\n";

    std::cout << "-- Correct" << std::endl;
    test_valid_utf("\x7f", 0x7f);
    test_valid_utf("\xc2\x80", 0x80);
    test_valid_utf("\xdf\xbf", 0x7ff);
    test_valid_utf("\xe0\xa0\x80", 0x800);
    test_valid_utf("\xef\xbf\xbf", 0xffff);
    test_valid_utf("\xf0\x90\x80\x80", 0x10000);
    test_valid_utf("\xf4\x8f\xbf\xbf", 0x10ffff);

    /// test that this actually works
    test_from_utf(make2(0x80), 0x80);
    test_from_utf(make2(0x7ff), 0x7ff);

    test_from_utf(make3(0x800), 0x800);
    test_from_utf(make3(0xffff), 0xffff);

    test_from_utf(make4(0x10000), 0x10000);
    test_from_utf(make4(0x10ffff), 0x10ffff);

    std::cout << "-- Too big" << std::endl;
    test_from_utf("\xf4\x9f\x80\x80", illegal); //  11 0000
    test_from_utf("\xfb\xbf\xbf\xbf", illegal); // 3ff ffff
    test_from_utf("\xf8\x90\x80\x80\x80", illegal);  // 400 0000
    test_from_utf("\xfd\xbf\xbf\xbf\xbf\xbf", illegal);  // 7fff ffff

    std::cout << "-- Invalid length" << std::endl;

    test_from_utf(make2(0), illegal);
    test_from_utf(make3(0), illegal);
    test_from_utf(make4(0), illegal);
    test_from_utf(make2(0x7f), illegal);
    test_from_utf(make3(0x7f), illegal);
    test_from_utf(make4(0x7f), illegal);

    test_from_utf(make3(0x80), illegal);
    test_from_utf(make4(0x80), illegal);
    test_from_utf(make3(0x7ff), illegal);
    test_from_utf(make4(0x7ff), illegal);

    test_from_utf(make4(0x8000), illegal);
    test_from_utf(make4(0xffff), illegal);
    test_from_utf(make4(0x110000), illegal);
    test_from_utf(make4(0x1fffff), illegal);

    std::cout << "-- Invalid surrogate" << std::endl;

    test_from_utf(make3(0xd800), illegal);
    test_from_utf(make3(0xdbff), illegal);
    test_from_utf(make3(0xdc00), illegal);
    test_from_utf(make3(0xdfff), illegal);

    test_from_utf(make4(0xd800), illegal);
    test_from_utf(make4(0xdbff), illegal);
    test_from_utf(make4(0xdc00), illegal);
    test_from_utf(make4(0xdfff), illegal);

    std::cout << "-- Incomplete" << std::endl;

    test_from_utf("", incomplete);

    test_from_utf("\x80", illegal);
    test_from_utf("\xc2", incomplete);

    test_from_utf("\xdf", incomplete);

    test_from_utf("\xe0", incomplete);
    test_from_utf("\xe0\xa0", incomplete);

    test_from_utf("\xef\xbf", incomplete);
    test_from_utf("\xef", incomplete);

    test_from_utf("\xf0\x90\x80", incomplete);
    test_from_utf("\xf0\x90", incomplete);
    test_from_utf("\xf0", incomplete);

    test_from_utf("\xf4\x8f\xbf", incomplete);
    test_from_utf("\xf4\x8f", incomplete);
    test_from_utf("\xf4", incomplete);
}

void test_utf16()
{
    std::cout << "- Test UTF-16\n";

    std::cout << "-- Correct" << std::endl;
    test_valid_utf(u16_seq(0x10), 0x10);
    test_valid_utf(u16_seq(0xffff), 0xffff);
    test_valid_utf(u16_seq(0xD800, 0xDC00), 0x10000);
    test_valid_utf(u16_seq(0xDBFF, 0xDFFF), 0x10FFFF);

    std::cout << "-- Invalid surrogate" << std::endl;
    test_from_utf(u16_seq(0xDFFF), illegal);
    test_from_utf(u16_seq(0xDC00), illegal);

    std::cout << "-- Incomplete" << std::endl;
    test_from_utf(u16_seq(0), incomplete);
    test_from_utf(u16_seq(0xD800), incomplete);
    test_from_utf(u16_seq(0xDBFF), incomplete);

#ifndef BOOST_NO_CXX11_CHAR16_T
    std::cout << "-- Test char16_t" << std::endl;
#if BOOST_WORKAROUND(BOOST_GCC_VERSION, < 50000)
    test_valid_utf(u"\x0010", 0x10);
    test_valid_utf(u"\xffff", 0xffff);
#else
    test_valid_utf(u"\u0010", 0x10);
    test_valid_utf(u"\uffff", 0xffff);
#endif
    test_valid_utf(u"\U00010000", 0x10000);
    test_valid_utf(u"\U0010FFFF", 0x10FFFF);
    test_from_utf(c16_seq(0xDFFF), illegal);
    test_from_utf(c16_seq(0xDC00), illegal);
#endif
}

void test_utf32()
{
    std::cout << "- Test UTF-32\n";

    std::cout << "-- Correct" << std::endl;
    test_valid_utf(u32_seq(0x10), 0x10);
    test_valid_utf(u32_seq(0xffff), 0xffff);
    test_valid_utf(u32_seq(0x10000), 0x10000);
    test_valid_utf(u32_seq(0x10ffff), 0x10ffff);

    std::cout << "-- Invalid surrogate" << std::endl;
    test_from_utf(u32_seq(0xD800), illegal);
    test_from_utf(u32_seq(0xDBFF), illegal);
    test_from_utf(u32_seq(0xDFFF), illegal);
    test_from_utf(u32_seq(0xDC00), illegal);
    test_from_utf(u32_seq(0x110000), illegal);

    std::cout << "-- Incomplete" << std::endl;
    test_from_utf(u32_seq(0), incomplete);

#ifndef BOOST_NO_CXX11_CHAR32_T
    std::cout << "-- Test char32_t" << std::endl;
#if BOOST_WORKAROUND(BOOST_GCC_VERSION, < 50000)
    test_valid_utf(U"\x0010", 0x10);
#else
    test_valid_utf(U"\U00000010", 0x10);
#endif
    test_valid_utf(U"\U0000ffff", 0xffff);
    test_valid_utf(U"\U00010000", 0x10000);
    test_valid_utf(U"\U0010ffff", 0x10ffff);
    test_from_utf(c32_seq(0xD800), illegal);
    test_from_utf(c32_seq(0xDBFF), illegal);
    test_from_utf(c32_seq(0xDFFF), illegal);
    test_from_utf(c32_seq(0xDC00), illegal);
    test_from_utf(c32_seq(0x110000), illegal);
#endif
}

void test_main(int /*argc*/, char** /*argv*/)
{
    test_utf8();
    test_utf16();
    test_utf32();
}

// boostinspect:noascii
