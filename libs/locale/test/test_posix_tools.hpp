//
// Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_LOCLAE_TEST_LOCALE_POSIX_TOOLS_HPP
#define BOOST_LOCLAE_TEST_LOCALE_POSIX_TOOLS_HPP

#include <clocale>
#include <string>

#if defined(__APPLE__) || defined(__FreeBSD__)
#include <xlocale.h>
#endif

inline bool have_locale(std::string const &name)
{
    locale_t l=newlocale(LC_ALL_MASK,name.c_str(),0);
    if(l) {
        freelocale(l);
        return true;
    }
    return false;
}

class locale_holder
{
    locale_t l_;
    locale_holder(const locale_holder&);
    locale_holder& operator=(const locale_holder&);
    void reset(const locale_t l = 0)
    {
        if(l_)
            freelocale(l_);
        l_ = l;
    }
public:
    explicit locale_holder(locale_t l = 0): l_(l) {}
    ~locale_holder() { reset(); }
    operator locale_t() const { return l_; }
    locale_holder& operator=(locale_t l) { reset(l); return *this; }
};

#endif

