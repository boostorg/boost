// Copyright 2022 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/describe.hpp>
#include <boost/mp11.hpp>
#include <fmt/format.h>
#include <type_traits>

template<class T> struct fmt::formatter<T, char, std::enable_if_t<
    boost::describe::has_describe_bases<T>::value &&
    boost::describe::has_describe_members<T>::value &&
    !std::is_union<T>::value>>
{
    constexpr auto parse( format_parse_context& ctx )
    {
        auto it = ctx.begin(), end = ctx.end();

        if( it != end && *it != '}' )
        {
            ctx.error_handler().on_error( "invalid format" );
        }

        return it;
    }

    auto format( T const& t, format_context& ctx ) const
    {
        using namespace boost::describe;

        using Bd = describe_bases<T, mod_any_access>;
        using Md = describe_members<T, mod_any_access>;

        auto out = ctx.out();

        *out++ = '{';

        bool first = true;

        boost::mp11::mp_for_each<Bd>([&](auto D){

            if( !first )
            {
                *out++ = ',';
            }

            first = false;

            out = fmt::format_to( out, " {}",
                (typename decltype(D)::type const&)t );
        });

        boost::mp11::mp_for_each<Md>([&](auto D){

            if( !first )
            {
                *out++ = ',';
            }

            first = false;

            out = fmt::format_to( out, " .{}={}",
                D.name, t.*D.pointer );
        });

        if( !first )
        {
            *out++ = ' ';
        }

        *out++ = '}';

        return out;
    }
};

struct point
{
    int x, y;
};

BOOST_DESCRIBE_STRUCT( point, (), (x, y) )

struct color
{
    unsigned char r, g, b;
};

BOOST_DESCRIBE_STRUCT( color, (), (r, g, b) )

struct line: color
{
    point first, last;
};

BOOST_DESCRIBE_STRUCT( line, (color), (first, last) )

int main()
{
    fmt::print( "{}\n", line{ { 255, 192, 16 }, { 1, 2 }, { 3, 4 } } );
}
