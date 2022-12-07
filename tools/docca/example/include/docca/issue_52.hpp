//
// Copyright (c) 2020 Evan Lenz (evan@lenzconsulting.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXAMPLE_ISSUE_52_HPP
#define EXAMPLE_ISSUE_52_HPP

namespace example {

/** Issue 52

    Constructor and destructor are annotated as such.
*/
class issue_52
{
public:
    /** Default constructor.

        Description
    */
    issue_52();

    /** Destructor.

        Description
    */
    ~issue_52();
};

} // example

#endif
