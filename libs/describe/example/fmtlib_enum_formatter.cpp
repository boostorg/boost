// Copyright 2022 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/describe.hpp>
#include <fmt/format.h>
#include <type_traits>

template<class T> struct fmt::formatter<T, char, std::enable_if_t<
    boost::describe::has_describe_enumerators<T>::value>>
{
private:

    using U = std::underlying_type_t<T>;

    fmt::formatter<fmt::string_view, char> sf_;
    fmt::formatter<U, char> nf_;

public:

    constexpr auto parse( format_parse_context& ctx )
    {
        auto i1 = sf_.parse( ctx );
        auto i2 = nf_.parse( ctx );

        if( i1 != i2 )
        {
            ctx.error_handler().on_error( "invalid format" );
        }

        return i1;
    }

    auto format( T const& t, format_context& ctx ) const
    {
        char const * s = boost::describe::enum_to_string( t, 0 );

        if( s )
        {
            return sf_.format( s, ctx );
        }
        else
        {
            return nf_.format( static_cast<U>( t ), ctx );
        }
    }
};

enum E1
{
    v1, v2, v3 = 11
};

BOOST_DESCRIBE_ENUM( E1, v1, v2, v3 )

int main()
{
    fmt::print( "{:_^10}\n", E1::v1 );
    fmt::print( "{:_^10}\n", (E1)7 );
}
