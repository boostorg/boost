// Copyright MetaCommunications, Inc. 2003-2007.
// Copyright Steven Watanabe 2010
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "add_expected_results.hpp"
#include "common.hpp"
#include "xml.hpp"
#include <string>
#include <boost/foreach.hpp>
#include <boost/functional/hash.hpp>

using namespace boost::regression;

bool target_result(const test_structure_t::test_log_t& test_log, const std::string& name) {
    boost::unordered_map<std::string, test_structure_t::target_t>::const_iterator pos = test_log.targets.find(name);
    if(pos != test_log.targets.end()) {
        return pos->second.result;
    } else {
        return false;
    }
}

bool is_test_log_complete(const test_structure_t::test_log_t& test_log) {
    // FIXME: The original XSL function is buggy and
    // Boost.Build relies on its behavior
    return true;
    if(test_log.test_type == "compile" || test_log.test_type == "compile_fail" ||
        !target_result(test_log, "compile")) {
        return test_log.targets.count("compile") == 1 &&
               test_log.targets.count("link") == 0 &&
               test_log.targets.count("run") == 0;
    } else if(test_log.test_type == "link" || test_log.test_type == "link_fail" ||
        test_log.test_type == "" || test_log.test_type == "lib" ||
        !target_result(test_log, "link")) {
        return test_log.targets.count("compile") == 1 &&
               test_log.targets.count("link") == 1 &&
               test_log.targets.count("run") == 0;
    } else if(test_log.test_type == "run" || test_log.test_type == "run_fail" ||
        test_log.test_type == "run_pyd" || test_log.test_type == "run_mpi") {
        return test_log.targets.count("compile") == 1 &&
               test_log.targets.count("link") == 1 &&
               test_log.targets.count("run") == 1;
    } else {
        throw std::runtime_error("Unknown test type " + test_log.test_type);
    }
}

std::string get_toolset_name(const std::string& toolset, const expected_results_t& expected_results) {
    expected_results_t::toolset_aliases_t::const_iterator pos = expected_results.toolset_aliases.find(toolset);
    if(pos != expected_results.toolset_aliases.end()) {
        return pos->second;
    } else {
        return toolset;
    }
}

void add_note(test_structure_t::test_log_t& test_log, const std::string& text, const std::string& class_name = "auto-note") {
    test_log.notes.push_back("<span class=\"" + class_name + "\">" + text + "</span>");
}

void process_test_log(test_structure_t::test_log_t& test_log,
                      const failures_markup_t& failures_markup,
                      const expected_results_t& expected_results) {

    bool is_complete = is_test_log_complete(test_log);

    bool has_failures = false;
    typedef boost::unordered_map<std::string, test_structure_t::target_t>::const_reference target_ref;
    BOOST_FOREACH(target_ref target, test_log.targets) {
        if(!target.second.result) {
            has_failures = true;
            break;
        }
    }

    bool actual_result = !(has_failures || !is_complete);

    std::string toolset_name = get_toolset_name(test_log.toolset, expected_results);

    const bool* expected_results_test_case = 0;
    {
        test_case_t test_id;
        test_id.library = test_log.library;
        test_id.test_name = test_log.test_name;
        test_id.toolset_name = toolset_name;
        expected_results_t::tests_t::const_iterator pos = expected_results.tests.find(test_id);
        if(pos != expected_results.tests.end()) {
            expected_results_test_case = &pos->second;
        }
    }

    std::string category = "0";
    node_ptr test_failures_markup = 0;
    {
        boost::unordered_map<std::string, node_ptr>::const_iterator pos = failures_markup.libraries.find(test_log.library);
        if(pos != failures_markup.libraries.end()) {
            node_ptr library_markup = pos->second;
            FOR_EACH_ELEMENT(elem, library_markup) {
                if(check_name(elem, "test")) {
                    std::string test_name;
                    if(lookup_attr(elem, "name", test_name) && re_match(test_name, test_log.test_name)) {
                        lookup_attr(elem, "category", category);
                        FOR_EACH_ELEMENT(mark_failure, elem) {
                            FOR_EACH_ELEMENT(toolset, mark_failure) {
                                std::string toolset_name;
                                if(lookup_attr(toolset, "name", toolset_name) && re_match(toolset_name, test_log.toolset)) {
                                    test_failures_markup = mark_failure;
                                    goto found_explicit_failure_markup;
                                }
                            }
                        }
                    }
                } else if(check_name(elem, "mark-expected-failures")) {
                    bool has_test = false;
                    bool has_toolset = false;
                    FOR_EACH_ELEMENT(subelem, elem) {
                        std::string name;
                        bool has_name = lookup_attr(subelem, "name", name);
                        if(has_name && check_name(subelem, "test") && re_match(name, test_log.test_name)) {
                            has_test = true;
                        } else if(has_name && check_name(subelem, "toolset") && re_match(name, test_log.toolset)) {
                            has_toolset = true;
                        }
                        if(has_toolset && has_test) {
                            test_failures_markup = elem;
                            goto found_explicit_failure_markup;
                        }
                    }
                }
            }
        }
        found_explicit_failure_markup:;
    }
    
    bool is_new = (expected_results_test_case == 0);
    bool has_explicit_markup = (test_failures_markup != 0);
    
    bool expected_result = !(has_explicit_markup || (expected_results_test_case && !*expected_results_test_case));

    bool status = (expected_result == actual_result);
    
    bool unexpected_success = (expected_result == false && actual_result == true);
    std::string expected_reason;
    lookup_attr(test_failures_markup, "reason", expected_reason);

    if(unexpected_success && has_explicit_markup) {
        add_note(test_log,
                 "This test case was explicitly marked up in \n"
                 "<a href=\"http://svn.boost.org/svn/boost/{$source}/status/explicit-failures-markup.xml\">\n"
                 "    status/explicit-failures-markup.xml</a> file in the Boost SVN as \"expected to fail\",\n"
                 "but is passing. Please consult the notes/output below for more details.\n");
    }
    if(has_explicit_markup && lookup_element(test_failures_markup, "note") == 0) {
        if(unexpected_success) {
            add_note(test_log,
                     "No explanation was provided for this markup. Please contact the library \n"
                     "author(s)/maintainer(s) for more details.\n");
        } else {
            add_note(test_log,
                     "This failure was explicitly marked as expected in \n"
                     "<a href=\"http://svn.boost.org/svn/boost/{$source}/status/explicit-failures-markup.xml\">\n"
                     "status/explicit-failures-markup.xml</a> file in the Boost SVN. \n"
                     "Please contact the library author(s)/maintainer(s) for the explanation of this markup.\n");
        }
    }
    if(node_ptr elem = lookup_element(test_failures_markup, "note")) {
        test_log.notes.push_back(elem);
    }

    if(expected_results_test_case && !*expected_results_test_case) {
        if(unexpected_success) {
            add_note(test_log,
                     "This test case used to fail in the reference (\"last-known-good\") release.\n");
        } else {
            add_note(test_log,
                     "This failure was present in the reference (\"last-known-good\") release.\n");
        }
    }
    
    if(!is_complete && !has_failures) {
        add_note(test_log,
            "<b>[Reporting Tools Internal Error]</b> This test case's XML is missing one or more log entries\n"
            "of the regression run's steps associated with the test case's type (\"" + test_log.test_type + "\").\n"
            "Please <a href=\"mailto:mailto:boost-testing@lists.boost.org\">contact reporting tools \n"
            "maintainers</a> about this problem.\n", "internal-error-note");
    }

    test_log.result = actual_result;
    test_log.expected_result = expected_result;
    test_log.expected_reason = expected_reason;
    test_log.status = status;
    test_log.is_new = is_new;
    test_log.category = category;
}

void boost::regression::add_expected_results(
    test_structure_t::run_t& tests,
    const failures_markup_t& failures_markup,
    const expected_results_t& expected_results)
{
    BOOST_FOREACH(test_structure_t::toolset_group_t::reference toolset, tests.toolsets) {
        BOOST_FOREACH(test_structure_t::toolset_t::reference library, toolset.second) {
            BOOST_FOREACH(test_structure_t::library_t::reference test_case, library.second) {
                BOOST_FOREACH(test_structure_t::test_case_t::reference test_log, test_case.second) {
                    process_test_log(test_log, failures_markup, expected_results);
                }
            }
        }
    }
}
