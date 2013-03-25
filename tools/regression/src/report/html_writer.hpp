// html_writer.hpp
//
// Copyright (c) 2010 Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanyiong file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HTML_WRITER_HPP_INCLUDED
#define HTML_WRITER_HPP_INCLUDED

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include "zip.hpp"

#include <cstring>

extern boost::shared_ptr<boost::zip::zip_archive> global_zip;

namespace boost {
namespace regression {

class html_writer : boost::noncopyable {
public:
    // path must be UTF-8 encoded.  The separator is '/'
    explicit html_writer(const std::string& path)
      : sink(*global_zip, path)
    {}
    ~html_writer() {
    }
    html_writer& operator<<(const std::string& arg) {
        sink.write(arg.data(), arg.size());
        return *this;
    }
    html_writer& operator<<(const char* arg) {
        sink.write(arg, ::std::strlen(arg));
        return *this;
    }
    html_writer& operator<<(char arg) {
        sink.write(&arg, 1);
        return *this;
    }
    html_writer& operator<<(std::size_t arg) {
        char buf[30];
        char* pos = &buf[0] + 30;
        if(arg == 0) {
            *--pos = '0';
        }
        for(; arg > 0; arg /= 10) {
            *--pos = static_cast<char>('0' + (arg % 10));
        }
        sink.write(pos, buf + 30 - pos);
        return *this;
    }
    html_writer& operator<<(int arg) {
        if(arg < 0) *this << '-' << std::size_t(-arg);
        else *this << std::size_t(arg);
        return *this;
    }
private:
    boost::zip::deflate_sink sink;
};

}
}

#endif
