// Copyright MetaCommunications, Inc. 2003-2004.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "runners.hpp"
#include "html_writer.hpp"
#include "common.hpp"
#include <boost/foreach.hpp>
#include <iostream>

void boost::regression::runners(const test_structure_t& tests) {
    {
        html_writer document("runners.html");
        document << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\">\n"
                    "<html>\n"
                    "  <body bgcolor=\"#FFFFFF\">\n";
        BOOST_FOREACH(test_structure_t::platform_group_t::const_reference platform, tests.platforms) {
            BOOST_FOREACH(test_structure_t::platform_t::const_reference run, platform.second) {
                document << "    <table>"
                            "<tr>"
                            "<td>"
                            "<a href=\"" << escape_uri(encode_path(run.runner)) << ".html\">" << escape_xml(run.runner) << "</a>"
                            "</td>"
                            "</tr>"
                            "</table>\n";
            }
        }
        document << "  </body>\n"
                    "</html>\n";
    }
    BOOST_FOREACH(test_structure_t::platform_group_t::const_reference platform, tests.platforms) {
        BOOST_FOREACH(test_structure_t::platform_t::const_reference run, platform.second) {

            std::cout << "Writing runner document " << encode_path(run.runner) << ".html" << std::endl;

            html_writer document(encode_path(run.runner) + ".html");

            document << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\">\n"
                        "<html>\n"
                        "  <head>\n"
                        "<title>" << escape_xml(run.runner) << "</title>\n"
                        "</head>\n"
                        "<body>\n"
                        "<h1>" << escape_xml(run.runner) << "</h1>\n"
                        "<hr></hr>"
                     << run.comment // Intentionally not escaped--contains html formatting
                     << "</body>\n"
                        "</html>\n";
        }
    }
}
