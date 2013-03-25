// boost_report.cpp
//
// Copyright (c) 2013
// Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENCE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "issues_page.hpp"
#include "links_page.hpp"
#include "result_page.hpp"
#include "issues_page.hpp"
#include "summary_page.hpp"
#include "add_expected_results.hpp"
#include "produce_expected_results.hpp"
#include "runners.hpp"
#include "xml.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/foreach.hpp>
#include <boost/exception/exception.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>

using namespace boost::regression;

boost::shared_ptr<boost::zip::zip_archive> global_zip;

int main(int argc, char* argv[]) {
    boost::program_options::options_description desc;
    boost::program_options::variables_map vm;
    desc.add_options()
        ("input-file", boost::program_options::value<std::vector<std::string> >(), "Runner XML files")
        ("expected,e", boost::program_options::value<std::string>()->required(), "Expected results file")
        ("markup,m", boost::program_options::value<std::string>()->required(), "Failures markup file")
        ("tag", boost::program_options::value<std::string>()->required(), "the tag for the results (i.e. 'trunk')")
        ("run-date", boost::program_options::value<boost::posix_time::ptime>()->default_value(boost::posix_time::second_clock::universal_time()), "the timestamp of the report")
        ("reports,r", boost::program_options::value<std::vector<std::string> >(), "The reports to generate")
        ("css", boost::program_options::value<std::string>(), "The CSS file")
        ("help,h", "produce a help message")
        ;

    boost::program_options::positional_options_description p;
    p.add("input-file", -1);

    try {
    
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv)
            .options(desc).positional(p).run(), vm);
        boost::program_options::notify(vm);

        boost::posix_time::ptime now = vm["run-date"].as<boost::posix_time::ptime>();
        std::string tag = vm["tag"].as<std::string>();
        std::set<std::string> reports;
        if(vm.count("reports")) {
            BOOST_FOREACH(const std::string& report, vm["reports"].as<std::vector<std::string> >())
                reports.insert(report);
        }
        std::vector<std::string> warnings;
        
        test_structure_t structure;
        failures_markup_t markup;
        expected_results_t expected;
        std::vector<test_structure_t::run_t*> runs;
        
        std::cout << "Reading expected results" << std::endl;
        boost::shared_ptr<document_type> expected_results = read_xml_file(vm["expected"].as<std::string>().c_str());
        load_expected_results(&*expected_results, expected);

        std::cout << "Reading failures markup" << std::endl;
        boost::shared_ptr<document_type> failures_markup = read_xml_file(vm["markup"].as<std::string>().c_str());
        load_failures_markup(&*failures_markup, markup);

        std::ofstream zip_file("report.zip", std::ios_base::binary);
        zip_file.exceptions(std::ios_base::failbit);
        global_zip.reset(new boost::zip::zip_archive(zip_file));

        if(vm.count("input-file")) {
            BOOST_FOREACH(const std::string& file, vm["input-file"].as<std::vector<std::string> >()) {
                boost::shared_ptr<document_type> test_results;
                try {
                    std::cout << "Reading " << file << std::endl;
                    test_results = read_xml_file(file.c_str());
                    load_test_structure(&*test_results, structure, runs);
                    test_structure_t::run_t* test_run = runs.back();
                    std::cout << "Merging expected results" << std::endl;
                    add_expected_results(*test_run, markup, expected);
                    std::cout << "Generating links pages" << std::endl;
                    // must be run before test_results is discarded
                    if(reports.count("l"))
                        links_page(now, markup, *test_run);
                } catch(std::ios_base::failure& e) {
                    std::cerr << e.what() << std::endl;
                } catch(boost::property_tree::detail::rapidxml::parse_error& e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        }

        std::vector<std::string> modes;
        modes.push_back("developer");
        modes.push_back("user");

        if (reports.count("i") != 0) {
            std::cout << "Generating issues page" << std::endl;
            issues_list("developer", structure, markup,
                        true, tag, now, warnings, "");
        }

        BOOST_FOREACH(const std::string& mode, modes) {
            if(reports.count(mode.substr(0, 1) + "d"))
                result_page(structure, markup,
                            false, tag, now, warnings, mode, "comment.html");
        }

        BOOST_FOREACH(const std::string& mode, modes) {
            if(reports.count(mode.substr(0, 1) + "s"))
                summary_page(mode, tag, now, std::vector<std::string>(),
                             structure, markup, false);
        }

        BOOST_FOREACH(const std::string& mode, modes) {
            if(reports.count(mode.substr(0, 1) + "dr"))
                result_page(structure, markup,
                            true, tag, now, warnings, mode, "comment.html");
        }

        BOOST_FOREACH(const std::string& mode, modes) {
            if(reports.count(mode.substr(0, 1) + "sr"))
                summary_page(mode, tag, now, std::vector<std::string>(),
                             structure, markup, true);
        }

        if (reports.count("e")) {
            produce_expected_results(structure);
        }

        if(reports.count("n")) {
            runners(structure);
        }

        if(vm.count("css")) {
            std::cout << "Writing file master.css" << std::endl;
            html_writer css("master.css");
            std::string filename = vm["css"].as<std::string>();
            std::ifstream input(filename.c_str());
            if (input) {
                std::string data(std::istreambuf_iterator<char>(input.rdbuf()), std::istreambuf_iterator<char>());
                css << data;
            } else {
                std::cerr << "warning: Could not open file: " << filename << std::endl;
            }
        }

        global_zip.reset();

    } catch(boost::program_options::error& e) {
        if(vm.count("help")) {
            std::cerr << desc << std::endl;
        } else {
            std::cerr << e.what() << std::endl;
            return EXIT_FAILURE;
        }
    } catch(boost::exception& e) {
        std::cerr << boost::diagnostic_information(e) << std::endl;
        return EXIT_FAILURE;
    } catch(std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
