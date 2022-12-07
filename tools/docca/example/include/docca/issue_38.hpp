//
// Copyright (c) 2020 Evan Lenz (evan@lenzconsulting.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXAMPLE_ISSUE_38_HPP
#define EXAMPLE_ISSUE_38_HPP

namespace example {

/** Issue 38

    Open:      [a, b]
    Half-open: [a, b)
    Closed:    (a, b)

    @li Gracefully handle brackets in javadoc comments
*/
class issue_38
{
    int i_;

public:
    /** Half-open: [a, b)

        first to last exclusive, i.e. [ first, last )
    */
    issue_38();
};

} // example

#endif
