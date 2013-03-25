// issues_page.hpp
//
// Copyright (c) 2010
// Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "xml.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

#include <string>
#include <vector>

namespace boost {
namespace regression {

void issues_list(const std::string& output_dir,
                 const test_structure_t& tests,
                 const failures_markup_t& explicit_markup,
                 bool release,
                 const std::string& source,
                 const boost::posix_time::ptime& run_date,
                 const std::vector<std::string>& warnings,
                 const std::string& purpose);

}
}