// Copyright (c) 2022 Klemens D. Morgenstern
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// Disable autolinking for unit tests.
#if !defined(BOOST_ALL_NO_LIB)
#define BOOST_ALL_NO_LIB 1
#endif // !defined(BOOST_ALL_NO_LIB)

// Test that header file is self-contained.
#include <boost/process/v2/detail/utf8.hpp>

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_codecvt)
{
    struct end_t
    {
        std::size_t operator()(const char * c) 
        {
            return std::char_traits<char>::length(c);
        }
        std::size_t operator()(const wchar_t * c) 
        {
            return std::char_traits<wchar_t>::length(c);
        }
    } end{};

    
    const char * in = "test-input-\320\240\320\230\320\221\320\220";
    const wchar_t  * win_t  = L"test-input-\u0420\u0418\u0411\u0410";

    BOOST_CHECK(boost::process::v2::detail::conv_string<char>   ( in,    end( in  )) ==  in  );
    BOOST_CHECK(boost::process::v2::detail::conv_string<wchar_t>(win_t,  end(win_t)) == win_t);

    BOOST_CHECK(boost::process::v2::detail::conv_string<wchar_t>( in,    end( in  )) == win_t);
    BOOST_CHECK(boost::process::v2::detail::conv_string<char>   (win_t,  end(win_t)) ==  in  );

}