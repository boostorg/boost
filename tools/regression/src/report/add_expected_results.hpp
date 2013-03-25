// add_expected_results.hpp
//
// Copyright (c) 2010 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanyiong file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef ADD_EXPECTED_RESULTS_HPP_INCLUDED
#define ADD_EXPECTED_RESULTS_HPP_INCLUDED

#include "xml.hpp"

namespace boost {
namespace regression {

void add_expected_results(
    test_structure_t::run_t& tests,
    const failures_markup_t& failures_markup,
    const expected_results_t& expected_results);

}
}

#endif
