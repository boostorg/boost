// Copyright 2021 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/core/detail/string_view.hpp>
#include <boost/config/pragma_message.hpp>
#include <string>
#include <iterator>

#if !defined(__cpp_lib_ranges)

BOOST_PRAGMA_MESSAGE( "Skipping test because __cpp_lib_ranges is not defined" )

#else

struct Iterator
{
    using value_type = std::string;
    boost::core::string_view operator*() const noexcept;
};

static_assert( std::indirectly_readable<Iterator> );

#endif
