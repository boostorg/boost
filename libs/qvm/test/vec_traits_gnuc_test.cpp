// Copyright 2008-2022 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(__GNUC__) || !defined(__SSE__)

#include <iostream>

int main()
{
    std::cout << "Test not applicable due to lack of C++11 support in the compiler" << std::endl;
    return 0;
}

#else

#ifdef BOOST_QVM_TEST_SINGLE_HEADER
#   include BOOST_QVM_TEST_SINGLE_HEADER
#else
#   include <boost/qvm/vec_access.hpp>
#   include <boost/qvm/vec_operations.hpp>
#   include <boost/qvm/vec.hpp>
#   include <boost/qvm/swizzle4.hpp>
#endif

#include <boost/qvm/vec_traits_gnuc.hpp>
#include <boost/core/lightweight_test.hpp>
#include <xmmintrin.h>

template <class T,class U>
struct same_type;

template <class T>
struct
same_type<T,T>
    {
    };

int
main()
    {
    using namespace boost::qvm;
    typedef float v2sf __attribute__ ((vector_size(8)));
    typedef float v4sf __attribute__ ((vector_size(16)));
    typedef double v2sd __attribute__ ((vector_size(16)));
    typedef double v4sd __attribute__ ((vector_size(32)));
    BOOST_QVM_STATIC_ASSERT(is_vec<v2sf>::value);
    BOOST_QVM_STATIC_ASSERT(is_vec<v4sf>::value);
    BOOST_QVM_STATIC_ASSERT(is_vec<__v4sf>::value);
    BOOST_QVM_STATIC_ASSERT(is_vec<v2sd>::value);
    BOOST_QVM_STATIC_ASSERT(is_vec<v4sd>::value);
    BOOST_QVM_STATIC_ASSERT(vec_traits<v2sf>::dim==2);
    BOOST_QVM_STATIC_ASSERT(vec_traits<v4sf>::dim==4);
    BOOST_QVM_STATIC_ASSERT(vec_traits<__v4sf>::dim==4);
    BOOST_QVM_STATIC_ASSERT(vec_traits<v2sd>::dim==2);
    BOOST_QVM_STATIC_ASSERT(vec_traits<v4sd>::dim==4);
    {
        __v4sf v = { 1, 2, 3, 4 };
        BOOST_TEST_EQ(X(v), 1);
        BOOST_TEST_EQ(Y(v), 2);
        BOOST_TEST_EQ(Z(v), 3);
        BOOST_TEST_EQ(W(v), 4);
        X(v) = 5;
        BOOST_TEST_EQ(X(v), 5);
        BOOST_TEST_EQ(Y(v), 2);
        BOOST_TEST_EQ(Z(v), 3);
        BOOST_TEST_EQ(W(v), 4);
        Y(v) = 6;
        BOOST_TEST_EQ(X(v), 5);
        BOOST_TEST_EQ(Y(v), 6);
        BOOST_TEST_EQ(Z(v), 3);
        BOOST_TEST_EQ(W(v), 4);
        Z(v) = 7;
        BOOST_TEST_EQ(X(v), 5);
        BOOST_TEST_EQ(Y(v), 6);
        BOOST_TEST_EQ(Z(v), 7);
        BOOST_TEST_EQ(W(v), 4);
        W(v) = 8;
        BOOST_TEST_EQ(X(v), 5);
        BOOST_TEST_EQ(Y(v), 6);
        BOOST_TEST_EQ(Z(v), 7);
        BOOST_TEST_EQ(W(v), 8);
    }
    {
        vec<float,4> v1 = {{1, 2, 3, 4}};
        __v4sf v2 = {5, 6, 7, 8};
        __v4sf r1 = v1 + v2;
        BOOST_TEST_EQ(X(r1), 6);
        BOOST_TEST_EQ(Y(r1), 8);
        BOOST_TEST_EQ(Z(r1), 10);
        BOOST_TEST_EQ(W(r1), 12);
        v4sd r2 = v1 - v2;
        BOOST_TEST_EQ(X(r2), -4);
        BOOST_TEST_EQ(Y(r2), -4);
        BOOST_TEST_EQ(Z(r2), -4);
        BOOST_TEST_EQ(W(r2), -4);
    }
#ifndef BOOST_QVM_TEST_SINGLE_HEADER
    {
        vec<float,4> v1 = {{1, 2, 3, 4}};
        __v4sf v2 = {0, 0, 0, 0};
        BOOST_TEST_EQ(X(v2), 0);
        BOOST_TEST_EQ(Y(v2), 0);
        BOOST_TEST_EQ(Z(v2), 0);
        BOOST_TEST_EQ(W(v2), 0);
        WZYX(v2) = v1;
        BOOST_TEST_EQ(X(v2), 4);
        BOOST_TEST_EQ(Y(v2), 3);
        BOOST_TEST_EQ(Z(v2), 2);
        BOOST_TEST_EQ(W(v2), 1);
    }
#endif
    return boost::report_errors();
    }

#endif
