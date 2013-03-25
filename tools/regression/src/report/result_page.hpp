// result_page.cpp
//
// Copyright MetaCommunications, Inc. 2003-2007.
// Copyright Steven Watanabe 2010-2011
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/filesystem/path.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <string>
#include <vector>
#include "xml.hpp"

namespace boost {
namespace regression {

void result_page(const test_structure_t& tests,
                 const failures_markup_t& explicit_markup,
                 bool release,
                 const std::string& source,
                 const boost::posix_time::ptime& run_date,
                 const std::vector<std::string>& warnings,
                 const std::string& mode,
                 const boost::filesystem::path& comment_file);

}
}
