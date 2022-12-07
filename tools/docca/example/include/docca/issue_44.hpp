//
// Copyright (c) 2020 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXAMPLE_ISSUE_44_HPP
#define EXAMPLE_ISSUE_44_HPP

namespace example {

/** Issue 44

    @li All of the constructors should be
    grouped together on the overload
    resolution page.

    @li The class page should show three
    unique briefs for the constructor.
*/
class issue_44
{
    int i_;

public:
    /** Default constructor.

        Description.
    */
    issue_44();

    /** Constructor.

        Description.
    */
    /** @{ */
    issue_44(char);
    issue_44(short, short);
    /** @} */

    /** Constructor.

        Description.
    */
    /** @{ */
    issue_44(char, int);
    issue_44(long, long);
    /** @} */

    /** Void constructor

        Description.
    */
    issue_44(void*);
};

} // example

#endif
