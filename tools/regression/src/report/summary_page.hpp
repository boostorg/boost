// result_page.cpp
//
// Copyright Steven Watanabe 2013
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_REGRESSION_SUMMARY_PAGE_HPP
#define BOOST_REGRESSION_SUMMARY_PAGE_HPP

#include <boost/filesystem/path.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <string>
#include <vector>
#include "xml.hpp"

namespace boost {
namespace regression {

void summary_page(const std::string& mode,
                  const std::string& source,
                  const boost::posix_time::ptime& run_date,
                  const std::vector<std::string>& warnings,
                  const test_structure_t& tests,
                  const failures_markup_t & explicit_markup,
                  bool release);

}
}

#endif
