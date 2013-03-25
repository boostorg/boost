// common.hpp
//
// Copyright (c) 2010 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanyiong file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef COMMON_HPP_INCLUDED
#define COMMON_HPP_INCLUDED

#include <vector>
#include <string>
#include <set>
#include <boost/filesystem/path.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include "xml.hpp"

namespace boost {
namespace regression {

class html_writer;
typedef std::vector<const test_structure_t::test_log_t*> test_log_group_t;

bool is_library_beta(const failures_markup_t& explicit_markup, const std::string& library);
bool is_test_log_a_test_case(const test_structure_t::test_log_t& test_log);

bool is_unusable(const failures_markup_t& markup, const std::string& library, const std::string& toolset);

void get_unusable(const failures_markup_t& markup,
                  const std::string& library,
                  const test_structure_t& test_structure,
                  boost::unordered_map<std::string, std::size_t>& out,
                  std::vector<node_ptr>& notes);

bool re_match(const std::string& pattern, const std::string& text);

int timestamp_difference(const boost::posix_time::ptime& x, const boost::posix_time::ptime& y);
std::string format_timestamp(const boost::posix_time::ptime& timestamp);

std::string encode_path(const std::string& path);
std::string escape_uri(const std::string& path); // escapes a URI path (leaves '/' alone)
std::string escape_literal_uri(const std::string& path); // escapes all special characters in a URI
std::string output_file_path(const std::string& path);
std::string log_file_path(
    const failures_markup_t& explicit_markup,
    const test_structure_t::test_log_t& test_log,
    const std::string& runner,
    const std::string& release_postfix = "");

bool show_library(const failures_markup_t& explicit_markup, const std::string& library, bool release);
bool show_output(const failures_markup_t& markup, const test_structure_t::test_log_t& test_log);
bool show_toolset(const failures_markup_t& explicit_markup, const std::string& toolset, bool release);

void insert_report_header(html_writer& document,
                          const boost::posix_time::ptime& run_date,
                          const std::vector<std::string>& warnings,
                          const std::string& purpose = "");

void insert_view_link(html_writer& document,
                      const std::string& page,
                      const std::string& class_,
                      bool release);

void insert_page_links(html_writer& document,
                       const std::string& page,
                       bool release,
                       const std::string& mode);

void insert_runners_rows(html_writer& document,
                         const std::string& mode,
                         const std::string& top_or_bottom,
                         const test_structure_t& test_structure,
                         const boost::posix_time::ptime& run_date);

void insert_toolsets_row(html_writer& document,
                         const test_structure_t& test_structure,
                         const failures_markup_t& explicit_markup,
                         const std::string& mode,
                         const boost::posix_time::ptime& run_date,
                         const std::string& library = std::string(),
                         const boost::unordered_map<std::string, std::size_t>& library_marks = boost::unordered_map<std::string, std::size_t>());

void show_note(
    html_writer& document,
    const test_structure_t::note_t& note,
    const std::string& references,
    const failures_markup_t& explicit_markup);
void show_notes(html_writer& document, const std::vector<test_structure_t::note_t>& notes, const failures_markup_t& explicit_markup);

std::string result_cell_class(const failures_markup_t& explicit_markup,
                              const std::string& library,
                              const std::string& toolset,
                              const test_log_group_t& test_logs);

std::string result_cell_class(const failures_markup_t& explicit_markup,
                              const std::string& library,
                              const std::string& toolset,
                              const test_structure_t::library_t& test_logs);

std::string alternate_mode(const std::string& mode);
std::string release_postfix(bool is_release);

void get_libraries(const test_structure_t& test_structure, std::set<std::string>& out);

}
}

#endif
