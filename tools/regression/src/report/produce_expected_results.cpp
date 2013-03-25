// Copyright MetaCommunications, Inc. 2003-2005.
// Copyright Steven Watanabe 2013
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "produce_expected_results.hpp"
#include "html_writer.hpp"
#include <iostream>

void boost::regression::produce_expected_results(const test_structure_t& tests) {
    std::cout << "Warning: expected results not implemented" << std::endl;
    return;

    html_writer document("expected_results.xml");
    document << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    document << "<root>\n";
    document << "  <expected-failures>\n";
#if 0
    foreach test-log
    <xsl:if test="meta:is_test_log_a_test_case(.)">
        <test-result library="{@library}" test-name="{@test-name}" toolset="{@toolset}" result="{@result}" />
    </xsl:if>
#endif
    document << "  </expected-failures>\n";
    document << "</root>\n";
}
