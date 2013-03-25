// Copyright MetaCommunications, Inc. 2003-2004.
// Copyright Steven Watanabe 2013
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/foreach.hpp>
#include <boost/next_prior.hpp>
#include "common.hpp"
#include "summary_page.hpp"
#include "html_writer.hpp"
#include "html.hpp"
#include <iostream>

using namespace boost::regression;

namespace {

// report developer status
// safe
void insert_cell_developer(html_writer& document,
                           const failures_markup_t& explicit_markup,
                           const std::string& library,
                           const std::string& toolset,
                           const test_structure_t::library_t& current_cell,
                           bool release) {

    std::string class_ = "summary-" + result_cell_class(explicit_markup, library, toolset, current_cell);
    
    std::string library_page = encode_path(library);

    document << "<td class=\"" << class_ << "\" title=\"" << escape_xml(library) << "/" << escape_xml(toolset) << "\">\n";

    if(class_ == "summary-unusable") {
        document << "&#160;&#160;"
                    "<a href=\"" << escape_uri(library_page) << release_postfix(release) << ".html\" class=\"log-link\" target=\"_top\">"
                    "n/a"
                    "</a>"
                    "&#160;&#160;";
    } else if(class_ == "summary-missing") {
        document << "&#160;&#160;&#160;&#160;";
    } else if(class_ == "summary-fail-unexpected") {
        document << "<a href=\"" << escape_uri(library_page) << release_postfix(release) << ".html\" class=\"log-link\" target=\"_top\">"
                    "broken"
                    "</a>";
    } else if(class_ == "summary-fail-unexpected-new") {
        document << "&#160;&#160;"
                    "<a href=\"" << escape_uri(library_page) << release_postfix(release) << ".html\" class=\"log-link\" target=\"_top\">"
                    "fail"
                    "</a>"
                    "&#160;&#160;"; 
    } else {
        document << "&#160;&#160;OK&#160;&#160;";
    }
    document << "</td>\n";
}

// report user status
// safe
void insert_cell_user(html_writer& document,
                      const failures_markup_t& explicit_markup,
                      const std::string& library,
                      const std::string& toolset,
                      const test_structure_t::library_t& current_cell,
                      bool release) {
    std::string class_ = "summary-" + result_cell_class(explicit_markup, library, toolset, current_cell);
    
    std::string library_page = encode_path(library);

    document << "<td class=\"" << class_ << " user-" << class_ << "\" title=\"" << escape_xml(library) << "/" << escape_xml(toolset) << "\">\n";

    if(class_ == "summary-unusable") {
        document << "&#160;"
                    "<a href=\"" << escape_uri(library_page) << release_postfix(release) << ".html\" class=\"log-link\" target=\"_top\">"
                    "unusable"
                    "</a>"
                    "&#160;";
    } else if(class_ == "summary-missing") {
        document << "&#160;no&#160;results&#160;";
    } else if(class_ == "summary-fail-unexpected") {
        document << "&#160;"
                    "<a href=\"" << escape_uri(library_page) << release_postfix(release) << ".html\" class=\"log-link\" target=\"_top\">"
                    "regress."
                    "</a>"
                    "&#160;";
    } else if(class_ == "summary-fail-unexpected-new" ||
              class_ == "summary-fail-expected" ||
              class_ == "summary-unknown-status" ||
              class_ == "summary-fail-unexpected-unresearched") {
        document << "&#160;"
                    "<a href=\"" << escape_uri(library_page) << release_postfix(release) << ".html\" class=\"log-link\" target=\"_top\">"
                    "details"
                    "</a>"
                    "&#160;"; 
    } else {
        document << "&#160;pass&#160;";
    }
    document << "</td>\n";
}

}

// requires: mode = developer | user
// requires: source is the name of an SVN branch
void boost::regression::summary_page(const std::string& mode,
                                     const std::string& source,
                                     const boost::posix_time::ptime& run_date,
                                     const std::vector<std::string>& warnings,
                                     const test_structure_t& tests,
                                     const failures_markup_t & explicit_markup,
                                     bool release) {

    std::set<std::string> sorted_libraries;
    get_libraries(tests, sorted_libraries);

    std::string summary_results("summary" + release_postfix(release) + "_.html");

    std::cout << "Writing document " << "summary" << release_postfix(release) << ".html" << std::endl;

    {
        html_writer document(mode + "/" + "summary" + release_postfix(release) + ".html");

        document << "<!DOCTYPE html PUBLIC \"-//W3C//DTD html 4.01 Transitional//EN\">\n"
                    "<html>\n"
                    "    <head>\n"
                    "        <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n"
                    "        <link rel=\"stylesheet\" type=\"text/css\" href=\"../master.css\" title=\"master\"/>\n"
                    "        <title>Boost regression summary: " << source << "</title>\n"
                    "    </head>\n"
                    "    <frameset cols=\"190px,*\" frameborder=\"0\" framespacing=\"0\" border=\"0\">\n"
                    "    <frame name=\"tocframe\" src=\"toc" << release_postfix(release) << ".html\" scrolling=\"auto\"/>\n"
                    "    <frame name=\"docframe\" src=\"" << summary_results << "\" scrolling=\"auto\"/>\n"
                    "    </frameset>\n"
                    "</html>\n";
    }

    // Summary results
    std::cout << "Writing document " << summary_results << std::endl;

    {
        html_writer document(mode + "/" + summary_results);

        document << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\">\n"
                    "<html>\n"
                    "<head>\n"
                    "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n"
                    "    <link rel=\"stylesheet\" type=\"text/css\" href=\"../master.css\" title=\"master\"/>\n"
                    "</head>\n"
                    "<body>\n";

        insert_page_links(document, "summary", release, alternate_mode(mode));

        document << "<h1 class=\"page-title\">\n"
                    "    Summary: \n"
                    "    <a class=\"hover-link\" href=\"summary" << release_postfix(release) << ".html\" target=\"_top\">" << source << "</a>\n"
                    "</h1>\n";

        insert_report_header(document, run_date, warnings);

        std::size_t num_unusable = 0;
        std::size_t num_regressions = 0;
        std::size_t num_new_failures = 0;

        BOOST_FOREACH(test_structure_t::platform_group_t::const_reference platform, tests.platforms) {
            BOOST_FOREACH(test_structure_t::platform_t::const_reference run, platform.second) {
                BOOST_FOREACH(test_structure_t::toolset_group_t::const_reference toolset, run.toolsets) {
                    BOOST_FOREACH(test_structure_t::toolset_t::const_reference library, toolset.second) {
                        bool unusable = is_unusable(explicit_markup, library.first, toolset.first);
                        BOOST_FOREACH(test_structure_t::library_t::const_reference test_case, library.second) {
                            BOOST_FOREACH(test_structure_t::test_case_t::const_reference test_log, test_case.second) {
                                if(unusable) ++num_unusable;
                                else if(!test_log.result && !test_log.status) {
                                    if(test_log.is_new) ++num_new_failures;
                                    else ++num_regressions;
                                }
                            }
                        }
                    }
                }
            }
        }

        document << "<div class=\"statistics\">\n"
                    "Unusable: " << num_unusable << "\n"
                    "&#160;|&#160;\n"
                    "Regressions: " << num_regressions << "\n"
                    "&#160;|&#160;\n"
                    "New failures: " << num_new_failures << "\n"
                    "</div>\n";
            
       // summary table

       document << "<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" width=\"1%\" class=\"summary-table\" summary=\"Overall summary\">\n";

       document << "<thead>\n";
       insert_runners_rows(document, "summary", "top", tests, run_date);
       insert_toolsets_row(document, tests, explicit_markup, "summary", run_date);
       document << "</thead>\n";

       document << "<tfoot>\n";
       insert_toolsets_row(document, tests, explicit_markup, "summary", run_date);
       insert_runners_rows(document, "summary", "bottom", tests, run_date);
       document << "</tfoot>\n";
          
       document << "<tbody>\n";

       BOOST_FOREACH(const std::string& library, sorted_libraries) {
           std::string library_page = encode_path(library);

           std::string library_header =
                        "<td class=\"library-name\">\n"
                        "    <a href=\"" + escape_uri(library_page) + release_postfix(release) + ".html\" class=\"library-link\" target=\"_top\">\n"
                        "        " + escape_xml(library) + "\n"
                        "    </a>\n"
                        "</td>\n";

           std::string line_mod;
           if(sorted_libraries.size() == 1) line_mod = "-single";
           else if(library == *sorted_libraries.begin()) line_mod = "-first";
           else if(library == *boost::prior(sorted_libraries.end())) line_mod = "-last";

           document << "<tr class=\"summary-row" << line_mod << "\">\n";
           document << library_header;

           test_structure_t::library_t empty_library;
            BOOST_FOREACH(test_structure_t::platform_group_t::const_reference platform, tests.platforms) {
                BOOST_FOREACH(test_structure_t::platform_t::const_reference run, platform.second) {
                    BOOST_FOREACH(test_structure_t::toolset_group_t::const_reference toolset, run.toolsets) {
                        test_structure_t::toolset_t::const_iterator pos = toolset.second.find(library);
                        const test_structure_t::library_t * current_cell =
                            (pos != toolset.second.end())?
                                &pos->second : &empty_library;
                        if(mode == "user") {
                            insert_cell_user(document, explicit_markup, library, toolset.first, *current_cell, release);
                        } else {
                            insert_cell_developer(document, explicit_markup, library, toolset.first, *current_cell, release);
                        }
                    }
                }
            }
                    
            document << library_header;
            document << "</tr>\n";
       }
        
        document << "</tbody>\n";
        document << "</table>\n";

        document << "<div id=\"legend\">\n"
                 << (mode == "developer"? summary_developer_legend : summary_user_legend) << "\n"
                    "</div>\n";

        insert_page_links(document, "summary", release, alternate_mode(mode));

        document << "</body>\n";
        document << "</html>\n";
    }
}
