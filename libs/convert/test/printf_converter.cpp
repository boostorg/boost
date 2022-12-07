// Boost.Convert test and usage example
// Copyright (c) 2009-2020 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#include "./test.hpp"

#if defined(BOOST_CONVERT_IS_NOT_SUPPORTED)
int main(int, char const* []) { return 0; }
#else

#include <boost/convert.hpp>
#include <boost/convert/printf.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>

using std::string;
using boost::convert;

namespace cnv = boost::cnv;
namespace arg = boost::cnv::parameter;

static
void
test_notation()
{
    //[printf_notation
    boost::cnv::printf cnv;

    BOOST_TEST(    "-3.14159" == convert<string>(-3.14159, cnv(arg::notation = cnv::notation::fixed)(arg::precision = 5)).value());
    BOOST_TEST(  "-3.142e+00" == convert<string>(-3.14159, cnv(arg::notation = cnv::notation::scientific)(arg::precision = 3)).value());
    BOOST_TEST("-0x1.9220p+1" == convert<string>(-3.14159, cnv(arg::notation = cnv::notation::hex)(arg::precision = 4)).value());

    const auto close = boost::math::fpc::close_at_tolerance<double>(1);

    BOOST_TEST_WITH(-3.14159, convert<double>("-3.14159", cnv(arg::notation = cnv::notation::fixed)).value(), close);
    BOOST_TEST_WITH(-3.14159, convert<double>("-3.142e+00", cnv(arg::notation = cnv::notation::scientific)).value(), close);
    BOOST_TEST_WITH(-3.14159, convert<double>("-0x1.9220p+1", cnv(arg::notation = cnv::notation::hex)).value(), close);
    //]
}

int
main(int, char const* [])
{
    boost::cnv::printf cnv;

    string const not_int_str = "not an int";
    string const     std_str = "-11";
    char const* const  c_str = "-12";

    BOOST_TEST( -1 == convert<int>(not_int_str, cnv).value_or(-1));
    BOOST_TEST(-11 == convert<int>(std_str,     cnv).value_or(-1));
    BOOST_TEST(-12 == convert<int>(c_str,       cnv).value_or(-1));

    BOOST_TEST( float(3.14) == convert< float>("3.14", cnv(arg::precision = 2)).value_or(-1));
    BOOST_TEST(double(3.14) == convert<double>("3.14", cnv(arg::precision = 2)).value_or(-1));

    BOOST_TEST("255" == convert<std::string>(255, cnv(arg::base = boost::cnv::base::dec)).value());
    BOOST_TEST( "ff" == convert<std::string>(255, cnv(arg::base = boost::cnv::base::hex)).value());
    BOOST_TEST("377" == convert<std::string>(255, cnv(arg::base = boost::cnv::base::oct)).value());

    string s01 = convert<string>(12.3456, cnv(arg::precision = 6)).value();
    string s02 = convert<string>(12.3456, cnv(arg::precision = 3)).value();

    BOOST_TEST(s01 == "12.345600");
    BOOST_TEST(s02 == "12.346");

    test_notation();

    return boost::report_errors();
}

#endif
