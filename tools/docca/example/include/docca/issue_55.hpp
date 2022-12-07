//
// Copyright (c) 2020 Krystian Stasiowski (sdkrystian@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXAMPLE_ISSUE_55_HPP
#define EXAMPLE_ISSUE_55_HPP

namespace example {

/** Issue 55

    Clicking @ref f should lead to the
    overload resolution landing page.
*/
struct issue_55
{
    void f(int);
    void f(double);
};

} // example

#endif
