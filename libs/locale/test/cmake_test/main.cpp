//
// Copyright (c) 2022 Alexander Grund
//
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#include <boost/locale/util.hpp>

#include <boost/assert.hpp>
#include <iostream>

int main()
{
    // Simple test that including a library header and using an exported function works
    #ifndef BOOST_NO_CXX11_SMART_PTR
    std::cout << "create_utf8_converter_unique_ptr" << std::endl;
    std::unique_ptr<boost::locale::util::base_converter> cvt = boost::locale::util::create_utf8_converter_unique_ptr();
    #elif BOOST_LOCALE_USE_AUTO_PTR
    std::cout << "create_utf8_converter" << std::endl;
    std::auto_ptr<boost::locale::util::base_converter> cvt = boost::locale::util::create_utf8_converter();
    #else
    std::cout << "create_utf8_converter_new_ptr" << std::endl;
    boost::locale::hold_ptr<boost::locale::util::base_converter> cvt(boost::locale::util::create_utf8_converter_new_ptr());
    #endif

    if(cvt.get())
    {
        std::cout << "Created..." << std::endl;
        BOOST_ASSERT(cvt->is_thread_safe());
        BOOST_ASSERT(cvt->max_len() == 4);
    }else
    {
        std::cout << "Failed creation..." << std::endl;
    }

    return cvt.get() ? 0 : 1;
}
