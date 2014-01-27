//  tools/regression/src/detail/common.hpp  --------------------------------------------//

//  Copyright Beman Dawes 2002.

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_REGRESSION_TOOLS_COMMON_HPP
#define BOOST_REGRESSION_TOOLS_COMMON_HPP

#include <boost/filesystem/operations.hpp>
#include <boost/assert.hpp>
#include <string>

namespace boost
{
namespace regression_tools
{

  //  boost_root_path  -----------------------------------------------------------------//

  inline boost::filesystem::path boost_root_path(const boost::filesystem::path& start =
    boost::filesystem::initial_path())
  // Requires: start is absolute
  {
    BOOST_ASSERT(start.is_absolute());
    for(boost::filesystem::path root = start;
      !root.empty();
      root = root.parent_path())
    {
      if ( boost::filesystem::exists(root / "libs"))
        return root;
    }
    return boost::filesystem::path();
  }

}  // namespace regression_tools
}  // namespace boost

#endif  // include guard
