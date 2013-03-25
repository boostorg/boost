// links_page.hpp
//
// Copyright (c) 2010 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanyiong file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef LINKS_PAGE_HPP_INCLUDED
#define LINKS_PAGE_HPP_INCLUDED

#include "xml.hpp"
#include <boost/filesystem/path.hpp>

namespace boost {
namespace regression {

void links_page(
    const boost::posix_time::ptime& run_date,
    const failures_markup_t& explicit_markup,
    const test_structure_t::run_t& test_run);

}
}

#endif
