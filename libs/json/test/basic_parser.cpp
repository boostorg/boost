//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
// Copyright (c) 2020 Krystian Stasiowski (sdkrystian@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/json
//

// Test that header file is self-contained.
#include <boost/json/basic_parser_impl.hpp>

#include <memory>
#include <string>
#include <utility>

#include "parse-vectors.hpp"
#include "test.hpp"
#include "test_suite.hpp"

BOOST_JSON_NS_BEGIN

BOOST_STATIC_ASSERT( std::is_nothrow_destructible<basic_parser<int>>::value );

namespace base64 {

std::size_t constexpr
decoded_size(std::size_t n)
{
    return n / 4 * 3; // requires n&3==0, smaller
}

signed char const*
get_inverse()
{
    static signed char constexpr tab[] = {
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //   0-15
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, //  16-31
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63, //  32-47
         52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, //  48-63
         -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, //  64-79
         15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1, //  80-95
         -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, //  96-111
         41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1, // 112-127
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 128-143
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 144-159
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 160-175
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 176-191
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 192-207
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 208-223
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // 224-239
         -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1  // 240-255
    };
    return &tab[0];
}

std::pair<std::size_t, std::size_t>
decode(void* dest, char const* src, std::size_t len)
{
    char* out = static_cast<char*>(dest);
    auto in = reinterpret_cast<unsigned char const*>(src);
    unsigned char c3[3], c4[4];
    int i = 0;
    int j = 0;

    auto const inverse = base64::get_inverse();

    while(len-- && *in != '=')
    {
        auto const v = inverse[*in];
        if(v == -1)
            break;
        ++in;
        c4[i] = v;
        if(++i == 4)
        {
            c3[0] =  (c4[0]        << 2) + ((c4[1] & 0x30) >> 4);
            c3[1] = ((c4[1] & 0xf) << 4) + ((c4[2] & 0x3c) >> 2);
            c3[2] = ((c4[2] & 0x3) << 6) +   c4[3];

            for(i = 0; i < 3; i++)
                *out++ = c3[i];
            i = 0;
        }
    }

    if(i)
    {
        c3[0] = ( c4[0]        << 2) + ((c4[1] & 0x30) >> 4);
        c3[1] = ((c4[1] & 0xf) << 4) + ((c4[2] & 0x3c) >> 2);
        c3[2] = ((c4[2] & 0x3) << 6) +   c4[3];

        for(j = 0; j < i - 1; j++)
            *out++ = c3[j];
    }

    return {out - static_cast<char*>(dest),
        in - reinterpret_cast<unsigned char const*>(src)};
}

} // base64

namespace {

bool
validate( string_view s )
{
    // Parse with the null parser and return false on error
    null_parser p;
    error_code ec;
    p.write( s.data(), s.size(), ec );
    if( ec )
        return false;

    // The string is valid JSON.
    return true;
}

parse_options
make_options(
    bool comments,
    bool commas,
    bool utf8)
{
    parse_options opt;
    opt.allow_comments = comments;
    opt.allow_trailing_commas = commas;
    opt.allow_invalid_utf8 = utf8;
    return opt;
}

} // (anon)

class basic_parser_test
{
public:
    ::test_suite::log_type log;

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4456) // non dll-interface class used as base for dll-interface class
#endif

#define TEST_GRIND_ONE(s, good, po) \
    do { \
        string_view _test_gr1_s = s; \
        error_code ec; \
        fail_parser p((po)); \
        auto sz = p.write(false, _test_gr1_s.data(), _test_gr1_s.size(), ec); \
        if(good) \
        { \
            if(! BOOST_TEST(!ec) ) \
            { \
                log << "  " << ec.what() << " after " << sz << " characters of " << _test_gr1_s << '\n'; \
            } \
        } \
        else \
            BOOST_TEST(ec); \
    } while (false)

#define TEST_GRIND(s, good, po) \
    do { \
        TEST_GRIND_ONE((s), (good), (po)); \
        string_view _test_gr_s = s; \
        /* split/errors matrix*/ \
        if(! _test_gr_s.empty()) \
        { \
            for(std::size_t i = 1; i < _test_gr_s.size(); ++i) \
            { \
                for(std::size_t j = 1;;++j) \
                { \
                    error_code ec; \
                    fail_parser p(j, (po)); \
                    auto sz = p.write(true, _test_gr_s.data(), i, ec); \
                    if(ec == error::test_failure) \
                        continue; \
                    if(! ec) \
                    { \
                        sz += p.write(false, \
                            _test_gr_s.data() + i, \
                            _test_gr_s.size() - i, \
                            ec); \
                        if(ec == error::test_failure) \
                            continue; \
                    } \
                    if(good) \
                    { \
                        if(! BOOST_TEST(!ec) ) \
                        { \
                            log << "  " << ec.what() << " after " << sz << " characters of " << _test_gr_s << '\n'; \
                        } \
                    } \
                    else \
                        BOOST_TEST(ec); \
                    break; \
                } \
            } \
        } \
        /* split/exceptions matrix */ \
        if(! _test_gr_s.empty()) \
        { \
            for(std::size_t i = 1; i < _test_gr_s.size(); ++i) \
            { \
                for(std::size_t j = 1;;++j) \
                { \
                    error_code ec; \
                    throw_parser p(j, (po)); \
                    try \
                    { \
                        auto sz = p.write(true, _test_gr_s.data(), i, ec); \
                        if(! ec) \
                            sz += p.write(false, \
                                _test_gr_s.data() + i, \
                                _test_gr_s.size() - i, \
                                ec); \
                        if(good) \
                        { \
                            if(! BOOST_TEST(!ec) ) \
                            { \
                                log << "  " << ec.what() << " after " << sz << " characters of " << _test_gr_s << '\n'; \
                            } \
                        } \
                        else \
                            BOOST_TEST(ec); \
                        break; \
                    } \
                    catch(test_exception const&) \
                    { \
                        continue; \
                    } \
                    catch(std::exception const& e) \
                    { \
                        BOOST_TEST_FAIL(); \
                        log << "  " << e.what() << std::endl; \
                    } \
                } \
            } \
        } \
    } while (false)

#define TEST_BAD_EXT(s, po) TEST_GRIND((s), false, (po))
#define TEST_BAD_ONE_EXT(s, po) TEST_GRIND_ONE((s), false, (po))
#define TEST_BAD(s) TEST_BAD_EXT((s), parse_options())
#define TEST_BAD_ONE(s) TEST_BAD_ONE_EXT((s), parse_options())

#define TEST_GOOD_EXT(s, po) TEST_GRIND((s), true, (po))
#define TEST_GOOD_ONE_EXT(s, po) TEST_GRIND_ONE((s), true, (po))
#define TEST_GOOD(s) TEST_GOOD_EXT((s), parse_options())
#define TEST_GOOD_ONE(s) TEST_GOOD_ONE_EXT((s), parse_options())

    //------------------------------------------------------

    void
    testNull()
    {
        TEST_GOOD("null");
        TEST_GOOD(" null");
        TEST_GOOD("null ");
        TEST_GOOD("\tnull");
        TEST_GOOD("null\t");
        TEST_GOOD("\r\n\t null\r\n\t ");

        TEST_BAD ("n     ");
        TEST_BAD ("nu    ");
        TEST_BAD ("nul   ");
        TEST_BAD ("n---  ");
        TEST_BAD ("nu--  ");
        TEST_BAD ("nul-  ");

        TEST_BAD ("NULL");
        TEST_BAD ("Null");
        TEST_BAD ("nulls");
    }

    void
    testBoolean()
    {
        TEST_GOOD("true");
        TEST_GOOD(" true");
        TEST_GOOD("true ");
        TEST_GOOD("\ttrue");
        TEST_GOOD("true\t");
        TEST_GOOD("\r\n\t true\r\n\t ");

        TEST_BAD ("t     ");
        TEST_BAD ("tr    ");
        TEST_BAD ("tru   ");
        TEST_BAD ("t---  ");
        TEST_BAD ("tr--  ");
        TEST_BAD ("tru-  ");
        TEST_BAD ("TRUE");
        TEST_BAD ("True");
        TEST_BAD ("truer");

        TEST_GOOD("false");
        TEST_GOOD(" false");
        TEST_GOOD("false ");
        TEST_GOOD("\tfalse");
        TEST_GOOD("false\t");
        TEST_GOOD("\r\n\t false\r\n\t ");

        TEST_BAD ("f     ");
        TEST_BAD ("fa    ");
        TEST_BAD ("fal   ");
        TEST_BAD ("fals  ");
        TEST_BAD ("f---- ");
        TEST_BAD ("fa--- ");
        TEST_BAD ("fal-- ");
        TEST_BAD ("fals- ");
        TEST_BAD ("FALSE");
        TEST_BAD ("False");
        TEST_BAD ("falser");
    }

    void
    testString()
    {
        TEST_GOOD(R"jv( "x"   )jv");
        TEST_GOOD(R"jv( "xy"  )jv");
        TEST_GOOD(R"jv( "x y" )jv");

        // escapes
        TEST_GOOD(R"jv(" \" ")jv");
        TEST_GOOD(R"jv(" \\ ")jv");
        TEST_GOOD(R"jv(" \/ ")jv");
        TEST_GOOD(R"jv(" \b ")jv");
        TEST_GOOD(R"jv(" \f ")jv");
        TEST_GOOD(R"jv(" \n ")jv");
        TEST_GOOD(R"jv(" \r ")jv");
        TEST_GOOD(R"jv(" \t ")jv");

        // utf-16 escapes
        TEST_GOOD(R"jv( " \u0000 "       )jv");
        TEST_GOOD(R"jv( " \ud7ff "       )jv");
        TEST_GOOD(R"jv( " \ue000 "       )jv");
        TEST_GOOD(R"jv( " \uffff "       )jv");
        TEST_GOOD(R"jv( " \ud800\udc00 " )jv");
        TEST_GOOD(R"jv( " \udbff\udfff " )jv");
        TEST_GOOD(R"jv( " \n\u0000     " )jv");

        // escape in key
        TEST_GOOD(R"jv( {" \n":null} )jv");

        // incomplete
        TEST_BAD ("\"");

        // illegal control character
        TEST_BAD ((string_view{ "\"" "\x00" "\"", 3 }));
        TEST_BAD ("\"" "\x1f" "\"");
        TEST_BAD ("\"" "\\n" "\x1f" "\"");

        // incomplete escape
        TEST_BAD (R"jv( "\" )jv");

        // invalid escape
        TEST_BAD (R"jv( "\z" )jv");

        // utf-16 escape, fast path,
        // invalid surrogate
        TEST_BAD (R"jv( " \u----       " )jv");
        TEST_BAD (R"jv( " \ud---       " )jv");
        TEST_BAD (R"jv( " \ud8--       " )jv");
        TEST_BAD (R"jv( " \ud80-       " )jv");
        // invalid low surrogate
        TEST_BAD (R"jv( " \ud800------ " )jv");
        TEST_BAD (R"jv( " \ud800\----- " )jv");
        TEST_BAD (R"jv( " \ud800\u---- " )jv");
        TEST_BAD (R"jv( " \ud800\ud--- " )jv");
        TEST_BAD (R"jv( " \ud800\udc-- " )jv");
        TEST_BAD (R"jv( " \ud800\udc0- " )jv");
        // illegal leading surrogate
        TEST_BAD (R"jv( " \udc00       " )jv");
        TEST_BAD (R"jv( " \udfff       " )jv");
        // illegal trailing surrogate
        TEST_BAD (R"jv( " \ud800\udbff " )jv");
        TEST_BAD (R"jv( " \ud800\ue000 " )jv");
    }

    void
    testNumber()
    {
        TEST_GOOD("0");
        TEST_GOOD("0                                ");
        TEST_GOOD("0e0                              ");
        TEST_GOOD("0E0                              ");
        TEST_GOOD("0e00                             ");
        TEST_GOOD("0E01                             ");
        TEST_GOOD("0e+0                             ");
        TEST_GOOD("0e-0                             ");
        TEST_GOOD("0.0                              ");
        TEST_GOOD("0.01                             ");
        TEST_GOOD("0.0e0                            ");
        TEST_GOOD("0.01e+0                          ");
        TEST_GOOD("0.02E-0                          ");
        TEST_GOOD("1                                ");
        TEST_GOOD("12                               ");
        TEST_GOOD("1e0                              ");
        TEST_GOOD("1E0                              ");
        TEST_GOOD("1e00                             ");
        TEST_GOOD("1E01                             ");
        TEST_GOOD("1e+0                             ");
        TEST_GOOD("1e-0                             ");
        TEST_GOOD("1.0                              ");
        TEST_GOOD("1.01                             ");
        TEST_GOOD("1.0e0                            ");
        TEST_GOOD("1.01e+0                          ");
        TEST_GOOD("1.02E-0                          ");
        TEST_GOOD("1.0");

        TEST_GOOD("-0                               ");
        TEST_GOOD("-0e0                             ");
        TEST_GOOD("-0E0                             ");
        TEST_GOOD("-0e00                            ");
        TEST_GOOD("-0E01                            ");
        TEST_GOOD("-0e+0                            ");
        TEST_GOOD("-0e-0                            ");
        TEST_GOOD("-0.0                             ");
        TEST_GOOD("-0.01                            ");
        TEST_GOOD("-0.0e0                           ");
        TEST_GOOD("-0.01e+0                         ");
        TEST_GOOD("-0.02E-0                         ");
        TEST_GOOD("-1                               ");
        TEST_GOOD("-12                              ");
        TEST_GOOD("-1                               ");
        TEST_GOOD("-1e0                             ");
        TEST_GOOD("-1E0                             ");
        TEST_GOOD("-1e00                            ");
        TEST_GOOD("-1E01                            ");
        TEST_GOOD("-1e+0                            ");
        TEST_GOOD("-1e-0                            ");
        TEST_GOOD("-1.0                             ");
        TEST_GOOD("-1.01                            ");
        TEST_GOOD("-1.0e0                           ");
        TEST_GOOD("-1.01e+0                         ");
        TEST_GOOD("-1.02E-0                         ");
        TEST_GOOD("-1.0");

        TEST_GOOD("1.1e309                          ");
        TEST_GOOD("9223372036854775807              ");
        TEST_GOOD("-9223372036854775807             ");
        TEST_GOOD("18446744073709551615             ");
        TEST_GOOD("-18446744073709551615            ");

        TEST_GOOD("1234567890123456");
        TEST_GOOD("-1234567890123456");
        TEST_GOOD("10000000000000000000000000");

        TEST_GOOD("0.900719925474099178             ");

        // non-significant digits
        TEST_GOOD("1000000000000000000000000        ");
        TEST_GOOD("1000000000000000000000000e1      ");
        TEST_GOOD("1000000000000000000000000.0      ");
        TEST_GOOD("1000000000000000000000000.00     ");
        TEST_GOOD("1000000000000000000000000.000000000001");
        TEST_GOOD("1000000000000000000000000.0e1    ");
        TEST_GOOD("1000000000000000000000000.0      ");

        TEST_GOOD("1000000000.1000000000            ");

        TEST_BAD("");
        TEST_BAD("-                                 ");
        TEST_BAD("00                                ");
        TEST_BAD("01                                ");
        TEST_BAD("00.                               ");
        TEST_BAD("00.0                              ");
        TEST_BAD("-00                               ");
        TEST_BAD("-01                               ");
        TEST_BAD("-00.                              ");
        TEST_BAD("-00.0                             ");
        TEST_BAD("1a                                ");
        TEST_BAD("-a                                ");
        TEST_BAD(".                                 ");
        TEST_BAD("1.                                ");
        TEST_BAD("1+                                ");
        TEST_BAD("0.0+                              ");
        TEST_BAD("0.0e+                             ");
        TEST_BAD("0.0e-                             ");
        TEST_BAD("0.0e0-                            ");
        TEST_BAD("0.0e                              ");
        TEST_BAD("1eX                               ");
        TEST_BAD("1000000000000000000000000.e       ");
        TEST_BAD("0.");
        TEST_BAD("0.0e+");
        TEST_BAD("0.0e2147483648");
    }

    void
    testArray()
    {
        TEST_GOOD("[]");
        TEST_GOOD("[ ]");
        TEST_GOOD("[ \t ]");
        TEST_GOOD("[ \"\" ]");
        TEST_GOOD("[ \" \" ]");
        TEST_GOOD("[ \"x\" ]");
        TEST_GOOD("[ \"x\", \"y\" ]");
        TEST_GOOD("[1,2,3]");
        TEST_GOOD(" [1,2,3]");
        TEST_GOOD("[1,2,3] ");
        TEST_GOOD(" [1,2,3] ");
        TEST_GOOD("[1,2,3]");
        TEST_GOOD("[ 1,2,3]");
        TEST_GOOD("[1 ,2,3]");
        TEST_GOOD("[1, 2,3]");
        TEST_GOOD("[1,2 ,3]");
        TEST_GOOD("[1,2, 3]");
        TEST_GOOD("[1,2,3 ]");
        TEST_GOOD(" [  1 , 2 \t\n ,  \n3]");

        TEST_BAD ("[");
        TEST_BAD (" [");
        TEST_BAD (" []]");
        TEST_BAD ("[{]");
        TEST_BAD (R"jv( [ null ; 1 ] )jv");
    }

    void
    testObject()
    {
        TEST_GOOD("{}");
        TEST_GOOD("{ }");
        TEST_GOOD("{ \t }");
        TEST_GOOD("{\"x\":null}");
        TEST_GOOD("{ \"x\":null}");
        TEST_GOOD("{\"x\" :null}");
        TEST_GOOD("{\"x\": null}");
        TEST_GOOD("{\"x\":null }");
        TEST_GOOD("{ \"x\" : null }");
        TEST_GOOD("{ \"x\" : {} }");
        TEST_GOOD("{ \"x\" : [] }");
        TEST_GOOD("{ \"x\" : { \"y\" : null } }");
        TEST_GOOD("{ \"x\" : [{}] }");
        TEST_GOOD("{\"x\\ny\\u0022\":null}");
        TEST_GOOD("{ \"x\":1, \"y\":null}");
        TEST_GOOD("{\"x\":1,\"y\":2,\"z\":3}");
        TEST_GOOD(" {\"x\":1,\"y\":2,\"z\":3}");
        TEST_GOOD("{\"x\":1,\"y\":2,\"z\":3} ");
        TEST_GOOD(" {\"x\":1,\"y\":2,\"z\":3} ");
        TEST_GOOD("{ \"x\":1,\"y\":2,\"z\":3}");
        TEST_GOOD("{\"x\" :1,\"y\":2,\"z\":3}");
        TEST_GOOD("{\"x\":1 ,\"y\":2,\"z\":3}");
        TEST_GOOD("{\"x\":1,\"y\" :2,\"z\":3}");
        TEST_GOOD("{\"x\":1,\"y\": 2,\"z\":3}");
        TEST_GOOD("{\"x\":1,\"y\":2 ,\"z\":3}");
        TEST_GOOD("{\"x\":1,\"y\":2, \"z\":3}");
        TEST_GOOD("{\"x\":1,\"y\":2, \"z\" :3}");
        TEST_GOOD("{\"x\":1,\"y\":2, \"z\": 3}");
        TEST_GOOD("{\"x\":1,\"y\":2, \"z\":3 }");
        TEST_GOOD(" \t { \"x\" \n  :   1, \"y\" :2, \"z\" : 3} \n");

        TEST_GOOD("[{\"x\":[{\"y\":null}]}]");

        TEST_BAD ("{");
        TEST_BAD (" {");
        TEST_BAD (" {}}");
        TEST_BAD ("{{}}");
        TEST_BAD ("{[]}");

        TEST_BAD (R"jv( {"x";null} )jv");
        TEST_BAD (R"jv( {"x":null . "y":0} )jv");
    }

    void
    testParser()
    {
        auto const check =
        [this]( string_view s,
            bool done)
        {
            fail_parser p;
            error_code ec;
            p.write_some(
                true,
                s.data(), s.size(),
                ec);
            if(! BOOST_TEST(! ec))
            {
                log << "    failed to parse: " << s << '\n';
                return;
            }
            BOOST_TEST(done == p.done());
        };

        // done()

        check("{}", true);
        check("{} ", true);
        check("{}x", true);
        check("{} x", true);

        check("[]", true);
        check("[] ", true);
        check("[]x", true);
        check("[] x", true);

        check("\"a\"", true);
        check("\"a\" ", true);
        check("\"a\"x", true);
        check("\"a\" x", true);

        check("0", false);
        check("0 ", true);
        check("0x", true);
        check("0 x", true);
        check("0.", false);
        check("0.0", false);
        check("0.0 ", true);
        check("0.0 x", true);

        check("true", true);
        check("true ", true);
        check("truex", true);
        check("true x", true);

        check("false", true);
        check("false ", true);
        check("falsex", true);
        check("false x", true);

        check("null", true);
        check("null ", true);
        check("nullx", true);
        check("null x", true);

        // flush
        {
            {
                for(auto esc :
                    { "\\\"", "\\\\", "\\/", "\\b",
                      "\\f", "\\n", "\\r", "\\t", "\\u0000"
                    })
                {
                    std::string const big =
                        "\\\"" + std::string(
                        BOOST_JSON_STACK_BUFFER_SIZE-4, '*') + esc;
                    std::string const s =
                        "{\"" + big + "\":\"" + big + "\"}";
                    TEST_GOOD_ONE(s);
                }
            }
            {
                std::string big;
                big = "\\\"" +
                    std::string(
                        BOOST_JSON_STACK_BUFFER_SIZE+ 1, '*');
                std::string s;
                s = "{\"" + big + "\":\"" + big + "\"}";
                TEST_GOOD_ONE(s);
            }
        }

        // no input
        {
            error_code ec;
            fail_parser p;
            p.write(false, nullptr, 0, ec);
            BOOST_TEST(ec);
        }
    }

    void
    testMembers()
    {
        fail_parser p;
        std::size_t n;
        error_code ec;
        n = p.write_some(true, "null", 4, ec );
        if(BOOST_TEST(! ec))
        {
            BOOST_TEST(n == 4);
            BOOST_TEST(p.done());
            n = p.write_some(false, " \t42", 4, ec);
            BOOST_TEST(n == 2);
            BOOST_TEST(! ec);
        }
        p.reset();
        n = p.write_some(false, "[1,2,3]", 7, ec);
        if(BOOST_TEST(! ec))
        {
            BOOST_TEST(n == 7);
            BOOST_TEST(p.done());
        }
    }

    void
    testParseVectors()
    {
        std::vector<parse_options> all_configs =
        {
            make_options(false, false, true),
            make_options(true, false, true),
            make_options(false, true, true),
            make_options(true, true, true),
            make_options(false, false, false),
            make_options(true, false, false),
            make_options(false, true, false),
            make_options(true, true, false)
        };
        parse_vectors pv;
        for(auto const& v : pv)
        {
            // skip these , because basic_parser
            // doesn't have a max_depth setting.
            if( v.name == "structure_100000_opening_arrays" ||
                v.name == "structure_open_array_object")
            {
                continue;
            }
            for (const parse_options& po : all_configs)
            {
                if(v.result == 'i')
                {
                    error_code ec;
                    fail_parser p(po);
                    p.write(
                        false,
                        v.text.data(),
                        v.text.size(),
                        ec);
                    if(! ec)
                        TEST_GOOD_ONE_EXT(v.text, po);
                    else
                        TEST_BAD_ONE_EXT(v.text, po);
                }
                else if(v.result == 'y')
                    TEST_GOOD_ONE_EXT(v.text, po);
                else
                    TEST_BAD_ONE_EXT(v.text, po);
            }
        }
    }

    // https://github.com/boostorg/json/issues/13
    void
    testIssue13()
    {
        validate("\"~QQ36644632   {n");
    }

    void
    testIssue20()
    {
        string_view s =
            "WyL//34zOVx1ZDg0ZFx1ZGM4M2RcdWQ4M2RcdWRlM2M4dWRlMTlcdWQ4M2RcdWRlMzlkZWUzOVx1"
            "ZDg0ZFx1ZGM4M2RcdWQ4M2RcdWRlMzlcXHVkY2M4M1x1ZDg5ZFx1ZGUzOVx1ZDgzZFx1ZGUzOWRb"
            "IGZhbHNlLDMzMzMzMzMzMzMzMzMzMzMzNDMzMzMzMTY1MzczNzMwLDMzMzMzMzMzMzMzMzMzMzMz"
            "MzM3ODAsMzMzMzMzMzMzMzM0MzMzMzMxNjUzNzM3MzAsMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMz"
            "MzM3ODAsMzMzMzMzMzMzMzMzMzQzMzMzMzE2NTM3MzczMCwzMzMzMzMzMzMzMzMzMzMzMzMzNzgw"
            "LDMzMzMzMzM4MzU1MzMwNzQ3NDYwLDMzMTY2NTAwMDAzMzMzMzMwNzQ3MzMzMzMzMzc3OSwzMzMz"
            "MzMzMzMzMzMzMzMzNDMzMzMzMzMwNzQ3NDYwLDMzMzMzMzMzMzMzMzMzMzMzMzMzNzgwLDMzMzMz"
            "MzMzMzMzMzMzMzA4ODM1NTMzMDc0Mzc4MCwzMzMzMzMzMzMzMzMzMzMwODgzNTUzMzA3NDc0NjAs"
            "MzMzMzMzMzMxNjY1MDAwMDMzMzMzNDc0NjAsMzMzMzMzMzMzMzMzMzMzMzMzMzc4MCwzMzMzMzMz"
            "MzMzMzM3MzMzMzE2NjUwMDAwMzMzMzMzMDc0NzMzMzMzMzM3NzksMzMzMzMzMzMzMzMzMzMzMzQz"
            "MzMzMzMwNzQ3NDYwLDMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzc4MCwzMzMzMzMzMzMzNzgw"
            "LDMzMzMzMzMzMzMzMzA4ODM1NTMzMDc0NzQ2MCwzMzE2NjUwMDAwMzMzMzMzMDc0NzMzMzMzMzM3"
            "NzksMzMzMzMzMzMzMzMzMzMzMzQzMzMzMzMwNzQ3NDYwLDMzMzMzMzMzMzMzMzMzMzMzMzMzNzgw"
            "LDMzMzMzMzMzMzMzMzMzMzA4ODM1NTMzMDc0Mzc4MCwzMzMzMzMzMzMzMzMzMzMzMzMwODgzNTUz"
            "MzA3NDM3ODAsMzMzMzMzMzMzMzMzMzMzMDg4MzU1MzMwNzQ3NDYwLDMzMzMzMzMzMzMzMDczMzM3"
            "NDc0NjAsMzMzMzMzMzMzMzMzMzMzMzMzNzgwLDMzMzMzMzMzMzMzMzA4ODM1NTMzMDc0NzQ2MCwz"
            "MzE2NjUwMDAwMzMzMzMzMDc0NzMzMzMzMzM3NzksMzMzMzMzMzMzMzMzMzMzMzQzMzMzMzMzMDc0"
            "NzQ2MCwzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzMzM3ODAsMzMzMzMzMzMzMzMzMzMzMDg4"
            "MzU1MzMwNzQzNzgwLDMzMzMzMzMzMzMzMzMzMzA4ODM1NTMzMDc0NzQ2MCwzMzMzMzMzMzMzMzMz"
            "MzMzMzM0MjQ3LDMzMzMzMzMzMzMzMzMzMzQzMzMzMzMzMzMzMzMzMzM3MzMzMzQzMzMzMzMzMDc0"
            "NzQ2MCwzMzMzMzMzMzMzMzMzMzMzMzMzNzgwLDMzMzMzMzMzMzMzMzA4ODM1NTMzMDc0NzQ2MCwz"
            "MzE2NjUwMDAwMzMzMzMzMDc0NzMzMzMzMzM3NzksMzMzMzMzMzMzMzMzMzMzMzQzMzMzMzMwNzQ3"
            "NDYwLDMzMzMzMzMzMzMzMzMzMzMzMzMzNzgwLDMzMzMzMzMzMzMzMzMzMzA4ODM1NTMzMDc0Mzc4"
            "MCwzMzMzMzMzMzMzMzMzMzMwODgzNTUzMzA3NDc0NjAsMzMzMzMzMzMzLDMzMzMzMzMzMzMzMzMz"
            "MzMzMzM3ODAsMzMzMzMzMzMzMzc4MCwzMzMzMzMzMzMzMzMwODgzNTUzMzA3NDc0NjAsMzMxNjY1"
            "MDAwMDMzMzMzMzA3NDczMzMzMzMzNzc5LDMzMzMzMzMzMzM3ODAsMzMzMzMzMzgzNTUzMzA3NDc0"
            "NjAsMzMxNjY1MDAwMDMzMzMzMzA3NDczMzMzMzMzNzc5LDMzMzMzMzMzMzMzMzMzMzM0MzMzMzMz"
            "MzA3NDc0NjAsMzMzMzMzMzMzMzMzMzMzMzMzMzM3ODAsMzMzMzMzMzMzMzMzMzMzMDg4MzU1MzMw"
            "NzQzNzgwLDMzMzMzMzMzMzMzMzMzMzA4ODM1NTMzMDc0NzQ2MCwzMzMzMzMzMzE2NjUwMDAwMzMz"
            "MzM0NzQ2MCwzMzMzMzMzMzMzMzMzMzMzMzMzNzgwLDMzMzMzMzMzMzMzMzM0MzMzMzMxNjUzNzM3"
            "MzAsMzMzMzMzMzMzMzMzMzMzMzMzMzc4MCwzMzMzMzMzODM1NTMzMDc0NzQ2MCwzMzE2NjUwMDAw"
            "MzMzMzMzMDc0NzMzMzMzMzM3NzksMzMzMzMzMzMzMzMzMzMzMzQzMzMzMzMzMDc0NzQ2MCwzMzMz"
            "MzMzMzMzMzMzMzMzMzMzMzc4MCwzMzMzMzMzMzMzMzMzMzMwODgzNTUzMzA3NDM3ODAsMzMzMzMz"
            "MzMzMzMzMzMzMDg4MzU1MzMwNzQ3NDYwLDMzMzMzMzMzMTY2NTAwMDAzMzMzMzQ3NDYwLDMzMzMz"
            "MzMzMzMzMzMzMzMzMzM3ODAsMzMzMzMzMzMzMzMzNzMzMzM0MzMzMzMzMzA3NDc0NjAsMzMzMzMz"
            "MzMzMzMzMzMzMzMzMzc4MCwzMzMzMzMzMzMzMzMwODgzNTUzMzA3NDc0NjAsMzMxNjY1MDAwMDMz"
            "MzMzMzA3NDczMzMzMzMzNzc5LDMzMzMzMzMzMzMzMzMzMzM0MzMzMzNcdWQ4N2RcdWRlZGV1ZGM4"
            "ZGUzOVx1ZDg0ZFx1ZGM4M2RcdWQ4OGRcdWRlMzlcdWQ4OWRcdWRlMjM5MzMzZWUzOVxk";
        auto const len = base64::decoded_size(s.size());
        std::unique_ptr<char[]> p(new char[len]);
        base64::decode(
            p.get(), s.data(), s.size());
        string_view const js(p.get(), len);
        BOOST_TEST(! validate(js));
    }

    void
    testIssue113()
    {
        string_view s =
            "\"\\r\\n section id='description'>\\r\\nAll        mbers form the uncountable set "
            "\\u211D.  Among its subsets, relatively simple are the convex sets, each expressed "
            "as a range between two real numbers <i>a</i> and <i>b</i> where <i>a</i> \\u2264 <i>"
            "b</i>.  There are actually four cases for the meaning of \\\"between\\\", depending "
            "on open or closed boundary:\\r\\n\\r\\n<ul>\\r\\n  <li>[<i>a</i>, <i>b</i>]: {<i>"
            "x</i> | <i>a</i> \\u2264 <i>x</i> and <i>x</i> \\u2264 <i>b</i> }</li>\\r\\n  <li>"
            "(<i>a</i>, <i>b</i>): {<i>x</i> | <i>a</i> < <i>x</i> and <i>x</i> < <i>b</i> }"
            "</li>\\r\\n  <li>[<i>a</i>, <i>b</i>): {<i>x</i> | <i>a</i> \\u2264 <i>x</i> and "
            "<i>x</i> < <i>b</i> }</li>\\r\\n  <li>(<i>a</i>, <i>b</i>]: {<i>x</i> | <i>a</i> "
            "< <i>x</i> and <i>x</i> \\u2264 <i>b</i> }</li>\\r\\n</ul>\\r\\n\\r\\nNote that "
            "if <i>a</i> = <i>b</i>, of the four only [<i>a</i>, <i>a</i>] would be non-empty."
            "\\r\\n\\r\\n<strong>Task</strong>\\r\\n\\r\\n<ul>\\r\\n  <li>Devise a way to "
            "represent any set of real numbers, for the definition of \\\"any\\\" in the "
            "implementation notes below.</li>\\r\\n  <li>Provide methods for these common "
            "set operations (<i>x</i> is a real number; <i>A</i> and <i>B</i> are sets):</li>"
            "\\r\\n  <ul>\\r\\n    <li>\\r\\n      <i>x</i> \\u2208 <i>A</i>: determine if <i>"
            "x</i> is an element of <i>A</i><br>\\r\\n      example: 1 is in [1, 2), while 2, "
            "3, ... are not.\\r\\n    </li>\\r\\n    <li>\\r\\n      <i>A</i> \\u222A <i>B</i>: "
            "union of <i>A</i> and <i>B</i>, i.e. {<i>x</i> | <i>x</i> \\u2208 <i>A</i> or <i>x"
            "</i> \\u2208 <i>B</i>}<br>\\r\\n      example: [0, 2) \\u222A (1, 3) = [0, 3); "
            "[0, 1) \\u222A (2, 3] = well, [0, 1) \\u222A (2, 3]\\r\\n    </li>\\r\\n    <li>"
            "\\r\\n      <i>A</i> \\u2229 <i>B</i>: intersection of <i>A</i> and <i>B</i>, i.e. "
            "{<i>x</i> | <i>x</i> \\u2208 <i>A</i> and <i>x</i> \\u2208 <i>B</i>}<br>\\r\\n      "
            "example: [0, 2) \\u2229 (1, 3) = (1, 2); [0, 1) \\u2229 (2, 3] = empty set\\r\\n    "
            "</li>\\r\\n    <li>\\r\\n      <i>A</i> - <i>B</i>: difference between <i>A</i> and "
            "<i>B</i>, also written as <i>A</i> \\\\ <i>B</i>, i.e. {<i>x</i> | <i>x</i> \\u2208 "
            "<i>A</i> and <i>x</i> \\u2209 <i>B</i>}<br>\\r\\n      example: [0, 2) \\u2212 (1, "
            "3) = [0, 1]\\r\\n    </li>\\r\\n  </ul>\\r\\n</ul>\\r\\n</section>\\r\\n\"\n";
        TEST_GOOD_ONE(s);
    }

    class comment_parser
    {
        struct handler
        {
            constexpr static std::size_t max_object_size = std::size_t(-1);
            constexpr static std::size_t max_array_size = std::size_t(-1);
            constexpr static std::size_t max_key_size = std::size_t(-1);
            constexpr static std::size_t max_string_size = std::size_t(-1);

            std::string captured = "";
            bool on_document_begin( error_code& ) { return true; }
            bool on_document_end( error_code& ) { return true; }
            bool on_object_begin( error_code& ) { return true; }
            bool on_object_end( std::size_t, error_code& ) { return true; }
            bool on_array_begin( error_code& ) { return true; }
            bool on_array_end( std::size_t, error_code& ) { return true; }
            bool on_key_part( string_view, std::size_t, error_code& ) { return true; }
            bool on_key( string_view, std::size_t, error_code& ) { return true; }
            bool on_string_part( string_view, std::size_t, error_code& ) { return true; }
            bool on_string( string_view, std::size_t, error_code& ) { return true; }
            bool on_number_part( string_view, error_code&) { return true; }
            bool on_int64( std::int64_t, string_view, error_code& ) { return true; }
            bool on_uint64( std::uint64_t, string_view, error_code& ) { return true; }
            bool on_double( double, string_view, error_code& ) { return true; }
            bool on_bool( bool, error_code& ) { return true; }
            bool on_null( error_code& ) { return true; }
            bool on_comment_part( string_view s, error_code& )
            {
                captured.append(s.data(), s.size());
                return true;
            }
            bool on_comment( string_view s, error_code& )
            {
                captured.append(s.data(), s.size());
                return true;
            }
        };

        basic_parser<handler> p_;

    public:
        comment_parser()
            : p_(make_options(true, false, false))
        {
        }

        std::size_t
        write(
            char const* data,
            std::size_t size,
            error_code& ec)
        {
            auto const n = p_.write_some(
                false, data, size, ec);
            if(! ec && n < size)
                ec = error::extra_data;
            return n;
        }

        string_view
        captured() const noexcept
        {
            return p_.handler().captured;
        }
    };

    void
    testComments()
    {
        parse_options disabled;
        parse_options enabled;
        enabled.allow_comments = true;

        const auto replace_and_test =
            [&](string_view s)
        {
            static std::vector<string_view> comments =
            {
                "//\n",
                "//    \n",
                "//aaaa\n",
                "//    aaaa\n",
                "//    /* \n",
                "//    /**/ \n",
                "/**/",
                "/*//*/",
                "/*/*/",
                "/******/",
                "/***    ***/",
                "/**aaaa***/",
                "/***    aaaa***/"
            };

            std::string formatted = "";
            std::string just_comments = "";
            std::size_t guess = std::count(
                s.begin(), s.end(), '@') * 12;
            formatted.reserve(guess + s.size());
            just_comments.reserve(guess);
            std::size_t n = 0;
            for (char c : s)
            {
                if (c == '@')
                {
                    string_view com =
                        comments[((formatted.size() + n) % s.size()) % comments.size()];
                    formatted.append(com.data(), n = com.size());
                    just_comments.append(com.data(), com.size());
                    continue;
                }
                formatted += c;
            }
            TEST_BAD_EXT(formatted, disabled);
            TEST_GOOD_EXT(formatted, enabled);

            {
                // test the handler
                comment_parser p;
                error_code ec;
                p.write( formatted.data(), formatted.size(), ec );
                BOOST_TEST(! ec);
                BOOST_TEST(p.captured() == just_comments);
            }
        };

        replace_and_test("@1");
        replace_and_test("1@");
        replace_and_test("@1@");
        replace_and_test("[@1]");
        replace_and_test("[1@]");
        replace_and_test("[1,2@]");
        replace_and_test("[1,@2]");
        replace_and_test("[1@,2]");
        replace_and_test("@[@1@,@2@]@");
        replace_and_test("{@\"a\":1}");
        replace_and_test("{\"a\"@:1}");
        replace_and_test("{\"a\":1@}");
        replace_and_test("{\"a\":1@,\"b\":2}");
        replace_and_test("{\"a\":1,@\"b\":2}");
        replace_and_test("@{@\"a\"@:@1@,@\"b\"@:@2@}");

        // no following token
        TEST_BAD_EXT("1/", enabled);
        // bad second token
        TEST_BAD_EXT("1/x", enabled);
        // no comment close
        TEST_BAD_EXT("1/*", enabled);
        TEST_BAD_EXT("1/**", enabled);
        TEST_BAD_EXT("[1 //, 2]", enabled);

        // just comment
        TEST_BAD_EXT("//\n", enabled);
        TEST_BAD_EXT("//", enabled);
        TEST_BAD_EXT("/**/", enabled);

        // no newline at EOF
        TEST_GOOD_EXT("1//", enabled);
    }

    void
    testAllowTrailing()
    {
        parse_options disabled;
        parse_options enabled;
        enabled.allow_trailing_commas = true;

        TEST_BAD_EXT("[1,]", disabled);
        TEST_GOOD_EXT("[1,]", enabled);

        TEST_BAD_EXT("[1,[],]", disabled);
        TEST_GOOD_EXT("[1,[],]", enabled);

        TEST_BAD_EXT("[1,{},]", disabled);
        TEST_GOOD_EXT("[1,{},]", enabled);

        TEST_BAD_EXT("[1,{\"a\":1,},]", disabled);
        TEST_GOOD_EXT("[1,{\"a\":1,},]", enabled);

        TEST_BAD_EXT("{\"a\":1,}", disabled);
        TEST_GOOD_EXT("{\"a\":1,}", enabled);

        TEST_BAD_EXT("{\"a\":[1,],}", disabled);
        TEST_GOOD_EXT("{\"a\":[1,],}", enabled);

        TEST_BAD_EXT("{\"a\":[],}", disabled);
        TEST_GOOD_EXT("{\"a\":[],}", enabled);

        TEST_BAD_EXT("{\"a\":[{}, [1,]],}", disabled);
        TEST_GOOD_EXT("{\"a\":[{}, [1,]],}", enabled);

        TEST_BAD_EXT("[[[[[[[],],],],],],]", disabled);
        TEST_GOOD_EXT("[[[[[[[],],],],],],]", enabled);

        TEST_BAD_EXT(
            "{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{},},},},},},}",
            disabled);
        TEST_GOOD_EXT(
            "{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{\"a\":{},},},},},},}",
            enabled);
    }

    class utf8_parser
    {
        struct handler
        {
            constexpr static std::size_t max_object_size = std::size_t(-1);
            constexpr static std::size_t max_array_size = std::size_t(-1);
            constexpr static std::size_t max_key_size = std::size_t(-1);
            constexpr static std::size_t max_string_size = std::size_t(-1);

            std::string captured = "";
            bool on_document_begin( error_code& ) { return true; }
            bool on_document_end( error_code& ) { return true; }
            bool on_object_begin( error_code& ) { return true; }
            bool on_object_end( std::size_t, error_code& ) { return true; }
            bool on_array_begin( error_code& ) { return true; }
            bool on_array_end( std::size_t, error_code& ) { return true; }
            bool on_key_part( string_view, std::size_t, error_code& ) { return true; }
            bool on_key( string_view, std::size_t, error_code& ) { return true; }
            bool on_string_part( string_view sv, std::size_t, error_code& )
            {
                captured.append(sv.data(), sv.size());
                return true;
            }
            bool on_string( string_view sv, std::size_t, error_code& )
            {
                captured.append(sv.data(), sv.size());
                return true;
            }
            bool on_number_part( string_view, error_code&) { return true; }
            bool on_int64( std::int64_t, string_view, error_code& ) { return true; }
            bool on_uint64( std::uint64_t, string_view, error_code& ) { return true; }
            bool on_double( double, string_view, error_code& ) { return true; }
            bool on_bool( bool, error_code& ) { return true; }
            bool on_null( error_code& ) { return true; }
            bool on_comment_part( string_view, error_code& ) { return true; }
            bool on_comment( string_view, error_code& ) { return true; }
        };

        basic_parser<handler> p_;

    public:
        utf8_parser()
            : p_(parse_options())
        {
        }

        std::size_t
        write(
            bool more,
            char const* data,
            std::size_t size,
            error_code& ec)
        {
            auto const n = p_.write_some(
                more, data, size, ec);
            if(! ec && n < size)
                ec = error::extra_data;
            return n;
        }

        string_view
        captured() const noexcept
        {
            return p_.handler().captured;
        }
    };

    void
    testUTF8Validation()
    {
        TEST_GOOD("\"\xc2\x80----------\"");
        TEST_GOOD("\"\xc2\xbf----------\"");
        TEST_GOOD("\"\xdf\x80----------\"");
        TEST_GOOD("\"\xdf\xbf----------\"");

        TEST_GOOD("\"\xcf\x90----------\"");

        TEST_GOOD("\"\xe0\xa0\x80----------\"");
        TEST_GOOD("\"\xe0\xa0\xbf----------\"");
        TEST_GOOD("\"\xe0\xbf\x80----------\"");
        TEST_GOOD("\"\xe0\xbf\xbf----------\"");

        TEST_GOOD("\"\xe0\xb0\x90----------\"");

        TEST_GOOD("\"\xe1\x80\x80----------\"");
        TEST_GOOD("\"\xe1\xbf\x80----------\"");
        TEST_GOOD("\"\xec\x80\x80----------\"");
        TEST_GOOD("\"\xec\xbf\x80----------\"");
        TEST_GOOD("\"\xe1\x80\xbf----------\"");
        TEST_GOOD("\"\xe1\xbf\xbf----------\"");
        TEST_GOOD("\"\xec\x80\xbf----------\"");
        TEST_GOOD("\"\xec\xbf\xbf----------\"");

        TEST_GOOD("\"\xe6\x90\x90----------\"");

        TEST_GOOD("\"\xed\x80\x80----------\"");
        TEST_GOOD("\"\xed\x80\xbf----------\"");
        TEST_GOOD("\"\xed\x9f\x80----------\"");
        TEST_GOOD("\"\xed\x9f\xbf----------\"");

        TEST_GOOD("\"\xed\x90\x90----------\"");

        TEST_GOOD("\"\xee\x80\x80----------\"");
        TEST_GOOD("\"\xee\xbf\x80----------\"");
        TEST_GOOD("\"\xef\x80\x80----------\"");
        TEST_GOOD("\"\xef\xbf\x80----------\"");
        TEST_GOOD("\"\xee\x80\xbf----------\"");
        TEST_GOOD("\"\xee\xbf\xbf----------\"");
        TEST_GOOD("\"\xef\x80\xbf----------\"");
        TEST_GOOD("\"\xef\xbf\xbf----------\"");

        TEST_GOOD("\"\xee\x90\x90----------\"");
        TEST_GOOD("\"\xef\x90\x90----------\"");

        TEST_GOOD("\"\xf0\x90\x80\x80----------\"");
        TEST_GOOD("\"\xf0\x90\xbf\x80----------\"");
        TEST_GOOD("\"\xf0\x90\xbf\xbf----------\"");
        TEST_GOOD("\"\xf0\x90\x80\xbf----------\"");
        TEST_GOOD("\"\xf0\xbf\x80\x80----------\"");
        TEST_GOOD("\"\xf0\xbf\xbf\x80----------\"");
        TEST_GOOD("\"\xf0\xbf\xbf\xbf----------\"");
        TEST_GOOD("\"\xf0\xbf\x80\xbf----------\"");

        TEST_GOOD("\"\xf0\xA0\x90\x90----------\"");

        TEST_GOOD("\"\xf4\x80\x80\x80----------\"");
        TEST_GOOD("\"\xf4\x80\xbf\x80----------\"");
        TEST_GOOD("\"\xf4\x80\xbf\xbf----------\"");
        TEST_GOOD("\"\xf4\x80\x80\xbf----------\"");
        TEST_GOOD("\"\xf4\x8f\x80\x80----------\"");
        TEST_GOOD("\"\xf4\x8f\xbf\x80----------\"");
        TEST_GOOD("\"\xf4\x8f\xbf\xbf----------\"");
        TEST_GOOD("\"\xf4\x8f\x80\xbf----------\"");

        TEST_GOOD("\"\xf4\x88\x90\x90----------\"");

        TEST_GOOD("\"\xf1\x80\x80\x80----------\"");
        TEST_GOOD("\"\xf1\x80\xbf\x80----------\"");
        TEST_GOOD("\"\xf1\x80\xbf\xbf----------\"");
        TEST_GOOD("\"\xf1\x80\x80\xbf----------\"");
        TEST_GOOD("\"\xf1\xbf\x80\x80----------\"");
        TEST_GOOD("\"\xf1\xbf\xbf\x80----------\"");
        TEST_GOOD("\"\xf1\xbf\xbf\xbf----------\"");
        TEST_GOOD("\"\xf1\xbf\x80\xbf----------\"");
        TEST_GOOD("\"\xf3\x80\x80\x80----------\"");
        TEST_GOOD("\"\xf3\x80\xbf\x80----------\"");
        TEST_GOOD("\"\xf3\x80\xbf\xbf----------\"");
        TEST_GOOD("\"\xf3\x80\x80\xbf----------\"");
        TEST_GOOD("\"\xf3\xbf\x80\x80----------\"");
        TEST_GOOD("\"\xf3\xbf\xbf\x80----------\"");
        TEST_GOOD("\"\xf3\xbf\xbf\xbf----------\"");
        TEST_GOOD("\"\xf3\xbf\x80\xbf----------\"");

        TEST_GOOD("\"\xf2\x90\x90\x90----------\"");

        TEST_BAD("\"\xc0\x80----------\"");
        TEST_BAD("\"\xc2\xc0----------\"");
        TEST_BAD("\"\xef\x80----------\"");
        TEST_BAD("\"\xdf\x70----------\"");

        TEST_BAD("\"\xff\x90----------\"");

        TEST_BAD("\"\xe0\x9f\x80----------\"");
        TEST_BAD("\"\xe0\xa0\xfe----------\"");
        TEST_BAD("\"\xc0\xff\xff----------\"");
        TEST_BAD("\"\xc0\xbf\x76----------\"");

        TEST_BAD("\"\xe0\xde\x90----------\"");

        TEST_BAD("\"\xe1\x80\x7f----------\"");
        TEST_BAD("\"\xe1\x7f\x80----------\"");
        TEST_BAD("\"\xec\xff\x80----------\"");
        TEST_BAD("\"\xef\x7f\x80----------\"");
        TEST_BAD("\"\xe1\x80\xff----------\"");
        TEST_BAD("\"\xe1\xbf\x0f----------\"");
        TEST_BAD("\"\xec\x01\xff----------\"");
        TEST_BAD("\"\xec\xff\xff----------\"");

        TEST_BAD("\"\xe6\x60\x90----------\"");

        TEST_BAD("\"\xed\x7f\x80----------\"");
        TEST_BAD("\"\xed\xa0\xbf----------\"");
        TEST_BAD("\"\xed\xbf\x80----------\"");
        TEST_BAD("\"\xed\x9f\x7f----------\"");

        TEST_BAD("\"\xed\xce\xbf----------\"");

        TEST_BAD("\"\xee\x7f\x80----------\"");
        TEST_BAD("\"\xee\xcc\x80----------\"");
        TEST_BAD("\"\xef\x80\xcc----------\"");
        TEST_BAD("\"\xef\xbf\x0a----------\"");
        TEST_BAD("\"\xee\x50\xbf----------\"");
        TEST_BAD("\"\xee\xef\xbf----------\"");
        TEST_BAD("\"\xef\xf0\xff----------\"");
        TEST_BAD("\"\xef\xaa\xee----------\"");

        TEST_BAD("\"\xc0\x90\x90----------\"");
        TEST_BAD("\"\xc1\x90\x90----------\"");

        TEST_BAD("\"\xff\x90\x80\x80----------\"");
        TEST_BAD("\"\xfe\x90\xbf\x80----------\"");
        TEST_BAD("\"\xfd\x90\xbf\xbf----------\"");
        TEST_BAD("\"\xf0\xff\x80\xbf----------\"");
        TEST_BAD("\"\xf0\xfe\x80\x80----------\"");
        TEST_BAD("\"\xf0\xfd\xbf\x80----------\"");
        TEST_BAD("\"\xf0\x90\x80\xff----------\"");
        TEST_BAD("\"\xf0\x90\x5f\x80----------\"");

        TEST_BAD("\"\xf4\x70\x80\x80----------\"");
        TEST_BAD("\"\xf4\x80\x70\x80----------\"");
        TEST_BAD("\"\xf4\x80\xbf\x70----------\"");
        TEST_BAD("\"\xf4\xce\x80\xbf----------\"");
        TEST_BAD("\"\xf4\x8f\xce\x80----------\"");
        TEST_BAD("\"\xf4\x8f\xbf\xce----------\"");

        TEST_BAD("\"\xf1\x7f\xbf\xbf----------\"");
        TEST_BAD("\"\xf2\x80\x7f\xbf----------\"");
        TEST_BAD("\"\xf3\x80\xbf\xce----------\"");

        // utf8 after escape
        TEST_GOOD("\"\\u0000 \xf3\xbf\x80\xbf\xf3\xbf\x80\xbf\"");
        TEST_GOOD("\"\\ud7ff\xf4\x80\xbf\xbf       \"");
        TEST_GOOD("\"\\ue000            \xef\xbf\x80\"");
        TEST_GOOD("\"\xef\xbf\x80 \\uffff \xef\xbf\x80\"");
        TEST_GOOD("\"\xc2\x80\xc2\x80\xc2\x80\xc2\x80\xc2\x80\\ud800\\udc00 \"");
        TEST_GOOD("\"\\udbff\\udfff \xe1\x80\xbf  \\udbff\\udfff \xe1\x80\xbf\"");
        TEST_GOOD("\"\\u0000\xe1\x80\xbf     \"");
        TEST_BAD("\"\\t\\t\xf4\x70\x80\x80----------\"");
        TEST_BAD("\"\\n\xf4\x80\x70\x80----------\"");
        TEST_BAD("\"\\n\xf4\x80\xbf\x70-\\n\xf4\x80\xbf\x70\"");

        const auto check =
            [this](string_view expected)
        {
            TEST_GOOD(expected);
            for (std::size_t write_size : {2, 4, 8})
            {
                utf8_parser p;
                for(std::size_t i = 0; i < expected.size(); i += write_size)
                {
                    error_code ec;
                    write_size = (std::min)(write_size, expected.size() - i);
                    auto more = (i < expected.size() - write_size);
                    auto written = p.write(more,
                            expected.data() + i, write_size, ec);
                    BOOST_TEST(written == write_size);
                    BOOST_TEST(! ec);
                }
                BOOST_TEST(p.captured() ==
                    expected.substr(1, expected.size() - 2));
            }
        };

        check("\"\xd1\x82\"");
        check("\"\xd1\x82\xd0\xb5\xd1\x81\xd1\x82\"");
        check("\"\xc3\x0b1""and\xc3\xba\"");
    }

    void
    testMaxDepth()
    {
        {
            string_view s = "[[[[[]]]]]";
            parse_options opt;
            opt.max_depth = 4;
            null_parser p(opt);
            error_code ec;
            p.write(s.data(), s.size(), ec);
            BOOST_TEST(ec == error::too_deep);
            BOOST_TEST(ec.has_location());
        }
        {
            string_view s = "[[[[]]], [[[[]]]]]";
            parse_options opt;
            opt.max_depth = 4;
            null_parser p(opt);
            error_code ec;
            p.write(s.data(), s.size(), ec);
            BOOST_TEST(ec == error::too_deep);
            BOOST_TEST(ec.has_location());
        }
        {
            string_view s =
                "{\"a\":{\"b\":{\"c\":{}}},\"b\":{\"c\":{\"d\":{\"e\":{}}}}}";
            parse_options opt;
            opt.max_depth = 4;
            null_parser p(opt);
            error_code ec;
            p.write(s.data(), s.size(), ec);
            BOOST_TEST(ec == error::too_deep);
            BOOST_TEST(ec.has_location());
        }
        {
            string_view s =
                "{\"a\":{\"b\":{\"c\":{\"d\":{}}}}}";
            parse_options opt;
            opt.max_depth = 4;
            null_parser p(opt);
            error_code ec;
            p.write(s.data(), s.size(), ec);
            BOOST_TEST(ec == error::too_deep);
            BOOST_TEST(ec.has_location());
        }
    }

    class literal_parser
    {
        struct handler
        {
            constexpr static std::size_t max_object_size = std::size_t(-1);
            constexpr static std::size_t max_array_size = std::size_t(-1);
            constexpr static std::size_t max_key_size = std::size_t(-1);
            constexpr static std::size_t max_string_size = std::size_t(-1);

            std::string captured = "";
            bool on_document_begin( error_code& ) { return true; }
            bool on_document_end( error_code& ) { return true; }
            bool on_object_begin( error_code& ) { return true; }
            bool on_object_end( std::size_t, error_code& ) { return true; }
            bool on_array_begin( error_code& ) { return true; }
            bool on_array_end( std::size_t, error_code& ) { return true; }
            bool on_key_part( string_view, std::size_t, error_code& ) { return true; }
            bool on_key( string_view, std::size_t, error_code& ) { return true; }
            bool on_string_part( string_view, std::size_t, error_code& ) { return true; }
            bool on_string( string_view, std::size_t, error_code& ) { return true; }
            bool on_number_part( string_view sv, error_code&)
            {
                captured.append(sv.data(), sv.size());
                return true;
            }
            bool on_int64( std::int64_t, string_view sv, error_code& )
            {
                captured.append(sv.data(), sv.size());
                captured += 's';
                return true;
            }
            bool on_uint64( std::uint64_t, string_view sv, error_code& )
            {
                captured.append(sv.data(), sv.size());
                captured += 'u';
                return true;
            }
            bool on_double( double, string_view sv, error_code& )
            {
                captured.append(sv.data(), sv.size());
                captured += 'd';
                return true;
            }
            bool on_bool( bool, error_code& ) { return true; }
            bool on_null( error_code& ) { return true; }
            bool on_comment_part( string_view, error_code& ) { return true; }
            bool on_comment( string_view, error_code& ) { return true; }
        };

        basic_parser<handler> p_;

    public:
        literal_parser()
            : p_(make_options(true, false, false))
        {
        }

        std::size_t
        write(
            bool more,
            char const* data,
            std::size_t size,
            error_code& ec)
        {
            auto const n = p_.write_some(
                more, data, size, ec);
            if(! ec && n < size)
                ec = error::extra_data;
            return n;
        }

        string_view
        captured()
        {
            return p_.handler().captured;
        }
    };

    void
    testNumberLiteral()
    {
        const auto check =
        [](string_view expected)
        {
            string_view sv = expected;
            sv.remove_suffix(1);
            for(std::size_t i = 0;
                i < sv.size(); ++i)
            {
                literal_parser p;
                error_code ec;
                if(i != 0)
                {
                    p.write(true,
                        sv.data(), i, ec);
                }
                if(BOOST_TEST(! ec))
                {
                    p.write(false,
                        sv.data() + i,
                        sv.size() - i, ec);
                }
                BOOST_TEST(! ec);
                BOOST_TEST(p.captured() == expected);
            }
        };

        check("1s");
        check("-1s");
        check("0s");
        check("0s");
        check("123456s");
        check("-123456s");
        check("9223372036854775808u");

        check("1.0d");
        check("-1.0d");
        check("0.0d");
        check("1.0e3d");
        check("1e1d");
        check("1e+1d");
        check("1e-1d");
        check("-100000000000000000000000d");
        check("100000000000000000000000d");
        check("10000000000.10000000000000d");
        check("-10000000000.10000000000000d");
        check("1000000000000000.0e1000000d");
    }

    void
    testStickyErrors()
    {
        {
            null_parser p;
            error_code ec;
            p.write("*", 1, ec);
            BOOST_TEST(ec);
            error_code ec2;
            p.write("[]", 2, ec2);
            BOOST_TEST(ec2 == ec);
            p.reset();
            p.write("[]", 2, ec2);
            BOOST_TEST(! ec2);
        }

        // exceptions do not cause UB
        {
            throw_parser p(1);
            try
            {
                error_code ec;
                p.write(false, "null", 4, ec);
                BOOST_TEST_FAIL();
            }
            catch(std::exception const&)
            {
                error_code ec;
                p.write(false, "null", 4, ec);
                BOOST_TEST(ec == error::exception);
                BOOST_TEST(ec.has_location());
            }
        }
    }

#ifndef BOOST_NO_CXX17_HDR_STRING_VIEW
    using SV = std::string_view;
#else
    using SV = string_view;
#endif
    // The null parser, but uses std type equivalents
    class std_null_parser
    {
        struct handler
        {
            constexpr static std::size_t max_object_size = std::size_t(-1);
            constexpr static std::size_t max_array_size = std::size_t(-1);
            constexpr static std::size_t max_key_size = std::size_t(-1);
            constexpr static std::size_t max_string_size = std::size_t(-1);

            bool on_document_begin( std::error_code& ) { return true; }
            bool on_document_end( std::error_code& ) { return true; }
            bool on_object_begin( std::error_code& ) { return true; }
            bool on_object_end( std::size_t, std::error_code& ) { return true; }
            bool on_array_begin( std::error_code& ) { return true; }
            bool on_array_end( std::size_t, std::error_code& ) { return true; }
            bool on_key_part( SV, std::size_t, std::error_code& ) { return true; }
            bool on_key( SV, std::size_t, std::error_code& ) { return true; }
            bool on_string_part( SV, std::size_t, std::error_code& ) { return true; }
            bool on_string( SV, std::size_t, std::error_code& ) { return true; }
            bool on_number_part( SV, std::error_code&) { return true; }
            bool on_int64( std::int64_t, SV, std::error_code& ) { return true; }
            bool on_uint64( std::uint64_t, SV, std::error_code& ) { return true; }
            bool on_double( double, SV, std::error_code& ) { return true; }
            bool on_bool( bool, std::error_code& ) { return true; }
            bool on_null( std::error_code& ) { return true; }
            bool on_comment_part( SV, std::error_code& ) { return true; }
            bool on_comment( SV, std::error_code& ) { return true; }
        };

        basic_parser<handler> p_;

    public:
        std_null_parser()
            : p_(parse_options())
        {
        }

        explicit
        std_null_parser(parse_options po)
            : p_(po)
        {
        }

        void
        reset()
        {
            p_.reset();
        }

        std::size_t
        write(
            SV s,
            std::error_code& ec)
        {
            auto const n = p_.write_some(
                false, s.data(), s.size(), ec);
            if(! ec && n < s.size())
                ec = error::extra_data;
            return n;
        }
    };

    void testStdTypes()
    {
        std_null_parser p;
        std::error_code ec;
        std::string const doc = "{}";
        p.write(doc, ec);
        if(! BOOST_TEST(! ec))
        {
            log << "    failed to parse: " << doc << '\n';
            return;
        }
    }

#ifdef _MSC_VER
#pragma warning(pop)
#endif

    void
    run()
    {
        testNull();
        testBoolean();
        testString();
        testNumber();
        testArray();
        testObject();
        testParser();
        testMembers();
        testParseVectors();
        testIssue13();
        testIssue20();
        testIssue113();
        testAllowTrailing();
        testComments();
        testUTF8Validation();
        testMaxDepth();
        testNumberLiteral();
        testStickyErrors();
        testStdTypes();
    }
};

TEST_SUITE(basic_parser_test, "boost.json.basic_parser");

BOOST_JSON_NS_END
