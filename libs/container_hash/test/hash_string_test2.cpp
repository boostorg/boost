// Copyright 2022 Peter Dimov.
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/utility/string_view.hpp>
#include <boost/core/detail/string_view.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/config.hpp>
#include <string>
#if !defined(BOOST_NO_CXX17_HDR_STRING_VIEW)
#include <string_view>
#endif

// Test whether the hash values of a string and a
// string_view that refers to it match. This is
// important for unordered heterogeneous lookups.

template<class T> std::size_t hv( T const& t )
{
    return boost::hash<T>()( t );
}

int main()
{
    std::string s( "Test." );

    BOOST_TEST_EQ( hv( s ), hv( boost::string_view( s ) ) );
    BOOST_TEST_EQ( hv( s ), hv( boost::core::string_view( s ) ) );

#if !defined(BOOST_NO_CXX17_HDR_STRING_VIEW)
    BOOST_TEST_EQ( hv( s ), hv( std::string_view( s ) ) );
#endif

    return boost::report_errors();
}
