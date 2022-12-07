//
// Copyright (c) 2020 Krystian Stasiowski (sdkrystian@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXAMPLE_ISSUE_34_HPP
#define EXAMPLE_ISSUE_34_HPP

namespace example {

/** Issue 34
*/
class issue_34
{
    /// Should not be displayed
    struct P
    {
    };

public:
    /// Base of Q
    struct R
    {
    };

    /** Q should be derived only from R
    */
    struct Q : private P, R
    {
    };
};

} // example

#endif
