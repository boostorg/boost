//
// Copyright (c) 2021 Evan Lenz (evan@lenzconsulting.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXAMPLE_ISSUE_78_HPP
#define EXAMPLE_ISSUE_78_HPP

namespace example {

/** Issue 78

    foo() should show up under the heading "Static Member Functions"
    and bar should show up under the heading "Static Members"
*/
class issue_78
{
public:
    static foo();
    static bar;
};

} // example

#endif
