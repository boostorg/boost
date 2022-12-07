//
// Copyright (c) 2012 Artyom Beilis (Tonkikh)
// Copyright (c) 2019-2020 Alexander Grund
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_NOWIDE_LIB_TEST_H_INCLUDED
#define BOOST_NOWIDE_LIB_TEST_H_INCLUDED

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

#if defined(_MSC_VER) && defined(_CPPLIB_VER) && defined(_DEBUG)
#include <crtdbg.h>
#endif

namespace boost {
namespace nowide {
    namespace test {
        struct test_error : std::runtime_error
        {
            using std::runtime_error::runtime_error;
        };
        struct test_monitor
        {
            test_monitor()
            {
#if defined(_MSC_VER) && (_MSC_VER > 1310)
                // disable message boxes on assert(), abort()
                ::_set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#endif
#if defined(_MSC_VER) && defined(_CPPLIB_VER) && defined(_DEBUG)
                // disable message boxes on iterator debugging violations
                _CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
                _CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDERR);
#endif
            }
            std::string context;
        };
        inline test_monitor& test_mon()
        {
            static test_monitor instance;
            return instance;
        }
        struct context
        {
            std::string oldCtx;
            context(std::string ctx) : oldCtx(std::move(test_mon().context))
            {
                test_mon().context = std::move(ctx);
            }
            ~context()
            {
                test_mon().context = std::move(oldCtx);
            }
        };
        /// Function called when a test failed to be able set a breakpoint for debugging
        inline void test_failed(const char* expr, const char* file, const int line, const char* function)
        {
            std::ostringstream ss;
            ss << expr << " at " << file << ':' << line << " in " << function;
            std::string& ctx = test_mon().context;
            if(!ctx.empty())
                ss << " context: " << ctx;
            throw test_error(ss.str());
        }

        template<typename T>
        const T& print_value(const T& value)
        {
            return value;
        }
        inline std::string print_value(const std::wstring& value)
        {
            std::ostringstream ss;
            const auto defWidth = ss.width();
            ss << std::hex << std::uppercase << std::setfill('0');
            for(const wchar_t c : value)
            {
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wtautological-constant-out-of-range-compare"
#endif
                if(c >= (std::numeric_limits<char>::min)() && c <= (std::numeric_limits<char>::max)())
                    ss << static_cast<char>(c);
                else
                    ss << "\\" << std::setw(sizeof(wchar_t) * 2) << static_cast<int>(c) << std::setw(defWidth);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
            }
            return ss.str();
        }

        template<typename T, typename U>
        void test_equal_impl(const T& lhs, const U& rhs, const char* file, const int line, const char* function)
        {
            if(lhs == rhs)
                return;
            std::ostringstream ss;
            ss << "[" << print_value(lhs) << "!=" << print_value(rhs) << "]";
            test_failed(ss.str().c_str(), file, line, function);
        }
    } // namespace test
} // namespace nowide
} // namespace boost

#ifdef _MSC_VER
#define DISABLE_CONST_EXPR_DETECTED __pragma(warning(push)) __pragma(warning(disable : 4127))
#define DISABLE_CONST_EXPR_DETECTED_POP __pragma(warning(pop))
#else
#define DISABLE_CONST_EXPR_DETECTED
#define DISABLE_CONST_EXPR_DETECTED_POP
#endif

#define TEST(x)                                            \
    do                                                     \
    {                                                      \
        using namespace boost::nowide::test;               \
        test_mon();                                        \
        if(x)                                              \
            break;                                         \
        test_failed(#x, __FILE__, __LINE__, __FUNCTION__); \
        DISABLE_CONST_EXPR_DETECTED                        \
    } while(0) DISABLE_CONST_EXPR_DETECTED_POP
#define TEST_EQ(lhs, rhs)                                                \
    do                                                                   \
    {                                                                    \
        using namespace boost::nowide::test;                             \
        test_mon();                                                      \
        test_equal_impl((lhs), (rhs), __FILE__, __LINE__, __FUNCTION__); \
        break;                                                           \
        DISABLE_CONST_EXPR_DETECTED                                      \
    } while(0) DISABLE_CONST_EXPR_DETECTED_POP

#define TEST_THROW(expr, error)                                                  \
    do                                                                           \
    {                                                                            \
        test_mon();                                                              \
        try                                                                      \
        {                                                                        \
            expr;                                                                \
            test_failed(#error " not thrown", __FILE__, __LINE__, __FUNCTION__); \
        } catch(const error&)                                                    \
        { /* OK */                                                               \
        }                                                                        \
        break;                                                                   \
        DISABLE_CONST_EXPR_DETECTED                                              \
    } while(0) DISABLE_CONST_EXPR_DETECTED_POP

#define TEST_CONTEXT(expr)                       \
    boost::nowide::test::context _##__COUNTER__( \
      static_cast<const std::stringstream&>(std::stringstream{} << expr).str())

#ifndef BOOST_NOWIDE_TEST_NO_MAIN
// Tests should implement this
void test_main(int argc, char** argv, char** env);

int main(int argc, char** argv, char** env)
{
    try
    {
#ifdef _MSC_VER
        std::cout << "MSVC version " << _MSC_VER << std::endl;
#endif
        test_main(argc, argv, env);
    } catch(const boost::nowide::test::test_error& e)
    {
        std::cerr << "Failed test assertion: " << e.what() << std::endl;
        return 1;
    } catch(const std::exception& e)
    {
        std::cerr << "Failed with unexpected exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
#endif

#endif // #ifndef BOOST_NOWIDE_LIB_TEST_H_INCLUDED
