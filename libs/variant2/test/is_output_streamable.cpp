// Copyright 2021 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/variant2/variant.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <ostream>
#include <string>

struct X
{
};

struct Y
{
};

std::ostream& operator<<( std::ostream& os, Y const& /*y*/ )
{
    os << "Y()";
    return os;
}

int main()
{
    BOOST_TEST_TRAIT_TRUE((boost::variant2::detail::is_output_streamable<std::ostream, int>));
    BOOST_TEST_TRAIT_TRUE((boost::variant2::detail::is_output_streamable<std::ostream, float>));
    BOOST_TEST_TRAIT_TRUE((boost::variant2::detail::is_output_streamable<std::ostream, std::string>));
    BOOST_TEST_TRAIT_FALSE((boost::variant2::detail::is_output_streamable<std::ostream, void>));
    BOOST_TEST_TRAIT_FALSE((boost::variant2::detail::is_output_streamable<std::ostream, X>));
    BOOST_TEST_TRAIT_TRUE((boost::variant2::detail::is_output_streamable<std::ostream, Y>));

    BOOST_TEST_TRAIT_TRUE((boost::variant2::detail::is_output_streamable<std::ostream, boost::variant2::monostate>));

    BOOST_TEST_TRAIT_TRUE((boost::variant2::detail::is_output_streamable<std::ostream, boost::variant2::variant<int, float, std::string>>));
    BOOST_TEST_TRAIT_FALSE((boost::variant2::detail::is_output_streamable<std::ostream, boost::variant2::variant<int, float, X>>));
    BOOST_TEST_TRAIT_TRUE((boost::variant2::detail::is_output_streamable<std::ostream, boost::variant2::variant<int, float, Y>>));
    BOOST_TEST_TRAIT_TRUE((boost::variant2::detail::is_output_streamable<std::ostream, boost::variant2::variant<boost::variant2::monostate, int, float>>));

    return boost::report_errors();
}
