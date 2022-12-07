// Copyright 2021 Peter Dimov
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/core/detail/string_view.hpp>
#include <boost/core/lightweight_test_trait.hpp>
#include <boost/config/pragma_message.hpp>
#include <boost/config.hpp>
#include <string>

#if defined(BOOST_NO_CXX20_HDR_CONCEPTS)

BOOST_PRAGMA_MESSAGE( "Skipping test because BOOST_NO_CXX20_HDR_CONCEPTS is defined" )
int main() {}

#else

#include <concepts>
#include <type_traits>

using T = std::common_reference_t< boost::core::string_view&&, std::string& >;

static_assert( std::common_reference_with< boost::core::string_view&&, std::string& > );
static_assert( std::common_reference_with< boost::core::wstring_view&&, std::wstring const& > );

int main()
{
    BOOST_TEST_TRAIT_SAME( std::common_reference_t< boost::core::string_view&&, std::string& >, boost::core::string_view );
    BOOST_TEST_TRAIT_SAME( std::common_reference_t< boost::core::wstring_view&&, std::wstring const& >, boost::core::wstring_view );

    return boost::report_errors();
}

#endif
