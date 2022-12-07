//
// Copyright (c) 2021 Evan Lenz (evan@lenzconsulting.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXAMPLE_ISSUE_69_HPP
#define EXAMPLE_ISSUE_69_HPP

namespace example {

/** Issue 69

    The brackets in the @ref operator[] page's
    code block should not appear escaped. Nor
    should they in that link just there (in this
    sentence).
*/
struct issue_69
{
    operator[](string_view key);
};

} // example

#endif
