// Copyright 2021, 2022 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/system.hpp>
#include <boost/core/lightweight_test.hpp>
#include <cerrno>

int main()
{
    int const val = ENOENT;
    boost::system::error_category const & cat = boost::system::generic_category();

    {
        boost::system::error_code ec;
        boost::system::error_code ec2( ec );

        BOOST_TEST( !ec.has_location() );
        BOOST_TEST_EQ( ec.location(), boost::source_location() );

        BOOST_STATIC_CONSTEXPR boost::source_location loc = BOOST_CURRENT_LOCATION;

        ec.assign( ec, &loc );

        BOOST_TEST_EQ( ec, ec2 );

        BOOST_TEST( !ec.has_location() );
        BOOST_TEST_EQ( ec.location(), boost::source_location() );

        ec.assign( ec, 0 );

        BOOST_TEST_EQ( ec, ec2 );

        BOOST_TEST( !ec.has_location() );
        BOOST_TEST_EQ( ec.location(), boost::source_location() );
    }

    {
        boost::system::error_code ec( val, cat );
        boost::system::error_code ec2( ec );

        BOOST_TEST( !ec.has_location() );
        BOOST_TEST_EQ( ec.location(), boost::source_location() );

        BOOST_STATIC_CONSTEXPR boost::source_location loc = BOOST_CURRENT_LOCATION;

        ec.assign( ec, &loc );

        BOOST_TEST_EQ( ec, ec2 );

        BOOST_TEST( ec.has_location() );
        BOOST_TEST_EQ( ec.location(), loc );

        ec.assign( ec, 0 );

        BOOST_TEST_EQ( ec, ec2 );

        BOOST_TEST( !ec.has_location() );
        BOOST_TEST_EQ( ec.location(), boost::source_location() );
    }

#if defined(BOOST_SYSTEM_HAS_SYSTEM_ERROR)

    {
        std::error_code e2( val, std::generic_category() );

        boost::system::error_code ec( e2 );
        boost::system::error_code ec2( ec );

        BOOST_TEST( !ec.has_location() );
        BOOST_TEST_EQ( ec.location(), boost::source_location() );

        BOOST_STATIC_CONSTEXPR boost::source_location loc = BOOST_CURRENT_LOCATION;

        ec.assign( ec, &loc );

        BOOST_TEST_EQ( ec, ec2 );

        BOOST_TEST( !ec.has_location() );
        BOOST_TEST_EQ( ec.location(), boost::source_location() );

        ec.assign( ec, 0 );

        BOOST_TEST_EQ( ec, ec2 );

        BOOST_TEST( !ec.has_location() );
        BOOST_TEST_EQ( ec.location(), boost::source_location() );
    }

#endif

    return boost::report_errors();
}
