// Copyright (c) 2022Klemens Morgernstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//[intro
#include <boost/process/v2.hpp>

#include <boost/asio/read.hpp>
#include <boost/asio/readable_pipe.hpp>
#include <boost/system/error_code.hpp>

#include <string>
#include <iostream>

namespace proc   = boost::process::v2;
namespace asio   = boost::asio;


int main()
{
    asio::io_context ctx;
    asio::readable_pipe p{ctx};

    const auto exe = proc::environment::find_executable("gcc");

    proc::process c{ctx, exe, {"--version"}, proc::process_stdio{nullptr, p}};

    std::string line;
    boost::system::error_code ec;

    auto sz = asio::read(p, asio::dynamic_buffer(line), ec);
    assert(ec == asio::error::eof);

    std::cout << "Gcc version: '"  << line << "'" << std::endl;

    c.wait();
}
//]
