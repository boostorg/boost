//
// Copyright (c) 2019 Vinnie Falco (vinnie.falco@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Official repository: https://github.com/boostorg/json
//

#ifndef BOOST_JSON_ERROR_HPP
#define BOOST_JSON_ERROR_HPP

#include <boost/json/detail/config.hpp>
#include <boost/json/system_error.hpp>

BOOST_JSON_NS_BEGIN

/** Error codes returned by JSON operations

*/
enum class error
{
    //
    // parse errors
    //

    /// syntax error
    syntax = 1,

    /// extra data
    extra_data,

    /// incomplete JSON
    incomplete,

    /// exponent too large
    exponent_overflow,

    /// too deep
    too_deep,

    /// illegal leading surrogate
    illegal_leading_surrogate,

    /// illegal trailing surrogate
    illegal_trailing_surrogate,

    /// expected hex digit
    expected_hex_digit,

    /// expected utf16 escape
    expected_utf16_escape,

    /// An object contains too many elements
    object_too_large,

    /// An array contains too many elements
    array_too_large,

    /// A key is too large
    key_too_large,

    /// A string is too large
    string_too_large,

    /// The parser encountered an exception and must be reset
    exception,

    //----------------------------------

    /// not a number
    not_number,

    /// number cast is not exact
    not_exact,

    /// test failure
    test_failure,

    //
    // JSON Pointer errors
    //

    /// missing slash character before token reference
    missing_slash,

    /// invalid escape sequence
    invalid_escape,

    /// token should be a number but cannot be parsed as such
    token_not_number,

    /// current value is neither an object nor an array
    value_is_scalar,

    /// current value does not contain referenced value
    not_found,

    /// token cannot be represented by std::size_t
    token_overflow,

    /// past-the-end index is not supported
    past_the_end,
};

/** Error conditions corresponding to JSON errors
*/
enum class condition
{
    /// A parser-related error
    parse_error = 1,

    /// An error on assignment to or from a JSON value
    assign_error,

    /// An error related to parsing JSON pointer string
    pointer_parse_error,

    /// An error related to applying JSON pointer string to a value
    pointer_use_error,
};

BOOST_JSON_NS_END

#include <boost/json/impl/error.hpp>

#endif
