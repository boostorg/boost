// result_page.cpp
//
// Copyright MetaCommunications, Inc. 2003-2007.
// Copyright Steven Watanabe 2010-2011
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "result_page.hpp"
#include "common.hpp"
#include "html.hpp"
#include "html_writer.hpp"
#include "xml.hpp"
#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <fstream>

using namespace boost::regression;

namespace {

// safe: no assumptions, no unconstrained output
void test_type_col(html_writer& document, const std::string& test_type) {
    document << "<td class=\"test-type\">\n";
    document << "    <a href=\"http://www.boost.org/status/compiler_status.html#Understanding\" class=\"legend-link\" target=\"_top\">";

    if(test_type == "run_pyd") {
        document << "r";
    } else if(test_type == "run_mpi") {
        document << "r";
    } else if(test_type == "run") {
        document << "r";
    } else if(test_type == "run_fail") {
        document << "rf";
    } else if(test_type == "compile") {
        document << "c";
    } else if(test_type == "compile_fail") {
        document << "cf";
    } else if(test_type == "link") {
        document << "l";
    } else if(test_type == "link_fail") {
        document << "lf";
    } else {
        throw std::runtime_error("Incorrect test type \"" + test_type + "\"");
    }

    document << "    </a>\n";
    document << "</td>\n";
}

// category/name
typedef std::pair<std::string, std::string> test_case_id_t;
// runner/toolset
typedef std::pair<std::string, std::string> test_toolset_id_t;

typedef std::vector<const test_structure_t::test_log_t*> test_log_group_t;
typedef boost::unordered_map<test_toolset_id_t, test_log_group_t> test_logs_by_run_t;
typedef std::map<test_case_id_t, test_logs_by_run_t> test_logs_t;

// requires: result contains no HTML special characters
// requires: log_link must not contain a '/' derived from the input (This won't actually break anything, though)
void insert_cell_link(html_writer& document, const std::string& result, const std::string& log_link) {
    if(log_link != "") {
        document << "&#160;&#160;"
                    "<a href=\"" << escape_uri(log_link) << "\" class=\"log-link\" target=\"_top\">"
                 << result <<
                    "</a>"
                    "&#160;&#160;";
    } else {
        document << "&#160;&#160;" << result << "&#160;&#160;";
    }
}
     
// requires:
void insert_cell_developer(html_writer& document,
                           const failures_markup_t& explicit_markup,
                           bool release,
                           const std::string& library,
                           const std::string& test_name,
                           const std::string& runner,
                           const std::string& toolset,
                           const test_log_group_t& test_logs) {
    std::string class_ = "library-" + result_cell_class(explicit_markup, library, toolset, test_logs);

    std::string cell_link = (test_logs.size() > 1)?
        encode_path(runner + "-" + library + "-" + toolset + "-" + test_logs.front()->test_name + "-variants_" + release_postfix(release)) + ".html" :
        (test_logs.empty())?
        std::string("") :
        log_file_path(explicit_markup, *test_logs.front(), runner, release_postfix(release));

    document << "<td class=\"" << class_ << "\" title=\"" << escape_xml(test_name) << "/" << escape_xml(toolset) << "\">\n";

    if(is_unusable(explicit_markup, library, toolset)) {
        insert_cell_link(document, "n/a", cell_link);
    } else if(test_logs.empty()) {
        document << "&#160;&#160;&#160;&#160;\n";
    } else {
        BOOST_FOREACH(test_log_group_t::value_type log, test_logs) {
            if(!log->result && log->status) {
                insert_cell_link(document, (log->expected_reason != "")? "fail?" : "fail*", cell_link);
                goto done;
            }
        }
        BOOST_FOREACH(test_log_group_t::value_type log, test_logs) {
            if(!log->result && !log->status) {
                insert_cell_link(document, "fail", cell_link);
                goto done;
            }
        }
        insert_cell_link(document, "pass", cell_link);
    }
done:
    document << "</td>\n";
}

// requires: 
void insert_cell_user(html_writer& document,
                      const failures_markup_t& explicit_markup,
                      bool release,
                      const std::string& library,
                      const std::string& test_name,
                      const std::string& runner,
                      const std::string& toolset,
                      const test_log_group_t& test_logs) {
    std::string class_ = "library-" + result_cell_class(explicit_markup, library, toolset, test_logs);

    std::string cell_link = (test_logs.size() > 1)?
        encode_path(runner + "-" + library + "-" + toolset + "-" + test_logs.front()->test_name + "-variants_" + release_postfix(release)) + ".html" :
        (test_logs.empty())?
        std::string("") :
        log_file_path(explicit_markup, *test_logs.front(), runner, release_postfix(release));

    document << "<td class=\"" << class_ << " user-" << class_ << "\" title=\"" << escape_xml(test_name) << "/" << escape_xml(toolset) << "\">\n";

    if(is_unusable(explicit_markup, library, toolset)) {
        insert_cell_link(document, "unusable", cell_link);
    } else if(test_logs.empty()) {
        document << "&#160;&#160;&#160;&#160;\n";
    } else {
        BOOST_FOREACH(test_log_group_t::value_type log, test_logs) {
            if(!log->result && log->status) {
                insert_cell_link(document, (log->expected_reason != "")? "fail?" : "fail*", cell_link);
                goto done;
            }
        }
        BOOST_FOREACH(test_log_group_t::value_type log, test_logs) {
            if(!log->result && !log->status) {
                insert_cell_link(document, "fail", cell_link);
                goto done;
            }
        }
        insert_cell_link(document, "pass", cell_link);
    }
done:
    document << "</td>\n";
}

// requires: line_mod should be from an enumerated set
// requires: source is a branch of svn
// requires: mode = developer | user
void insert_test_line(html_writer& document,
                      const failures_markup_t& explicit_markup,
                      bool release,
                      const std::string& library,
                      test_logs_t::const_reference test_results,
                      const std::vector<std::pair<std::string, std::string> >& all_toolsets,
                      const std::string& line_mod,
                      const std::string& source,
                      const std::string& mode) {
    // This is guaranteed to exist because of the way the nested maps are created
    const test_structure_t::test_log_t * first_log = (*test_results.second.begin()).second.front();
    std::string test_program(first_log->test_program);

    std::string test_header =
        "<td class=\"test-name\">\n"
        "    <a href=\"http://svn.boost.org/svn/boost/" + source + "/" + escape_uri(test_program) + "\" class=\"test-link\" target=\"_top\">\n"
        "        " + escape_xml(test_results.first.second) + "\n" // FIXME: sanitize test name
        "    </a>\n"
        "</td>\n";

    document << "<tr class=\"library-row" << line_mod << "\">\n"
             << test_header;

    test_log_group_t empty_test_log;
    test_type_col(document, first_log->test_type);
    BOOST_FOREACH(const test_toolset_id_t& run, all_toolsets) {
        const std::string& toolset = run.second;
        const std::string& runner = run.first;

        test_logs_by_run_t::const_iterator pos = test_results.second.find(run);
        
        const test_log_group_t* test_result_for_toolset = 
            (pos != test_results.second.end())?
            &pos->second :
            &empty_test_log;

        if(mode == "user") {
            insert_cell_user(document, explicit_markup, release, library, test_results.first.second, runner, toolset, *test_result_for_toolset);
        } else {
            insert_cell_developer(document, explicit_markup, release, library, test_results.first.second, runner, toolset, *test_result_for_toolset);
        }
    }

    document << test_header
             << "</tr>\n";
}

// requires: source is a branch of svn
// requires: mode = developer | user
void insert_test_section(html_writer& document,
                         const test_structure_t& test_structure,
                         const failures_markup_t& explicit_markup,
                         bool release,
                         const std::string& library,
                         const test_logs_t& logs,
                         const std::vector<std::pair<std::string, std::string> >& all_toolsets,
                         const std::string& source,
                         const std::string& mode) {
    std::size_t category_span = 3;
    BOOST_FOREACH(test_structure_t::platform_group_t::const_reference platform, test_structure.platforms) {
        BOOST_FOREACH(test_structure_t::platform_t::const_reference run, platform.second) {
            category_span += (run.toolsets.size());
        }
    }

    for(test_logs_t::const_iterator pos = logs.begin(), end = logs.end(); pos != end; ++pos) {
        std::string test_name = pos->first.second;
        bool category_start = (pos == logs.begin()) || (pos->first.first != boost::prior(pos)->first.first);
        bool category_end   = (boost::next(pos) == end) || (pos->first.first != boost::next(pos)->first.first);

        std::string line_mod =
            (category_start && category_end)? "-single" :
            category_start? "-first" :
            category_end? "-last" :
            "";

        if(category_start && pos->first.first != "0") {
            document << "<tr>\n"
                        "    <td class=\"library-test-category-header\" colspan=\"" << category_span << "\" align=\"center\">\n"
                        "        " << escape_xml(pos->first.first) << "\n"
                        "    </td>\n"
                        "</tr>\n";
        }

        insert_test_line(document, explicit_markup, release, library, *pos, all_toolsets, line_mod, source, mode);
    }
}

}

// requires: mode = developer | user
// requires: source = Boost SVN branch name
void boost::regression::result_page(const test_structure_t& tests,
                                    const failures_markup_t& explicit_markup,
                                    bool release,
                                    const std::string& source,
                                    const boost::posix_time::ptime& run_date,
                                    const std::vector<std::string>& warnings,
                                    const std::string& mode,
                                    const boost::filesystem::path& comment_file)
{
    // skip debug.xml

    std::string index_path("index" + release_postfix(release) + "_.html");
        
    {

    std::cout << "Writing document " << "index" << release_postfix(release) << ".html" << std::endl;

    html_writer index(mode + "/" + "index" + release_postfix(release) + ".html");
    index << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Frameset//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd\">\n"
             "<html>\n"
             "<head>\n"
             "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n"
             "    <link rel=\"stylesheet\" type=\"text/css\" href=\"../master.css\" title=\"master\" />\n"
             "    <title>Boost regression: " << source << "</title>\n"
             "</head>\n"
             "<frameset cols=\"190px,*\" frameborder=\"0\" framespacing=\"0\" border=\"0\">\n"
             "    <frame name=\"tocframe\" src=\"toc" << release_postfix(release) << ".html\" scrolling=\"auto\"/>\n"
             "    <frame name=\"docframe\" src=\"" << index_path << "\" scrolling=\"auto\"/>\n"
             "</frameset>\n"
             "</html>\n";
    }

    std::cout << "Writing document " << index_path << std::endl;

    {
        html_writer index(mode + "/" + index_path);
        index << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
                 "<html>\n"
                 "<head>\n"
                 "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n"
                 "    <link rel=\"stylesheet\" type=\"text/css\" href=\"../master.css\" title=\"master\" />\n"
                 "    <title>Boost regression: " << source << "</title>\n"
                 "</head>\n"
                 "<body>\n"
                 "\n"
                 "    <img border=\"0\" src=\"http://www.boost.org/boost.png\" width=\"277\" height=\"86\" align=\"right\" alt=\"Boost logo\"></img>\n"
                 "\n"
                 "    <h1 class=\"page-title\">\n"
                          << mode << " report: "
                 "        <a class=\"hover-link\" href=\"summary.html\" target=\"_top\">" << source << "</a>\n"
                 "    </h1>\n"
                 "\n";

        std::string purpose = (mode == "user")?
            "The purpose of this report is to help a user to find out whether a particular library "
            "works on the particular compiler(s). For SVN \"health report\", see "
            "       <a href=\"../developer/index.html\" target=\"_top\">developer summary</a>."
            :
            "Provides Boost developers with visual indication of the SVN \"health\". For user-level "
            "report, see <a href=\"../user/index.html\" target=\"_top\">user summary</a>.";

        insert_report_header(index, run_date, warnings, purpose);

        index << "    <div class=\"comment\">\n";
        if(comment_file != "") {
            std::ifstream comment(comment_file.string().c_str());
            if(!comment) {
                throw std::ios_base::failure("Could not open file " + comment_file.string());
            }
            std::string comment_data(std::istreambuf_iterator<char>(comment.rdbuf()), std::istreambuf_iterator<char>());
            index << comment_data;
        }
        index << "    </div>\n";

        index << "</body>\n";
        index << "</html>\n";
    }

    std::set<std::string> libraries;
    get_libraries(tests, libraries);

    {
        std::string toc_path("toc" + release_postfix(release) + ".html");
        
        std::cout << "Writing document " << toc_path << std::endl;

        html_writer toc(mode + "/" + toc_path);

        toc << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
            << "<html>\n"
            << "<head>\n"
            << "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n"
            << "    <link rel=\"stylesheet\" type=\"text/css\" href=\"../master.css\" title=\"master\"/>\n"
            << "    <title>Boost regression: " << source << "</title>\n"
            << "</head>\n"
            << "<body class=\"" << mode << "-toc\">\n"
            << "    <div class=\"toc-header-entry\">\n"
            << "        <a href=\"index" << release_postfix(release) << ".html\" class=\"toc-entry\" target=\"_top\">Report info</a>\n"
            << "    </div>\n"
            << "    <div class=\"toc-header-entry\">\n"
            << "        <a href=\"summary" << release_postfix(release) << ".html\" class=\"toc-entry\" target=\"_top\">Summary</a>\n"
            << "    </div>\n";

        if(mode == "developer") {
            toc << "    <div class=\"toc-header-entry\">\n"
                   "        <a href=\"issues.html\" class=\"toc-entry\" target=\"_top\">Unresolved issues</a>\n"
                   "    </div>\n";
        }

        toc << "    <div class=\"toc-header-entry\">\n";

        insert_view_link(toc, "index", "toc-entry", release);

        toc << "    </div>\n";

        toc << "    <hr/>\n";

        BOOST_FOREACH(const std::string& library, libraries) {
            std::string library_page(encode_path(library));
            toc << "    <div class=\"toc-entry\">\n"
                   "        <a href=\"" << escape_uri(library_page) << release_postfix(release) << ".html\" class=\"toc-entry\" target=\"_top\">\n"
                   "            " << escape_xml(library) <<  "\n"
                   "        </a>\n"
                   "    </div>\n";
        }

        toc << "</body>\n"
               "</html>\n";
    }

    BOOST_FOREACH(const std::string& library, libraries) {
        if(show_library(explicit_markup, library, release)) {
            
            std::string library_results(encode_path(library) + release_postfix(release) + "_.html");
            std::string library_page(encode_path(library) + release_postfix(release) + ".html");

            std::cout << "Writing document " << library_page << std::endl;

            {
                html_writer document(mode + "/" + library_page);

                document << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Frameset//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd\">\n"
                            "<html>\n"
                            "<head>\n"
                            "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n"
                            "    <link rel=\"stylesheet\" type=\"text/css\" href=\"../master.css\" title=\"master\"/>\n"
                            "    <title>Boost regression: " << escape_xml(library) << "/" << source << "</title>\n"
                            "</head>\n"
                            "<frameset cols=\"190px,*\" frameborder=\"0\" framespacing=\"0\" border=\"0\">\n"
                            "<frame name=\"tocframe\" src=\"toc" << release_postfix(release) << ".html\" scrolling=\"auto\"/>\n"
                            "<frame name=\"docframe\" src=\"" << escape_uri(library_results) << "\" scrolling=\"auto\"/>\n"
                            "</frameset>\n"
                            "</html>\n";
            }

            std::cout << "Writing document " << library_results << std::endl;
            
            {
                html_writer document(mode + "/" + library_results);

                document << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
                            "<html>\n"
                            "<head>\n"
                            "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n"
                            "    <link rel=\"stylesheet\" type=\"text/css\" href=\"../master.css\" title=\"master\" />\n"
                            "    <title>Boost regression: " << escape_xml(library) << "/" << source << "</title>\n"
                            "</head>\n"
                            "\n"
                            "<body>\n";

                insert_page_links(document, encode_path(library), release, alternate_mode(mode));

                document << "<h1 class=\"page-title\">\n"
                            "    <a class=\"hover-link\" name=\"" << escape_xml(library) << "\" href=\"http://www.boost.org/libs/" << escape_uri(library) << "\" target=\"_top\">"
                         << escape_xml(library) <<
                            "</a>"
                            "/"
                            "<a class=\"hover-link\" href=\"summary.html\" target=\"_top\">" << source << "</a>\n"
                            "</h1>\n";

                insert_report_header(document, run_date, warnings);

                // toolset/note/index
                boost::unordered_map<std::string, std::size_t> library_marks;
                std::vector<node_ptr> notes;
                get_unusable(explicit_markup, library, tests, library_marks, notes);

                document << "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" class=\"library-table\" width=\"1%\" summary=\"Library results\">\n"
                            "    <thead>\n";

                insert_runners_rows(document, "details", "top", tests, run_date); // okay
                insert_toolsets_row(document, tests, explicit_markup, "details", run_date, library, library_marks);
                      
                document << "    </thead>\n"
                            "    <tfoot>\n";
                
                insert_toolsets_row(document, tests, explicit_markup, "details", run_date, library, library_marks);
                insert_runners_rows(document, "details", "bottom", tests, run_date);
                document << "    </tfoot>\n"
                            "    <tbody>\n";

                test_logs_t lib_tests;
                std::vector<std::pair<std::string, std::string> > all_toolsets;
                BOOST_FOREACH(test_structure_t::platform_group_t::const_reference platform, tests.platforms) {
                    BOOST_FOREACH(test_structure_t::platform_t::const_reference run, platform.second) {
                        BOOST_FOREACH(test_structure_t::toolset_group_t::const_reference toolset, run.toolsets) {
                            all_toolsets.push_back(std::make_pair(run.runner, toolset.first));
                            test_structure_t::toolset_t::const_iterator pos = toolset.second.find(library);
                            if(pos != toolset.second.end()) {
                                BOOST_FOREACH(test_structure_t::library_t::const_reference test_case, pos->second) {
                                    test_log_group_t test_logs;
                                    BOOST_FOREACH(test_structure_t::test_case_t::const_reference log, test_case.second) {
                                        if(is_test_log_a_test_case(log)) {
                                            test_logs.push_back(&log);
                                        }
                                    }
                                    if(!test_logs.empty()) {
                                        std::string category = test_logs.front()->category;
                                        lib_tests[std::make_pair(category, test_case.first)][std::make_pair(run.runner, toolset.first)] = test_logs;
                                    }
                                }
                            }
                        }
                    }
                }

                insert_test_section(document, tests, explicit_markup, release, library, lib_tests, all_toolsets, source, mode);

                document << "    </tbody>\n"
                            "</table>\n";

                if(!notes.empty()) {
                    document << "<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" class=\"library-library-notes\" summary=\"library notes\">\n";
                    for(std::size_t i = 0; i < notes.size(); ++i) {
                        document << "<tr class=\"library-library-note\">\n"
                                    "    <td valign=\"top\" width=\"3em\">\n"
                                    "        <a name=\"" << escape_uri(library) << "-note-" << (i + 1) << "\">\n"
                                    "            <span class=\"super\">" << (i + 1) << "</span>\n"
                                    "        </a>\n"
                                    "    </td>\n"
                                    "    <td>\n";
                        std::string refid;
                        lookup_attr(notes[i], "refid", refid);
                        show_note(document, notes[i], refid, explicit_markup);
                        document << "    </td>\n"
                                    "</tr>\n";
                    }
                    document << "</table>\n";
                }

                document << "<div id=\"legend\">\n"
                         << (mode == "developer"? library_developer_legend : library_user_legend) << "\n"
                            "</div>\n";

                insert_page_links(document, encode_path(library), release, alternate_mode(mode));

                document << "</body>\n";
                document << "</html>\n";
            }
        }
    }
}
