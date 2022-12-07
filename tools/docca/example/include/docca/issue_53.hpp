//
// Copyright (c) 2020 Evan Lenz (evan@lenzconsulting.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXAMPLE_ISSUE_53_HPP
#define EXAMPLE_ISSUE_53_HPP

namespace example {

/** Issue 53

    @li Distinct constructor descriptions
    should be separated by a silver em dash.
*/
class issue_53
{
    int i_;

public:
    /** Default constructor.

        Description.
    */
    issue_53();

    /** Constructor.

        Description.
    */
    issue_53(char);

    /** Void constructor

        Description.
    */
    issue_53(void*);
};

} // example

#endif
