//
// Copyright (c) 2020 Vinnie Falco (vinnie dot falco at gmail dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXAMPLE_ISSUE_47_HPP
#define EXAMPLE_ISSUE_47_HPP

namespace example {

/** Issue 47

    The function `max_size` should be
    listed as a static member.
*/
class issue_47
{
    int i_;

public:
    /** Member
    */
    void f();

    /** Static Member
    */
    static int max_size() noexcept;
};

} // example

#endif
