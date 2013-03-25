// Copyright MetaCommunications, Inc. 2003-2006.
// Copyright Steven Watanabe 2010
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "links_page.hpp"
#include "xml.hpp"
#include "common.hpp"
#include "html_writer.hpp"
#include "html.hpp"

#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <vector>

using namespace boost::regression;

namespace {

void links_page(const boost::posix_time::ptime& run_date,
                const failures_markup_t& explicit_markup,
                const std::string& runner_id,
                const std::string& revision,
                const boost::posix_time::ptime& timestamp,
                const std::string& library_name,
                const std::string& toolset_name,
                const std::string& test_name,
                const std::vector<test_structure_t::test_log_t>& test_logs);
void write_variants_reference_file(const std::string& path,
                                   const std::string& variants_file_path,
                                   const std::string release_postfix);
std::string output_page_header(node_ptr test_log, const std::string& runner_id);
void write_variants_file(const boost::posix_time::ptime& run_date,
                         const failures_markup_t& explicit_markup,
                         const std::string& path,
                         const std::vector<test_structure_t::test_log_t>& test_logs,
                         const std::string& runner_id,
                         const std::string& revision,
                         const boost::posix_time::ptime& timestamp);
void write_test_result_file(const boost::posix_time::ptime& run_date,
                            const failures_markup_t& explicit_markup,
                            const std::string& path,
                            const test_structure_t::test_log_t& test_log,
                            const std::string& runner_id,
                            const std::string& revision,
                            const boost::posix_time::ptime& timestamp);
void write_test_results_reference_file(const std::string& path, const std::string& log_file_path);

// requires: revision must be a SVN revision.  i.e. of the form nnnnn
void links_page(const boost::posix_time::ptime& run_date,
                const failures_markup_t& explicit_markup,
                const std::string& runner_id,
                const std::string& revision,
                const boost::posix_time::ptime& timestamp,
                const std::string& library_name,
                const std::string& toolset_name,
                const std::string& test_name,
                const std::vector<test_structure_t::test_log_t>& test_logs) {
    //utils::log("Processing test \"" + runner_id + "/" + library_name + "/" + test_name + "/" + toolset_name + "\"");
                    
    const char* postfixes[] = {"", "_release"};
    const char* dirs[] = { "developer", "user" };

    if(test_logs.size() > 1) {
        // utils::log("  Processing variants");

        std::string variants_file_path = output_file_path(runner_id + "-" + library_name + "-" + toolset_name + "-" + test_name + "-variants");

        write_variants_file(run_date, explicit_markup, variants_file_path, test_logs, runner_id, revision, timestamp);

        BOOST_FOREACH(const std::string& release_postfix, postfixes) {
            BOOST_FOREACH(const std::string& directory, dirs) {
                std::string variants__file_path = directory + "/" + (encode_path(runner_id + "-" + library_name + "-" + toolset_name + "-" + test_name + "-variants_" + release_postfix) + ".html");
                write_variants_reference_file(variants__file_path, "../" + variants_file_path, release_postfix);
            }
        }
    }

    BOOST_FOREACH(const test_structure_t::test_log_t& test_log, test_logs) {
        //utils::log("  Processing test-log");

        if(show_output(explicit_markup, test_log)) {
            std::string log_path = log_file_path(explicit_markup, test_log, runner_id);
            write_test_result_file(run_date, explicit_markup, log_path, test_log, runner_id, revision, timestamp);
            
            BOOST_FOREACH(const std::string& release_postfix, postfixes) {
                BOOST_FOREACH(const std::string& directory, dirs) {
                    std::string reference_file_path = directory + "/" + log_file_path(explicit_markup, test_log, runner_id, release_postfix);
                    write_test_results_reference_file(reference_file_path, log_path);
                }
            }
        }
    }
}

// requires: path must be a valid file path.
// requires: variants_file_path must be the path to the variants file relative to path
void write_variants_reference_file(const std::string& path,
                                   const std::string& variants_file_path,
                                   const std::string release_postfix)
{
    //utils::log("    Writing variants reference file %s" % path);

    html_writer document(path);
    
    document << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\">\n"
                "<html>\n"
                "    <head>\n"
                "        <link rel=\"stylesheet\" type=\"text/css\" href=\"../master.css\" title=\"master\" />\n"
                "    </head>\n"
                "    <frameset cols=\"190px,*\" frameborder=\"0\" framespacing=\"0\" border=\"0\">\n"
                "        <frame name=\"tocframe\" src=\"toc" << release_postfix  << ".html\" scrolling=\"auto\"/>\n"
                "        <frame name=\"docframe\" src=\"" << escape_uri(variants_file_path) << "\" scrolling=\"auto\"/>\n"
                "    </frameset>\n"
                "</html>\n";
}

// okay. result is unconstrained
std::string output_page_header(const test_structure_t::test_log_t& test_log, const std::string& runner_id) {
    if(test_log.test_name != "") {
        return runner_id + " - " + test_log.library + " - " + test_log.test_name + " / " + test_log.toolset;
    } else {
        return test_log.target_directory;
    }
}

// requires revision is an SVN revision #
// requires path is a valid path
void write_variants_file(const boost::posix_time::ptime& run_date,
                         const failures_markup_t& explicit_markup,
                         const std::string& path,
                         const std::vector<test_structure_t::test_log_t>& test_logs,
                         const std::string& runner_id,
                         const std::string& revision,
                         const boost::posix_time::ptime& timestamp)
{
    //utils::log("    Writing variants file " + path.string());
    html_writer document(path);

    std::string component = output_page_header(test_logs[0], runner_id);
    int age = timestamp_difference(timestamp, run_date);

    document << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\">\n"
                "<html>\n"
                "    <head>\n"
                "        <link rel=\"stylesheet\" type=\"text/css\" href=\"../master.css\" title=\"master\" />\n"
                "        <title>Test output: " << escape_xml(component) << "</title>\n"
                "    </head>\n"
                "    <body>\n"
                "        <div class=\"log-test-header\">\n"
                "            <div class=\"log-test-title\">\n"
                "                Test output: " << escape_xml(component) << "\n"
                "            </div>\n"
                "            <div><span class=\"timestamp-" << age << "\">\n"
                "                Rev " << revision << " /\n"
                "                " << format_timestamp(timestamp) << "\n"
                "            </span></div>\n"
                "        </div>\n"
                "\n"
                "        <div>\n"
                "            <b>Report Time: </b> " << format_timestamp(run_date) << "\n"
                "        </div>\n"
                "\n"
                "        <p>Output by test variants:</p>\n"
                "        <table>\n";

    BOOST_FOREACH(const test_structure_t::test_log_t& log, test_logs) {

        document << "            <tr>\n"
                    "                <td>\n";

        std::string log_file = log_file_path(explicit_markup, log, runner_id);
        if(!log_file.empty()) {

            document << "                    <a href=\"../" << escape_uri(log_file) << "\">\n"
                        "                        " << escape_xml(log.target_directory) << "\n"
                        "                    </a>\n";

        } else {

            document << "                    " << escape_xml(log.target_directory) << "\n";

        }

            document << "                </td>\n"
                        "            </tr>\n";
                     
    }

    document << "        </table>\n"
                "    </body>\n"
                "</html>\n";
}

// okay
const test_structure_t::target_t* lookup_target(const test_structure_t::test_log_t& test_log, const std::string& name) {
    boost::unordered_map<std::string, test_structure_t::target_t>::const_iterator pos = test_log.targets.find(name);
    if(pos != test_log.targets.end()) {
        return &pos->second;
    } else {
        return 0;
    }
}

// requires: path is a valid path
// requires: revision is an SVN revision
void write_test_result_file(const boost::posix_time::ptime& run_date,
                            const failures_markup_t& explicit_markup,
                            const std::string& path,
                            const test_structure_t::test_log_t& test_log,
                            const std::string& runner_id,
                            const std::string& revision,
                            const boost::posix_time::ptime& timestamp)
{
    //utils::log(boost::format("    Writing log file document %s") % path);

    html_writer document(path);
    
    std::string component = output_page_header(test_log, runner_id);
    int age = timestamp_difference(timestamp, run_date);
    
    document << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\">\n"
                "<html>\n";

    document << "    <head>\n"
                "        <link rel=\"stylesheet\" type=\"text/css\" href=\"../master.css\" title=\"master\" />\n"
                "        <title>Test output: " << escape_xml(component) << "</title>\n"
                "    </head>\n";

    document << "    <body>\n"
                "        <div class=\"log-test-header\">\n"
                "            <div class=\"log-test-title\">\n"
                "                Test output: " << escape_xml(component) << "\n"
                "            </div>\n"
                "            <div><span class=\"timestamp-" << age << "\">\n"
                "                Rev " << revision << " /\n"
                "                " << format_timestamp(timestamp) << "\n"
                "            </span></div>\n"
                "        </div>\n";

    document << "        <div>\n"
                "            <b>Report Time: </b> " << format_timestamp(run_date) << "\n"
                "        </div>\n";
    
    if(!test_log.notes.empty()) {

        document << "        <p>\n"
                    "            <div class=\"notes-title\">Notes</div>\n";

        show_notes(document, test_log.notes, explicit_markup);

        document << "        </p>\n";

    }

    if(const test_structure_t::target_t* compile = lookup_target(test_log, "compile")) {
        const char* compile_result = compile->result? "succeed" : "fail";
        document << "        <p>\n";
        document << "            <div class=\"log-compiler-output-title\">Compile [" << escape_xml(compile->timestamp) << "]:"
                                 " <span class=\"output-" << compile_result << "\">" << compile_result << "</span></div>\n";
        document << "            <pre>\n";
        write_contents(document, compile->contents, true);
        document << "            </pre>\n";
        document << "        </p>\n";
    }

    if(const test_structure_t::target_t* link = lookup_target(test_log, "link")) {
        const char* link_result = link->result? "succeed" : "fail";
        document << "        <p>\n";
        document << "            <div class=\"log-linker-output-title\">Link [" << escape_xml(link->timestamp) << "]:"
                                 " <span class=\"output-" << link_result << "\">" << link_result << "</span></div>\n";
        document << "            <pre>\n";
        write_contents(document, link->contents, true);
        document << "            </pre>\n";
        document << "        </p>\n";
    }

    if(const test_structure_t::target_t* lib = lookup_target(test_log, "lib")) {
        const char* lib_result = lib->result? "succeed" : "fail";
        std::string lib_name(lib->contents->value(), lib->contents->value_size());
        document << "        <p>\n";
        document << "            <div class=\"log-linker-output-title\">Lib [" << escape_xml(lib->timestamp) << "]:"
                                 " <span class=\"output-" << lib_result << "\">" << lib_result << "</span></div>\n";
        document << "            <p>\n";
        document << "                See <a href=\"" << escape_uri(encode_path(runner_id + "-" + lib_name)) << ".html\">\n";
        document << "                " << escape_xml(lib_name) << "\n";
        document << "                </a>\n";
        document << "            </p>\n";
        document << "        </p>\n";
    }
         
    if(const test_structure_t::target_t* run = lookup_target(test_log, "run")) {
        const char* run_result = run->result? "succeed" : "fail";
        document << "        <p>\n";
        document << "            <div class=\"log-linker-output-title\">Run [" << escape_xml(run->timestamp) << "]:"
                                 " <span class=\"output-" << run_result << "\">" << run_result << "</span></div>\n";
        document << "            <pre>\n";
        write_contents(document, run->contents, true);
        document << "            </pre>\n";
        document << "        </p>\n";
    }

    document << make_tinyurl;

    document << "    </body>\n";
    document << "</html>\n";
}

// requires path is a valid path
// requires: log_file_path is the location of the log file relative to path
void write_test_results_reference_file(const std::string& path, const std::string& log_file_path)
{
    html_writer document(path);
    
    document << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\">\n"
                "<html>\n"
                "    <head>\n"
                "        <link rel=\"stylesheet\" type=\"text/css\" href=\"../master.css\" title=\"master\" />\n"
                "    </head>\n"
                "    <frameset cols=\"190px,*\" frameborder=\"0\" framespacing=\"0\" border=\"0\">\n"
                "        <frame name=\"tocframe\" src=\"../toc.html\" scrolling=\"auto\"/>\n"
                "        <frame name=\"docframe\" src=\"../../" << escape_uri(log_file_path) << "\" scrolling=\"auto\"/>\n"
                "    </frameset>\n"
                "</html>\n";
}

}

// okay
void boost::regression::links_page(
    const boost::posix_time::ptime& run_date,
    const failures_markup_t& explicit_markup,
    const test_structure_t::run_t& test_run)
{
    BOOST_FOREACH(const test_structure_t::toolset_group_t::const_reference toolset, test_run.toolsets) {
        BOOST_FOREACH(const test_structure_t::toolset_t::const_reference library, toolset.second) {
            BOOST_FOREACH(const test_structure_t::library_t::const_reference test_case, library.second) {
                ::links_page(run_date,
                             explicit_markup,
                             test_run.runner,
                             test_run.revision,
                             test_run.timestamp,
                             library.first,
                             toolset.first,
                             test_case.first,
                             test_case.second);
            }
        }
    }
    BOOST_FOREACH(const test_structure_t::toolset_group_t::const_reference toolset, test_run.non_test_case_targets) {
        BOOST_FOREACH(const test_structure_t::toolset_t::const_reference library, toolset.second) {
            BOOST_FOREACH(const test_structure_t::library_t::const_reference test_case, library.second) {
                ::links_page(run_date,
                             explicit_markup,
                             test_run.runner,
                             test_run.revision,
                             test_run.timestamp,
                             library.first,
                             toolset.first,
                             test_case.first,
                             test_case.second);
            }
        }
    }
}
