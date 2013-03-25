// Copyright MetaCommunications, Inc. 2003-2005.
// Copyright Steven Watanabe 2010
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "common.hpp"
#include "xml.hpp"
#include "html.hpp"

#include <boost/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <algorithm>
#include <set>

using namespace boost::regression;

std::string boost::regression::alternate_mode(const std::string& mode) {
    if(mode == "user") {
        return "developer";
    } else {
        return "user";
    }
}

std::string boost::regression::release_postfix(bool is_release) {
    if(is_release) {
        return "_release";
    } else {
        return "";
    }
}

// safe
void boost::regression::get_libraries(const test_structure_t& test_structure, std::set<std::string>& out) {
    typedef boost::unordered_map<std::string, test_structure_t::platform_t>::const_reference outer_elem;
    BOOST_FOREACH(outer_elem platform, test_structure.platforms) {
        BOOST_FOREACH(test_structure_t::platform_t::const_reference run, platform.second) {
            BOOST_FOREACH(test_structure_t::toolset_group_t::const_reference toolset, run.toolsets) {
                BOOST_FOREACH(test_structure_t::toolset_t::const_reference library, toolset.second) {
                    out.insert(library.first);
                }
            }
        }
    }
}

#if 0

    <func:function name="meta:test_case_status">
        <xsl:param name="explicit_markup"/>
        <xsl:param name="test_log"/>

        <xsl:variable name="status">
            <xsl:choose> 
                 <xsl:when test="meta:is_unusable( $explicit_markup, $test_log/@library, $test_log/@toolset )">
                     <xsl:text>unusable</xsl:text>
                 </xsl:when>
                 <xsl:when test="$test_log/@result='fail' and  $test_log/@status='unexpected' and $test_log/@is-new='no'">
                     <xsl:text>fail-unexpected</xsl:text>
                 </xsl:when>
                 <xsl:when test="$test_log/@result='fail' and  $test_log/@status='unexpected' and $test_log/@is-new='yes'">
                     <xsl:text>fail-unexpected-new</xsl:text>
                 </xsl:when>
                 <xsl:when test="$test_log/@result='success' and  $test_log/@status='unexpected'">
                     <xsl:text>success-unexpected</xsl:text>
                 </xsl:when>
                 <xsl:when test="$test_log/@status='expected'">
                     <xsl:text>expected</xsl:text>
                 </xsl:when>
                 <xsl:otherwise>
                     <xsl:text>other</xsl:text>
                 </xsl:otherwise>
             </xsl:choose>
         </xsl:variable>
         <func:result select="$status"/>
     </func:function>

    <func:function name="meta:is_toolset_required">
        <xsl:param name="toolset"/>
        <func:result select="count( $explicit_markup/explicit-failures-markup/mark-toolset[ @name = $toolset and @status='required' ] ) > 0"/>
    </func:function>
    
#endif

bool boost::regression::is_library_beta(const failures_markup_t& explicit_markup, const std::string& library) {
    boost::unordered_map<std::string, node_ptr>::const_iterator pos = explicit_markup.libraries.find(library);
    if(pos != explicit_markup.libraries.end()) {
        return check_attr(pos->second, "status", "beta");
    }
    return false;
}

bool boost::regression::is_test_log_a_test_case(const test_structure_t::test_log_t& test_log) {
    const std::string& type = test_log.test_type;
    return type == "compile" || type == "compile_fail" || type == "link" || type == "link_fail" ||
        type == "run" || type == "run_fail" || type == "run_pyd" || type == "run_mpi";
}

// Does not assume any constraints on contents of the strings
bool boost::regression::is_unusable(const failures_markup_t& markup, const std::string& library, const std::string& toolset) {
    boost::unordered_map<std::string, node_ptr>::const_iterator pos = markup.libraries.find(library);
    if(pos != markup.libraries.end()) {
        FOR_EACH_ELEMENT(mark_unusable, pos->second) {
            if(check_name(mark_unusable, "mark-unusable")) {
                FOR_EACH_ELEMENT(toolset_node, mark_unusable) {
                    std::string name;
                    if(lookup_attr(toolset_node, "name", name) && re_match(name, toolset)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void boost::regression::get_unusable(const failures_markup_t& markup,
                                     const std::string& library,
                                     const test_structure_t& test_structure,
                                     boost::unordered_map<std::string, std::size_t>& out,
                                     std::vector<node_ptr>& notes) {
    boost::unordered_map<std::string, node_ptr>::const_iterator pos = markup.libraries.find(library);
    if(pos != markup.libraries.end()) {
        FOR_EACH_ELEMENT(mark_unusable, pos->second) {
            if(check_name(mark_unusable, "mark-unusable")) {
                node_ptr note = 0;
                std::vector<std::string> toolsets;
                FOR_EACH_ELEMENT(toolset_node, mark_unusable) {
                    std::string name;
                    if(check_name(toolset_node, "toolset") && lookup_attr(toolset_node, "name", name)) {
                        BOOST_FOREACH(test_structure_t::platform_group_t::const_reference platform, test_structure.platforms) {
                            BOOST_FOREACH(test_structure_t::platform_t::const_reference run, platform.second) {
                                BOOST_FOREACH(test_structure_t::toolset_group_t::const_reference toolset, run.toolsets) {
                                    if(re_match(name, toolset.first)) {
                                        toolsets.push_back(toolset.first);
                                    }
                                }
                            }
                        }
                    } else if(check_name(toolset_node, "note")) {
                        note = toolset_node;
                    }
                }
                if(note != 0 && !toolsets.empty()) {
                    notes.push_back(note);
                    BOOST_FOREACH(const std::string& toolset, toolsets) {
                        out[toolset] = notes.size();
                    }
                }
            }
        }
    }
}

// There are no restrictions on the pattern or the
// string.  The only special character in the pattern
// is '*', which matches any number of consecutive characters.
bool boost::regression::re_match(const std::string& pattern, const std::string& text) {
    std::size_t pattern_start = 0;
    std::size_t pattern_end = 0;
    std::size_t text_start = 0;
    // check that the leading portion of the string matches
    std::size_t first = pattern.find('*');
    if(first == std::string::npos) return pattern == text;
    if(pattern.substr(0, first) != text.substr(0, first)) return false;
    text_start = first;
    pattern_start = pattern_end = first + 1;
    for(; pattern_end != pattern.size(); ++pattern_end) {
        // split into blocks at '*'
        if(pattern[pattern_end] == '*') {
            // and search for each block
            std::size_t size = pattern_end - pattern_start;
            std::size_t off = text.find(pattern.data() + pattern_start, text_start, size);
            // if not found, the pattern doesn't match
            if(off == std::string::npos) return false;
            text_start = off + size;
            pattern_start = pattern_end + 1; // skip past the '*'
        }
    }
    // check that the tails of the strings are the same
    std::size_t tail_size = pattern_end - pattern_start;
    if(tail_size == 0) return true; // a trailing star
    std::size_t off = text.find(pattern.data() + pattern_start, text_start, tail_size);
    return off != std::string::npos && (off + tail_size == text.size());
}

// date-time

// The result is clamped to the range [0,30]
int boost::regression::timestamp_difference(const boost::posix_time::ptime& x, const boost::posix_time::ptime& y) {
    boost::posix_time::time_duration diff = y - x;
    int result = diff.hours() / 24;
    if(result < 0) return 0;
    else if(result > 30) return 30;
    else return result;
}

std::string boost::regression::format_timestamp(const boost::posix_time::ptime& timestamp) {
    std::ostringstream stream;
    stream.imbue(std::locale(std::locale::classic(), new boost::posix_time::time_facet("%a, %d %b %Y %H:%M:%S +0000")));
    stream << timestamp;
    return stream.str();
}
    
// path

// FIXME: The result MUST be a valid filesystem path.
std::string boost::regression::encode_path(const std::string& path) {
    std::string result;
    BOOST_FOREACH(char ch, path) {
        if(ch == '.' || ch == '/') {
            ch = '-';
        }
        // FIXME: allow only characters from the following set:
        // "[a-z][A-Z][0-9][-+_. ,()$!~?]...
        result += ch;
    }
    return result;
}

std::string boost::regression::escape_uri(const std::string& path) {
    std::string result;
    BOOST_FOREACH(char ch, path) {
        if (('a' <= ch && ch <= 'z') ||
            ('A' <= ch && ch <= 'Z') ||
            ('0' <= ch && ch <= '9') ||
            ch == '-' || ch == '_' || ch == '~' || ch == '.' ||
            // We're intentionally allowing '/' to go through.
            // to escape it as well, use escape_literal_uri
            ch == '/' ||
            // FIXME: reserved characters
            ch == '+')
            result += ch;
        else {
            unsigned digit = ch;
            ch &= 0xFF;
            const char * xdigits = "0123456789ABCDEF";
            result += '%';
            result += xdigits[digit >> 4];
            result += xdigits[digit & 0xF];
        }
    }
    return result;
}

std::string boost::regression::escape_literal_uri(const std::string& path) {
    std::string result;
    BOOST_FOREACH(char ch, path) {
        // FIXME: Assumes UTF-8
        if (('a' <= ch && ch <= 'z') ||
            ('A' <= ch && ch <= 'Z') ||
            ('0' <= ch && ch <= '9') ||
            ch == '-' || ch == '_' || ch == '~' || ch == '.')
            result += ch;
        else {
            unsigned digit = ch;
            ch &= 0xFF;
            const char * xdigits = "0123456789ABCDEF";
            result += '%';
            result += xdigits[digit >> 4];
            result += xdigits[digit & 0xF];
        }
    }
    return result;
}

// okay
std::string boost::regression::output_file_path(const std::string& path) {
    return("output/" + (encode_path(path) + ".html"));
}

// okay
std::string boost::regression::log_file_path(
    const failures_markup_t& explicit_markup,
    const test_structure_t::test_log_t& test_log,
    const std::string& runner,
    const std::string& release_postfix)
{
    if(show_output(explicit_markup, test_log)) {
        return output_file_path(runner + "-" + test_log.target_directory + release_postfix);
    } else {
        return "";
    }
}

bool boost::regression::show_library(const failures_markup_t& explicit_markup, const std::string& library, bool release) {
    return !release || !is_library_beta(explicit_markup, library);
}

bool boost::regression::show_output(const failures_markup_t& explicit_markup, const test_structure_t::test_log_t& test_log) {
    return ((!test_log.result || test_log.show_run_output) ||
        (test_log.result && !test_log.status))
        && !(is_unusable(explicit_markup, test_log.library, test_log.toolset));
}

bool boost::regression::show_toolset(const failures_markup_t& explicit_markup, const std::string& toolset, bool release) {
    return !release || explicit_markup.required_toolsets.find(toolset) != explicit_markup.required_toolsets.end();
}

// safe: no assumptions, enumerated result
std::string boost::regression::result_cell_class(const failures_markup_t& explicit_markup,
                                                 const std::string& library,
                                                 const std::string& toolset,
                                                 const test_log_group_t& test_logs) {
    if(is_unusable(explicit_markup, library, toolset)) {
        return "unusable";
    }
    if(test_logs.empty()) {
        return "missing";
    }
    BOOST_FOREACH(test_log_group_t::value_type log, test_logs) {
        if(!log->result && log->expected_result && !log->is_new) {
            return "fail-unexpected";
        }
    }
    BOOST_FOREACH(test_log_group_t::value_type log, test_logs) {
        if(!log->result && log->expected_result && log->is_new) {
            return "fail-unexpected-new";
        }
    }
    BOOST_FOREACH(test_log_group_t::value_type log, test_logs) {
        if(!log->result && log->expected_reason != "") {
            return "fail-expected-unreasearched";
        }
    }
    BOOST_FOREACH(test_log_group_t::value_type log, test_logs) {
        if(!log->result) {
            return "fail-expected";
        }
    }
    BOOST_FOREACH(test_log_group_t::value_type log, test_logs) {
        if(log->result && !log->expected_result) {
            return "success-unexpected";
        }
    }
    BOOST_FOREACH(test_log_group_t::value_type log, test_logs) {
        if(log->result && log->expected_result) {
            return "success-expected";
        }
    }
    return "unknown";
}
// safe
std::string boost::regression::result_cell_class(const failures_markup_t& explicit_markup,
                                                 const std::string& library,
                                                 const std::string& toolset,
                                                 const test_structure_t::library_t& test_logs)
{
    test_log_group_t tmp;
    BOOST_FOREACH(test_structure_t::library_t::const_reference test_case, test_logs) {
        BOOST_FOREACH(test_structure_t::test_case_t::const_reference log, test_case.second) {
            tmp.push_back(&log);
        }
    }
    return result_cell_class(explicit_markup, library, toolset, tmp);
}

// requires: purpose must be well-formed html
void boost::regression::insert_report_header(
    html_writer& document,
    const boost::posix_time::ptime& run_date,
    const std::vector<std::string>& warnings,
    const std::string& purpose)
{
    document << "<div class=\"report-info\">\n";
    document << "    <div>\n";
    document << "        <b>Report Time: </b> " << format_timestamp(run_date) << "\n";
    document << "    </div>\n";

    if(!purpose.empty()) {
        document << "    <div>\n";
        document << "        <b>Purpose: </b> " << purpose << "\n";
        document << "    </div>\n";
    }

    BOOST_FOREACH(const std::string& warning, warnings) {
        document << "    <div class=\"report-warning\">\n";
        document << "        <b>Warning: </b>\n";
        document << "        <a href=\"mailto:boost-testing@lists.boost.org?subject=[Report Pages]%20" << escape_literal_uri(warning) << " (" << format_timestamp(run_date) << ")\" class=\"warning-link\">\n";
        document << "            " << escape_xml(warning) << "\n";
        document << "        </a>\n";
        document << "    </div>\n";
    }

    document << "</div>\n";
}

// requires class_ is enumerated
void boost::regression::insert_view_link(html_writer& out, const std::string& page, const std::string& class_, bool release) {
    if(release) {
        out << "<a href=\"" << escape_uri(page) << ".html\" class=\"" << class_ << "\" target=\"_top\">"
               "Full View"
               "</a>\n";
    } else {
        out << "<a href=\"" << escape_uri(page) << "_release.html\" class=\"" << class_ << "\" target=\"_top\">"
               "Release View"
               "</a>";
    }

}

// requires: mode = developer | user (Should be the opposite of the current page)
// requires: page is the base name of the current page.  It should be valid
//           according to encode_path, but should not be URI escaped.
void boost::regression::insert_page_links(html_writer& document,
                                          const std::string& page,
                                          bool release,
                                          const std::string& mode) {
    document << "<div class=\"links\">\n"
             << make_tinyurl
             << "&#160;|&#160;";

    // yes, really.  The class is set to ""
    insert_view_link(document, page, "", release);

    std::string release_postfix(release? "_release" : "");

    document << "&#160;|&#160;"
                "<a href=\"../" << mode << "/" << escape_uri(page) << release_postfix << ".html\" class=\"view-link\" target=\"_top\">"
             << mode << " View"
                "</a>"
                "&#160;|&#160;"
                "<a href=\"" << escape_uri(page) << release_postfix << "_.html#legend\">Legend</a>\n"

                "</div>\n";

}

// requires: mode = summary | details
// requires: top_or_bottom = top | bottom
void boost::regression::insert_runners_rows(html_writer& document,
                                            const std::string& mode,
                                            const std::string& top_or_bottom,
                                            const test_structure_t& test_structure,
                                            const boost::posix_time::ptime& run_date) {
    std::string colspan = (mode == "summary") ? "1" : "2";

    if(top_or_bottom == "top") {
        document << "<tr>\n"
                    "    <td colspan=\"" << colspan << "\">&#160;</td>\n";
        BOOST_FOREACH(test_structure_t::platform_group_t::const_reference platform, test_structure.platforms) {
            std::size_t count = 0;
            BOOST_FOREACH(test_structure_t::platform_t::const_reference run, platform.second) {
                count += run.toolsets.size();
            }
            if(count > 0) {
                document << "    <td colspan=\"" << count << "\" class=\"runner\">\n"
                            "        " << escape_xml(platform.first) << "\n"
                            "    </td>\n";
            }
        }
        document << "    <td colspan=\"" << colspan << "\">&#160;</td>\n"
                    "</tr>\n";
    }

    document << "<tr>\n"
                "    <td colspan=\"" << colspan << "\">&#160;</td>\n";
    BOOST_FOREACH(test_structure_t::platform_group_t::const_reference platform, test_structure.platforms) {
        BOOST_FOREACH(test_structure_t::platform_t::const_reference run, platform.second) {
            if(run.toolsets.size() > 0) {
                document << "    <td colspan=\"" << run.toolsets.size() << "\" class=\"runner\">\n"
                            "        <a href=\"../" << escape_uri(encode_path(run.runner)) << ".html\">\n"
                            "            " << escape_xml(run.runner) << "\n"
                            "        </a>\n"
                            "    </td>\n";
            }
        }
    }
    document << "    <td colspan=\"" << colspan << "\">&#160;</td>\n"
                "</tr>\n";

    document << "<tr>\n"
                "<td colspan=\"" << colspan << "\">&#160;</td>\n";
    BOOST_FOREACH(test_structure_t::platform_group_t::const_reference platform, test_structure.platforms) {
        BOOST_FOREACH(test_structure_t::platform_t::const_reference run, platform.second) {
            if(run.toolsets.size() > 0) {
                document << "    <td colspan=\"" << run.toolsets.size() << "\" class=\"revision\">\n"
                            "         rev " << run.revision << "\n"
                            "    </td>\n";
            }
        }
    }
    document << "    <td colspan=\"" << colspan << "\">&#160;</td>\n"
                "</tr>\n";
    
    document << "<tr>\n"
                "    <td colspan=\"" << colspan << "\">&#160;</td>\n";
    BOOST_FOREACH(test_structure_t::platform_group_t::const_reference platform, test_structure.platforms) {
        BOOST_FOREACH(test_structure_t::platform_t::const_reference run, platform.second) {
            if(run.toolsets.size() > 0) {
                int age = timestamp_difference(run.timestamp, run_date);
                document << "    <td colspan=\"" << run.toolsets.size() << "\" class=\"timestamp\">\n"
                            "        <span class=\"timestamp-" << age << "\">" << format_timestamp(run.timestamp) << "</span>";
                if(run.run_type != "full") {
                    document << "<span class=\"run-type-" << run.run_type << "\">" << run.run_type[0] << "</span>\n";
                }
                document << "    </td>\n";
            }
        }
    }
    document << "    <td colspan=\"" << colspan << "\">&#160;</td>\n"
                "</tr>\n";

    if(top_or_bottom == "bottom") {
        document << "<tr>\n"
                    "    <td colspan=\"" << colspan << "\">&#160;</td>\n";
        BOOST_FOREACH(test_structure_t::platform_group_t::const_reference platform, test_structure.platforms) {
            std::size_t count = 0;
            BOOST_FOREACH(test_structure_t::platform_t::const_reference run, platform.second) {
                count += run.toolsets.size();
            }
            if(count > 0) {
                document << "    <td colspan=\"" << count << "\" class=\"runner\">\n"
                            "        " << escape_xml(platform.first) << "\n"
                            "    </td>\n";
            }
        }
        document << "    <td colspan=\"" << colspan << "\">&#160;</td>\n"
                    "</tr>\n";
    }
}

// requires mode = summary | details
void boost::regression::insert_toolsets_row(html_writer& document,
                                            const test_structure_t& test_structure,
                                            const failures_markup_t& explicit_markup,
                                            const std::string& mode,
                                            const boost::posix_time::ptime& run_date,
                                            const std::string& library,
                                            const boost::unordered_map<std::string, std::size_t>& library_marks) {

    document << "<tr valign=\"middle\">\n";

    std::string colspan = (mode == "summary") ? "1" : "2";
    std::string title = (mode == "summary") ?
        "&#160;library&#160;/&#160;toolset&#160;" :
        "&#160;test&#160;/&#160;toolset&#160;";
              
    document << "    <td class=\"head\" colspan=\"" << colspan << "\" width=\"1%\">" << title << "</td>\n";
    BOOST_FOREACH(const test_structure_t::platform_group_t::const_reference platform, test_structure.platforms) {
        BOOST_FOREACH(const test_structure_t::platform_t::const_reference run, platform.second) {
            BOOST_FOREACH(const test_structure_t::toolset_group_t::const_reference toolset, run.toolsets) {
                std::string name = toolset.first;
                std::string class_ = (explicit_markup.required_toolsets.find(name) != explicit_markup.required_toolsets.end())?
                    "required-toolset-name" :
                    "toolset-name";

                document << "<td class=\"" << class_ << "\">\n";
                int age = timestamp_difference(run.timestamp, run_date);

                document <<    "<span class=\"timestamp-" << age << "\">\n";

                // break toolset names into words
                BOOST_FOREACH(char ch, name) {
                    document << ch;
                    if(ch == '-') {
                        document << ' ';
                    }
                }

                if(mode == "details") {
                    //     <!-- prepare toolset notes -->
                    std::set<std::size_t> toolset_notes;
                    typedef boost::unordered_map<std::string, std::size_t>::const_reference ref_type;
                    BOOST_FOREACH(ref_type toolset_markup, library_marks.equal_range(name)) {
                        toolset_notes.insert(toolset_markup.second);
                    }
                    if(!toolset_notes.empty()) {
                        document << "<span class=\"super\">\n";
                        bool first = true;
                        BOOST_FOREACH(std::size_t note_index, toolset_notes) {
                            if(!first) document << ", "; else first = false;
                            document << "<a href=\"#" << escape_uri(library) << "-note-" << note_index << "\" title=\"Note " << note_index << "\">\n"
                                        "    " << note_index << "\n"
                                        "</a>\n";
                        }
                        document << "</span>\n";
                    }
                }

                document << "</span>\n"
                            "</td>\n";
            }
        }
    }
    document << "<td class=\"head\" width=\"1%\">" << title << "</td>\n"
                "</tr>\n";
}

namespace {

std::string get_note_attr(const test_structure_t::note_t& note, const std::string& name) {
    if(const node_ptr* node = boost::get<node_ptr>(&note)) {
        std::string result;
        lookup_attr(*node, name, result);
        return result;
    } else {
        return std::string();
    }
}

}

// requires: if note is a string, it is well-formed html
void boost::regression::show_note(
    html_writer& document,
    const test_structure_t::note_t& note,
    const std::string& references,
    const failures_markup_t& explicit_markup)
{
    document << "<div class=\"note\">\n";

    std::string author = get_note_attr(note, "author");
    std::string date = get_note_attr(note, "date");

    document << "    <span class=\"note-header\">\n";

    if(author != "" && date != "") {
        document << "        [&#160;" << escape_xml(author) << "&#160;" << escape_xml(date) << "&#160;]\n";
    } else if(author != "") {
        document << "        [&#160;" << escape_xml(author) << "&#160;]\n";
    } else if(date != "") {
        document << "        [&#160;" << escape_xml(date) << "&#160;]\n";
    }

    document << "    </span>\n";

    if(references != "") {
        // lookup references (refid="17,18")
        std::vector<std::string> refs;
        boost::algorithm::split(refs, references, boost::is_any_of(","));
        BOOST_FOREACH(const std::string& refid, refs) {
            boost::unordered_map<std::string, node_ptr>::const_iterator pos = explicit_markup.notes.find(refid);
            if(pos != explicit_markup.notes.end()) {
                write_contents(document, pos->second);
            } else {
                document << "    " << escape_xml(refid) << "\n";
            }
        }
    }
    if(const node_ptr* node_note = boost::get<node_ptr>(&note)) {
        write_contents(document, *node_note);  
    } else if(const std::string* string_note = boost::get<std::string>(&note)) {
        document << *string_note; // not escaped--can contain html markup
    }

    document << "</div>\n";
}

// requires: any note that is a string contains well-formed html
void boost::regression::show_notes(html_writer& document,
                                   const std::vector<test_structure_t::note_t>& notes,
                                   const failures_markup_t& explicit_markup)
{
    document << "<div class=\"notes\">\n";

    BOOST_FOREACH(const test_structure_t::note_t& note, notes) {

        document << "    <div>\n";

        std::string refid = get_note_attr(note, "refid");
        ::show_note(document, note, refid, explicit_markup);

        document << "    </div>\n";

    }

    document << "</div>\n";
}
