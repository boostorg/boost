//
// Test for boost/iterator.hpp
//
// Copyright 2014 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#define BOOST_ALLOW_DEPRECATED_HEADERS
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

#include <boost/config.hpp>

// std::iterator template is deprecated in C++17. Some standard libraries emit warnings
// that cannot be easily suppressed, so disable the tests in C++17 onwards.
#if BOOST_CXX_VERSION < 201703

#include <boost/iterator.hpp>
#include <boost/core/is_same.hpp>
#include <boost/core/lightweight_test_trait.hpp>

/*

template< class Category, class T,
  class Distance = ptrdiff_t,
  class Pointer = T*,
  class Reference = T&>
struct iterator
{
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
    typedef Category iterator_category;
};

*/

struct C
{
};

struct T
{
};

struct D
{
};

struct P
{
};

struct R
{
};

int main()
{
    using boost::core::is_same;

    BOOST_TEST_TRAIT_TRUE((is_same<boost::iterator<C,T,D,P,R>::iterator_category,C>));
    BOOST_TEST_TRAIT_TRUE((is_same<boost::iterator<C,T,D,P,R>::value_type,T>));
    BOOST_TEST_TRAIT_TRUE((is_same<boost::iterator<C,T,D,P,R>::difference_type,D>));
    BOOST_TEST_TRAIT_TRUE((is_same<boost::iterator<C,T,D,P,R>::pointer,P>));
    BOOST_TEST_TRAIT_TRUE((is_same<boost::iterator<C,T,D,P,R>::reference,R>));

    BOOST_TEST_TRAIT_TRUE((is_same<boost::iterator<C,T>::iterator_category,C>));
    BOOST_TEST_TRAIT_TRUE((is_same<boost::iterator<C,T>::value_type,T>));
    BOOST_TEST_TRAIT_TRUE((is_same<boost::iterator<C,T>::difference_type,std::ptrdiff_t>));
    BOOST_TEST_TRAIT_TRUE((is_same<boost::iterator<C,T>::pointer,T*>));
    BOOST_TEST_TRAIT_TRUE((is_same<boost::iterator<C,T>::reference,T&>));

    return boost::report_errors();
}

#else // BOOST_CXX_VERSION < 201703

int main()
{
}

#endif // BOOST_CXX_VERSION < 201703
