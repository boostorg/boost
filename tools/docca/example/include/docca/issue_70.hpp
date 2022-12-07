//
// Copyright (c) 2021 Evan Lenz (evan@lenzconsulting.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXAMPLE_ISSUE_70_HPP
#define EXAMPLE_ISSUE_70_HPP

namespace example {

/** Issue 70

    This reference, @ref issue_70_b::foo, should link to the issue_70_b page
    (not issue_63).
*/
enum class issue_70
{
    foo,
    bar
};

enum class issue_70_b
{
    foo,
    bar
};

} // example

#endif
