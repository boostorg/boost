// xml.cpp
//
// Copyright (c) 2010 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanyiong file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "xml.hpp"
#include "common.hpp"

#include <boost/date_time/posix_time/time_parsers.hpp>
#include <boost/functional/hash.hpp>
#include <fstream>
#include <boost/format.hpp>
#include <boost/foreach.hpp>

using namespace boost::regression;

std::size_t boost::regression::hash_value(const test_case_t& test_case) {
    std::size_t result = 0;
    boost::hash_combine(result, test_case.test_name);
    boost::hash_combine(result, test_case.library);
    boost::hash_combine(result, test_case.toolset_name);
    return result;
}

bool boost::regression::operator==(const test_case_t& lhs, const test_case_t& rhs) {
    return lhs.test_name == rhs.test_name &&
           lhs.library == rhs.library &&
           lhs.toolset_name == rhs.toolset_name;
}

boost::regression::node_ptr boost::regression::lookup_element(node_ptr element, const std::string& name);

boost::regression::attr_ptr boost::regression::lookup_attr(node_ptr element, const std::string& name) {
    if(element == 0) return 0;
    return element->first_attribute(name.data(), name.size());
}

bool boost::regression::lookup_attr(node_ptr element, const std::string& name, std::string& result) {
    if(element == 0) return false;
    if(attr_ptr attr = lookup_attr(element, name)) {
        result = std::string(attr->value(), attr->value_size());
        return true;
    } else {
        return false;
    }
}

void require_attr(node_ptr element, const std::string& name, std::string& result) {
    if(!lookup_attr(element, name, result)) {
        throw xml_error("Missing attribute " + name + " in element " + std::string(element->name(), element->name_size()));
    }
}

bool boost::regression::check_attr(node_ptr element, const std::string& name, const std::string& expected) {
    if(attr_ptr attr = lookup_attr(element, name)) {
        return std::string(attr->value(), attr->value_size()) == expected;
    } else {
        return false;
    }
}

bool boost::regression::check_name(node_ptr element, const std::string& name) {
    return std::string(element->name(), element->name_size()) == name;
}

bool boost::regression::check_attr(node_ptr element,
                const std::string& element1,
                const std::string& attr,
                const std::string& expected) {
    if(element == 0) return false;
    else if(element1 == "*") {
        FOR_EACH_ELEMENT(nested, element) {
            if(check_attr(nested, attr, expected)) {
                return true;
            }
        }
        return false;
    } else {
        return check_attr(lookup_element(element, element1), attr, expected);
    }
}

boost::regression::node_ptr boost::regression::lookup_element(node_ptr element, const std::string& name) {
    if(element == 0) {
        return 0;
    } else {
        return element->first_node(name.data(), name.size());
    }
}

int boost::regression::count_element(node_ptr element, const std::string& name) {
    int result = 0;
    element = element->first_node(name.data(), name.size());
    while(element != 0) {
        ++result;
        element = element->next_sibling(name.data(), name.size());
    }
    return result;
}

std::string boost::regression::value_of(node_ptr element) {
    if(element && element->value() != 0) {
        return std::string(element->value(), element->value_size());
    } else {
        return std::string();
    }
}

void boost::regression::load_failures_markup(node_ptr root, failures_markup_t& failures_markup) {
    if(check_name(root, "library")) {
        std::string library;
        lookup_attr(root, "name", library);
        failures_markup.libraries.insert(std::make_pair(library, root));
    } else if(check_name(root, "mark-toolset")) {
        if(check_attr(root, "status", "required")) {
            std::string name;
            if(lookup_attr(root, "name", name)) {
                failures_markup.required_toolsets.insert(name);
            }
        }
    } else if(check_name(root, "note")) {
        std::string refid;
        if(lookup_attr(root, "id", refid)) {
            failures_markup.notes.insert(std::make_pair(refid, root));
        }
    } else {
        FOR_EACH_ELEMENT(elem, root) {
            load_failures_markup(elem, failures_markup);
        }
    }
}

namespace {

void load_test_log(node_ptr root, test_structure_t::test_log_t& test_log) {
    lookup_attr(root, "library", test_log.library);
    lookup_attr(root, "test-program", test_log.test_program);
    test_log.show_run_output = check_attr(root, "show-run-output", "true");
    lookup_attr(root, "toolset", test_log.toolset);
    lookup_attr(root, "test-type", test_log.test_type);
    lookup_attr(root, "test-name", test_log.test_name);
    lookup_attr(root, "target-directory", test_log.target_directory);
    // these are set by add_expected_results
    test_log.result = false; // check_attr(root, "result", "success");
    test_log.expected_result = false; // check_attr(root, "expected-result", "success");
    // lookup_attr(root, "expected-reason", test_log.expected_reason);
    test_log.status = check_attr(root, "status", "expected");
    test_log.is_new = check_attr(root, "is-new", "yes");
    lookup_attr(root, "category", test_log.category);

    // process compile/run/etc.
    FOR_EACH_ELEMENT(elem, root) {
        std::string name(elem->name(), elem->name_size());
        if(name != "") {
            test_structure_t::target_t& target = test_log.targets[name];
            target.type = name;
            lookup_attr(elem, "timestamp", target.timestamp);
            target.result = !check_attr(elem, "result", "fail");
            target.contents = elem;
        }
    }
}

void collect_toolsets(node_ptr root, test_structure_t::toolset_group_t& out, test_structure_t::toolset_group_t& non_test_case_targets) {
    if(check_name(root, "test-log")) {
        std::string toolset;
        if(lookup_attr(root, "toolset", toolset)) {
            std::string library, test_name;
            lookup_attr(root, "library", library);
            lookup_attr(root, "test-name", test_name);

            test_structure_t::test_log_t log;
            load_test_log(root, log);
            if(is_test_log_a_test_case(log))
                out[toolset][library][test_name].push_back(log);
            else
                non_test_case_targets[toolset][library][test_name].push_back(log);
        }
    } else {
        FOR_EACH_ELEMENT(elem, root) {
            collect_toolsets(elem, out, non_test_case_targets);
        }
    }
}

// FIXME: Make sure that Boost.DateTime handles parsing errors correctly
boost::posix_time::ptime parse_time(std::string arg) {
    // fix up some formatting problems
    if(!arg.empty() && arg[arg.size() - 1] == 'Z') arg.resize(arg.size() - 1);
    std::replace(arg.begin(), arg.end(), 'T', ' ');
    return boost::posix_time::time_from_string(arg);
}

void validate_run(const test_structure_t::run_t& run) {
    if(run.run_type != "incremental" && run.run_type != "full") {
        BOOST_THROW_EXCEPTION(xml_error("Expected run-type to be \"incremental\" or \"full\""));
    }
    BOOST_FOREACH(char ch, run.revision) {
        if(!('0' <= ch && ch <= '9')) {
            BOOST_THROW_EXCEPTION(xml_error("Expected revision to be a numeric constant"));
        }
    }
}

}

void boost::regression::load_test_structure(node_ptr root, test_structure_t& structure, std::vector<test_structure_t::run_t*>& runs) {
    if(check_name(root, "test-run")) {
        test_structure_t::run_t run;
        std::string timestamp;
        require_attr(root, "runner", run.runner);
        require_attr(root, "platform", run.platform);
        require_attr(root, "run-type", run.run_type);
        require_attr(root, "source", run.source);
        require_attr(root, "revision", run.revision);
        require_attr(root, "timestamp", timestamp);
        // "2010-05-11T18:29:17Z"
        run.timestamp = parse_time(timestamp);
        run.comment = value_of(lookup_element(root, "comment"));
        validate_run(run);
        collect_toolsets(root, run.toolsets, run.non_test_case_targets);
        structure.platforms[run.platform].push_back(run);
        runs.push_back(&structure.platforms[run.platform].back());
    } else {
        FOR_EACH_ELEMENT(elem, root) {
            load_test_structure(elem, structure, runs);
        }
    }
}

namespace {

struct escaped {
    const char* input;
    std::size_t size;
    bool trim;
};

// okay
void write_characters(html_writer& document, const char* input, std::size_t size) {
    for(std::size_t i = 0; i < size; ++i) {
        if(input[i] == '<') {
            document << "&lt;";
        } else if(input[i] == '>') {
            document << "&gt;";
        } else if(input[i] == '&') {
            document << "&amp;";
        } else {
            document << input[i];
        }
    }
}

// FIXME: do not break in the middle of a code point
html_writer& operator<<(html_writer& document, const escaped& text) {
    std::size_t max_size = 1 << 16;
    if(text.trim && (text.size > max_size)) {
        write_characters(document, text.input, max_size);
        document << str(boost::format("...\n\n[The content has been trimmed by the report system because it exceeds %d bytes]") % max_size);
    } else {
        write_characters(document, text.input, text.size);
    }
    return document;
}

escaped make_escaped(const char* input, std::size_t size, bool trim) {
    escaped result = { input, size, trim };
    return result;
}

std::string escape_characters(const char* input, std::size_t size) {
    std::string result;
    for(std::size_t i = 0; i < size; ++i) {
        if(input[i] == '<') {
            result += "&lt;";
        } else if(input[i] == '>') {
            result += "&gt;";
        } else if(input[i] == '&') {
            result += "&amp;";
        } else if(input[i] == '\'') {
            result += "&apos;";
        } else if(input[i] == '"') {
            result += "&quot;";
        } else {
            result += input[i];
        }
    }
    return result;
}
}

std::string boost::regression::escape_xml(const std::string& s) {
    return escape_characters(s.data(), s.size());
}

void boost::regression::write_to_stream(html_writer& os, node_ptr node, bool trim) {
    using namespace boost::property_tree::detail::rapidxml;
    switch(node->type()) {
        case node_document:
            FOR_EACH_ELEMENT(elem, node) {
                write_to_stream(os, elem);
            }
            break;
        case node_element:
            os << '<' << escape_characters(node->name(), node->name_size());
            for(attr_ptr attr = node->first_attribute(); attr != 0; attr = attr->next_attribute()) {
                os << ' ' << std::string(attr->name(), attr->name_size()) << '=' << '"' << escape_characters(attr->value(), attr->value_size()) << '"';
            }
            os << '>';
            FOR_EACH_ELEMENT(elem, node) {
                write_to_stream(os, elem);
            }
            os << '<' << '/' << escape_characters(node->name(), node->name_size()) << '>';
            break;
        case node_data:
            os << make_escaped(node->value(), node->value_size(), trim);
            break;
        default:
            throw xml_error("Don't know how to handle element type");
    }
}

void boost::regression::write_contents(html_writer& os, node_ptr node, bool trim) {
    FOR_EACH_ELEMENT(elem, node) {
        write_to_stream(os, elem, trim);
    }
}

namespace {
struct node_storage : document_type {
    std::vector<char> storage;
};
}

boost::shared_ptr<document_type> boost::regression::read_xml_file(const char* filename) {
    std::ifstream input(filename);
    if(!input) {
        throw(std::ios_base::failure(std::string("Could not open file: ") + filename));
    }
    boost::shared_ptr<node_storage> result(new node_storage());
    std::streambuf* buf = input.rdbuf();
    std::streambuf::int_type ch;
    while((ch = buf->sbumpc()) != std::char_traits<char>::eof()) {
        result->storage.push_back(ch);
    }
    result->storage.push_back('\0');
    result->parse<boost::property_tree::detail::rapidxml::parse_default>(&result->storage[0]);
    return result;
}

namespace {

void load_expected_results(node_ptr root, test_case_t id, expected_results_t& expected_results) {
    if(check_name(root, "test-result")) {
        lookup_attr(root, "test-name", id.test_name);
        bool result = !check_attr(root, "result", "fail");
        expected_results.tests.insert(std::make_pair(id, result));
    } else {
        if(check_name(root, "toolset")) {
            std::string name;
            lookup_attr(root, "name", name);
            id.toolset_name = name;
            FOR_EACH_ELEMENT(elem, root) {
                if(check_name(elem, "toolset-alias")) {
                    std::string alias_name;
                    if(lookup_attr(elem, "name", alias_name)) {
                        expected_results.toolset_aliases.insert(std::make_pair(alias_name, name));
                    }
                }
            }
        } else if(check_name(root, "library")) {
            lookup_attr(root, "name", id.library);
        }
        FOR_EACH_ELEMENT(elem, root) {
            load_expected_results(elem, id, expected_results);
        }
    }
}

}

void boost::regression::load_expected_results(node_ptr root, expected_results_t& expected_results) {
    test_case_t id;
    ::load_expected_results(root, id, expected_results);
}
