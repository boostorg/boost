// xml.hpp
//
// Copyright (c) 2010 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanyiong file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef XML_HPP_INCLUDED
#define XML_HPP_INCLUDED

#include <string>
#include <vector>
#include <iosfwd>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/property_tree/detail/rapidxml.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/variant.hpp>
#include <boost/shared_ptr.hpp>

#include "html_writer.hpp"

namespace boost {
namespace regression {

class xml_error : public std::exception {
public:
    explicit xml_error(const std::string& m) : message(m) {}
    virtual ~xml_error() throw() {}
    virtual const char * what() const throw() { return message.c_str(); }
private:
    std::string message;
};

typedef boost::property_tree::detail::rapidxml::xml_node<> node_type;
typedef boost::property_tree::detail::rapidxml::xml_attribute<> attr_type;
typedef boost::property_tree::detail::rapidxml::xml_document<> document_type;
typedef node_type* node_ptr;
typedef attr_type* attr_ptr;
typedef document_type* document_ptr;

struct test_case_t {
    std::string toolset_name;
    std::string library;
    std::string test_name;
};

std::size_t hash_value(const test_case_t& test_case);
bool operator==(const test_case_t& lhs, const test_case_t& rhs);

struct expected_results_t {
    typedef boost::unordered_map<test_case_t, bool> tests_t;
    typedef boost::unordered_map<std::string, std::string> toolset_aliases_t;
    tests_t tests;
    toolset_aliases_t toolset_aliases;
};
void load_expected_results(node_ptr root, expected_results_t& expected_results);

struct test_structure_t {
    struct target_t {
        std::string type;
        std::string timestamp;
        bool result;
        node_ptr contents;
    };
    typedef boost::variant<std::string, node_ptr> note_t;
    struct test_log_t {
        std::string library;
        std::string test_program;
        bool show_run_output;
        std::string toolset;
        std::string test_type;
        std::string test_name;
        std::string target_directory;
        bool result;
        bool expected_result;
        std::string expected_reason;
        bool status;
        bool is_new;
        std::string category;
        boost::unordered_map<std::string, target_t> targets;
        std::vector<note_t> notes;
    };
    typedef std::vector<test_log_t> test_case_t;
    typedef boost::unordered_map<std::string, test_case_t> library_t;
    typedef boost::unordered_map<std::string, library_t> toolset_t;
    typedef std::map<std::string, toolset_t> toolset_group_t;
    struct run_t {
        std::string runner;
        std::string platform;
        std::string run_type;
        std::string source;
        std::string revision;
        std::string comment;
        boost::posix_time::ptime timestamp;
        toolset_group_t toolsets;
        toolset_group_t non_test_case_targets;
    };
    typedef std::vector<run_t> platform_t;
    typedef std::map<std::string, platform_t> platform_group_t;
    platform_group_t platforms;
};
void load_test_structure(node_ptr root, test_structure_t& structure, std::vector<test_structure_t::run_t*>& runs);

struct failures_markup_t {
    boost::unordered_map<std::string, node_ptr> libraries;
    boost::unordered_set<std::string> required_toolsets;
    boost::unordered_map<std::string, node_ptr> notes;
};
void load_failures_markup(node_ptr root, failures_markup_t& failures_markup);

#define FOR_EACH_ELEMENT(name, node)\
    for(::boost::regression::node_ptr name = (node)->first_node(); name != 0; name = name->next_sibling())

attr_ptr lookup_attr(node_ptr element, const std::string& name);
bool lookup_attr(node_ptr element, const std::string& name, std::string& result);
bool check_attr(node_ptr element, const std::string& name, const std::string& expected);
bool check_name(node_ptr element, const std::string& name);
bool check_attr(node_ptr element, const std::string& element1, const std::string& attr, const std::string& expected);
node_ptr lookup_element(node_ptr element, const std::string& name);
int count_element(node_ptr element, const std::string& name);
std::string value_of(node_ptr element);

std::string escape_xml(const std::string& s);
void write_to_stream(html_writer& os, node_ptr node, bool trim=false);
void write_contents(html_writer& document, node_ptr node, bool trim=false);
boost::shared_ptr<document_type> read_xml_file(const char* filename);

}
}

#endif
