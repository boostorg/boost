//
// Copyright (c) 2021 Evan Lenz (evan@lenzconsulting.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXAMPLE_ISSUE_63_HPP
#define EXAMPLE_ISSUE_63_HPP

namespace example {

/** Issue 63

    @li Markup (@ commands) in tables should be rendered
*/
enum class issue_63
{
    /**
     * foo brief

       foo description

       @see issue_55
    */
    foo,

    /**
     * bar brief

       bar description

       @li First bullet referencing @ref issue_55
       @li Second bullet
    */
    bar
};

} // example

#endif
