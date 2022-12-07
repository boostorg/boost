// Copyright 2021, 2022 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// http://www.boost.org/LICENSE_1_0.txt

#include <boost/system/error_code.hpp>
#include <boost/system/error_category.hpp>
#include <boost/system/errc.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/config/pragma_message.hpp>
#include <boost/config.hpp>
#include <cerrno>

#if !defined(BOOST_SYSTEM_HAS_SYSTEM_ERROR)

BOOST_PRAGMA_MESSAGE( "BOOST_SYSTEM_HAS_SYSTEM_ERROR not defined, test will be skipped" )
int main() {}

#else

#include <system_error>

enum my_errc
{
    my_enoent = ENOENT
};

class my_category: public boost::system::error_category
{
public:

    char const* name() const BOOST_NOEXCEPT
    {
        return "mycat";
    }

    boost::system::error_condition default_error_condition( int ev ) const BOOST_NOEXCEPT
    {
        switch( ev )
        {
        case my_enoent:

            return boost::system::error_condition( ENOENT, boost::system::generic_category() );

        default:

            return boost::system::error_condition( ev, *this );
        }
    }

    std::string message( int ev ) const
    {
        switch( ev )
        {
        case my_enoent:

            return "No such entity";

        default:

            return "Unknown error";
        }
    }
};

#if defined(BOOST_GCC) && BOOST_GCC < 70000

// g++ 6 and earlier do not allow specializations outside the namespace

namespace boost
{
namespace system
{

template<> struct is_error_code_enum<my_errc>: std::true_type {};

} // namespace system
} // namespace boost

namespace std
{

template<> struct is_error_code_enum<my_errc>: std::true_type {};

} // namespace std

#else

template<> struct boost::system::is_error_code_enum<my_errc>: std::true_type {};
template<> struct std::is_error_code_enum<my_errc>: std::true_type {};

#endif

boost::system::error_code make_error_code( my_errc e )
{
    // If `cat` is declared constexpr or const, msvc-14.1 and
    // msvc-14.2 before 19.29 put it in read-only memory,
    // despite the `ps_` member being mutable. So it crashes.

    static /*BOOST_SYSTEM_CONSTEXPR*/ my_category cat;
    return boost::system::error_code( e, cat );
}

int main()
{
    {
        boost::system::error_code e1 = my_enoent;

        BOOST_TEST( e1 == my_enoent );
        BOOST_TEST_NOT( e1 != my_enoent );

        BOOST_TEST( e1 == boost::system::errc::no_such_file_or_directory );
        BOOST_TEST( e1 == std::errc::no_such_file_or_directory );
    }

    {
        std::error_code e1 = my_enoent;

        BOOST_TEST( e1 == my_enoent );
        BOOST_TEST_NOT( e1 != my_enoent );

        BOOST_TEST( e1 == std::errc::no_such_file_or_directory );
    }

    return boost::report_errors();
}

#endif
