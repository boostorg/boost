//
// Copyright (c) 2021 Evan Lenz (evan@lenzconsulting.com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef EXAMPLE_ISSUE_101_HPP
#define EXAMPLE_ISSUE_101_HPP

namespace example {

/** Issue 101

    `typename` parameters should be handled (without generating an error).
*/
template<typename charT, typename traits>
class issue_101;

} // example

#endif
