// issues_page.cpp
//
// Copyright MetaCommunications, Inc. 2003-2004.
// Copyright Steven Watanabe 2010
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "issues_page.hpp"
#include "html_writer.hpp"
#include "xml.hpp"
#include "html.hpp"
#include "common.hpp"

#include <map>
#include <string>
#include <boost/foreach.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

using namespace boost::regression;

typedef std::pair<const test_structure_t::test_log_t*, const std::string*> test_failure_t;
typedef std::map<std::string, std::map<std::string, std::vector<test_failure_t> > > library_test_names_t;
typedef std::map<std::string, library_test_names_t> libraries_t;

namespace {

void failing_tests(const test_structure_t& tests,
                   const failures_markup_t& explicit_markup,
                   bool release,
                   libraries_t& out)
{
    typedef boost::unordered_map<std::string, test_structure_t::platform_t> test_structure_top;
    BOOST_FOREACH(test_structure_top::const_reference platform, tests.platforms) {
        BOOST_FOREACH(const test_structure_t::run_t& runs, platform.second) {
            BOOST_FOREACH(test_structure_t::toolset_group_t::const_reference toolset, runs.toolsets) {
                BOOST_FOREACH(test_structure_t::toolset_t::const_reference library, toolset.second) {
                    BOOST_FOREACH(test_structure_t::library_t::const_reference test_case, library.second) {
                        BOOST_FOREACH(test_structure_t::test_case_t::const_reference test_log, test_case.second) {
                            if(test_log.status == false && test_log.result == false &&
                               explicit_markup.required_toolsets.find(test_log.toolset) != explicit_markup.required_toolsets.end() &&
                               is_test_log_a_test_case(test_log) &&
                               show_library(explicit_markup, library.first, release) &&
                               show_toolset(explicit_markup, toolset.first, release) &&
                               !(is_unusable(explicit_markup, library.first, toolset.first))) {
                               out[library.first][test_log.test_name][test_log.toolset].push_back(std::make_pair(&test_log, &runs.runner));
                            }
                        }
                    }
                }
            }
        }
    }
}

std::size_t count_failures(const library_test_names_t& library) {
    std::size_t result = 0;
    BOOST_FOREACH(library_test_names_t::const_reference test, library) {
        BOOST_FOREACH(library_test_names_t::mapped_type::const_reference toolset, test.second) {
            result += toolset.second.size();
        }
    }
    return result;
}

// okay
void write_issues_list_reference_file(const std::string& out,
                                      const std::string& source,
                                      bool release,
                                      const std::string& issues_list)
{
    html_writer document(out);
    document << "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01 Frameset//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-frameset.dtd\">\n";
    document << "<html>\n";
    document << "  <head>\n";
    document << "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n";
    document << "    <link rel=\"stylesheet\" type=\"text/css\" href=\"../master.css\" title=\"master\" />\n";
    document << "    <title>Boost regression unresolved issues: " << source << "</title>\n";
    document << "  </head>\n";
    document << "  <frameset cols=\"190px,*\" frameborder=\"0\" framespacing=\"0\" border=\"0\">\n";
    document << "    <frame name=\"tocframe\" src=\"toc" << release_postfix(release) << ".html\" scrolling=\"auto\"/>\n";
    document << "    <frame name=\"docframe\" src=\"" << issues_list << "\" scrolling=\"auto\"/>\n";
    document << "  </frameset>\n";
    document << "</html>\n";
}

void print_failure_cell(html_writer& document,
                        const failures_markup_t& explicit_markup,
                        const std::string& output_directory,
                        const test_structure_t::test_log_t& test_log,
                        const std::string& toolset,
                        const std::string& runner,
                        const std::string& release_postfix)
{
    std::string log_link = log_file_path(explicit_markup, test_log, runner, release_postfix);

    const char* class_ = test_log.is_new?
        "library-fail-unexpected-new" :
        "library-fail-unexpected";
    
    document << "<td class=\"" << class_ << "\">\n";
    document << "  <span>\n";
    document << "    <a href=\"" << escape_uri(log_link) << "\" class=\"log-link\" target=\"_top\">\n";
    document << "      " << escape_xml(toolset) << "\n";
    document << "    </a>\n";
    document << "  </span>\n";
    document << "</td>\n";
}

void write_issues_list(const std::string& path,
                       const failures_markup_t& explicit_markup,
                       const std::string& output_directory,
                       const libraries_t& libraries,
                       const std::string& source,
                       const boost::posix_time::ptime& run_date,
                       const std::vector<std::string>& warnings,
                       const std::string& purpose,
                       bool release)
{
    //utils::log("Writing document " + path);
    
    const char* release_postfix = release? "_release" : "";

    html_writer document(path);

    document << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
      
    document << "<html>\n";
    document << "  <head>\n";
    document << "    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"/>\n";
    document << "    <link rel=\"stylesheet\" type=\"text/css\" href=\"../master.css\" title=\"master\" />\n";
    document << "    <title>Boost regression unresolved issues: " << source << "</title>\n";
    document << "  </head>\n";
    document << "  <body>\n";
    document << "\n";
    document << "    <h1 class=\"page-title\">\n";
    document << "      Unresolved Issues: \n";
    document << "      <a class=\"hover-link\" href=\"summary" << release_postfix << ".html\" target=\"_top\">" << source << "</a>\n";
    document << "    </h1>\n";
    document << "\n";

    insert_report_header(document, run_date, warnings, purpose);

    // Emit the index
    document << "    <h2>Libraries with unresolved failures</h2>\n";
    document << "    <div align=\"center\">\n";

    BOOST_FOREACH(libraries_t::const_reference library, libraries) {
        document << "      <a href=\"#" << escape_uri(library.first) << "\">\n";
        document << "        " << escape_xml(library.first) << "\n";
        document << "      </a>\n";
    }

    BOOST_FOREACH(libraries_t::const_reference library, libraries) {
        std::string library_page(encode_path(library.first));
        const library_test_names_t& library_test_names(library.second);
        std::size_t failures = count_failures(library.second);

        document << "      <h2>\n";
        document << "        <a name=\"" << escape_uri(library.first) << "\"/>\n";
        document << "        <a class=\"hover-link\" href=\"" << escape_uri(library_page)
                 << release_postfix << ".html\" target=\"_top\">\n";
        document << "        " << escape_xml(library.first) << " (" << failures
                 << " failure" << (failures == 1? "":"s") << ")\n";
        document << "        </a>\n";
        document << "      </h2>\n";
            
        document << "      <table class=\"library-issues-table\" summary=\"issues\">\n";
        document << "      <thead>\n";
        document << "        <tr valign=\"middle\">\n";
        document << "          <td class=\"head\">test</td>\n";
        document << "          <td class=\"head\">failures</td>\n";
        document << "        </tr>\n";
        document << "      </thead>\n";
        document << "      <tfoot>\n";
        document << "        <tr valign=\"middle\">\n";
        document << "          <td class=\"head\">test</td>\n";
        document << "          <td class=\"head\">failures</td>\n";
        document << "        </tr>\n";
        document << "      </tfoot>\n";

        document << "      <tbody>\n";

        BOOST_FOREACH(library_test_names_t::const_reference test, library_test_names) {
            const std::string& test_name = test.first;
            const std::string& test_program = test.second.begin()->second.front().first->test_program;

            document << "        <tr>\n";
            document << "          <td class=\"test-name\">\n";
            document << "            <a href=\"http://svn.boost.org/svn/boost/"
                     << source << "/" << escape_uri(test_program) << "\" class=\"test-link\" target=\"_top\">\n";
            document << "              " << escape_xml(test_name) << "\n";
            document << "            </a>\n";
            document << "          </td>\n";
            document << "          <td class=\"failures-row\">\n";
            document << "            <table summary=\"unexpected fail legend\" class=\"issue-box\">\n";
            document << "              <tr class=\"library-row-single\">\n";

            typedef library_test_names_t::mapped_type::const_reference toolset_t;
            BOOST_FOREACH(toolset_t toolset, test.second) {
                BOOST_FOREACH(const test_failure_t& failure, toolset.second) {
                    print_failure_cell(document, explicit_markup, output_directory, *failure.first, toolset.first, *failure.second, release_postfix);
                }
            }

            document << "              </tr>\n";
            document << "            </table>\n";
            document << "          </td>\n";
            document << "        </tr>\n";
        }
        document << "      </tbody>\n";

        document << "    </table>\n";
    }
    document << "    </div>\n";
    document << issues_legend;
    document << "  </body>\n";
    document << "</html>\n";
}

// okay
void write_issues_email(const std::string& path,
                        const boost::posix_time::ptime& run_date,
                        const std::string& source,
                        const libraries_t& libraries)
{
    boost::filesystem::ofstream document(path);
    std::cout << "Writing document " << path << std::endl;
    //utils::log(boost::format("Writing document %s") % path);

    std::size_t failing_tests = 0;
    BOOST_FOREACH(libraries_t::const_reference library, libraries) {
        failing_tests += count_failures(library.second);
    }
    
    document << "Boost regression test failures\n"
"------------------------------\n"
"Report time: " << run_date << "\n"
"\n"
"This report lists all regression test failures on release platforms.\n"
"\n"
"Detailed report: \n"
"        http://beta.boost.org/development/tests/" << source << "/developer/issues.html\n"
"\n"
<< failing_tests << " failure" << (failing_tests == 1? "" : "s")
<< " in " << libraries.size() << " librar" << (libraries.size() == 1? "y" : "ies") << ":\n";

    BOOST_FOREACH(libraries_t::const_reference library, libraries) {
        document << "  " << library.first << " (" << count_failures(library.second) << ")\n";
    }

    BOOST_FOREACH(libraries_t::const_reference library, libraries) {

        std::string library_page = encode_path(library.first);
        document << "\n"
        "|" << library.first << "|\n";

        BOOST_FOREACH(libraries_t::mapped_type::const_reference test_name, library.second) {
            document << "  " << test_name.first << ":";
            BOOST_FOREACH(libraries_t::mapped_type::mapped_type::const_reference toolset, test_name.second) {
                document << "  " << toolset.first;
            }
            document << "\n";
        }
    }
}

}

void boost::regression::issues_list(const std::string& output_dir,
                                    const test_structure_t& tests,
                                    const failures_markup_t& explicit_markup,
                                    bool release,
                                    const std::string& source,
                                    const boost::posix_time::ptime& run_date,
                                    const std::vector<std::string>& warnings,
                                    const std::string& purpose)
{
    ::libraries_t libraries;
    failing_tests(tests, explicit_markup, release, libraries);
    
    std::string release_postfix_(release_postfix(release));
    std::string issues_list("issues" + release_postfix_ + "_.html");

    write_issues_list_reference_file(output_dir + "/issues.html", source, release, issues_list);
    write_issues_list(output_dir + "/" + issues_list, explicit_markup, output_dir, libraries, source, run_date, warnings, purpose, release);

    write_issues_email(output_dir + "/issues-email.txt",
                       run_date,
                       source,
                       libraries);
}
