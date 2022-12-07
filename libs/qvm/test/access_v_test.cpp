// Copyright 2008-2022 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifdef BOOST_QVM_TEST_SINGLE_HEADER
#   include BOOST_QVM_TEST_SINGLE_HEADER
#else
#   include <boost/qvm/vec_access.hpp>
#endif

#include "test_qvm_vector.hpp"

using namespace boost::qvm;

template <class V>
void
check_write_ref()
    {
    V v;
    BOOST_TEST((&A<0>(v)==&v.a[0]));
    BOOST_TEST((&A<1>(v)==&v.a[1]));
    BOOST_TEST((&A<2>(v)==&v.a[2]));
    BOOST_TEST((&A<3>(v)==&v.a[3]));
    BOOST_TEST((&A<4>(v)==&v.a[4]));
    BOOST_TEST((&A<5>(v)==&v.a[5]));
    BOOST_TEST((&A<6>(v)==&v.a[6]));
    BOOST_TEST((&A<7>(v)==&v.a[7]));
    BOOST_TEST((&A<8>(v)==&v.a[8]));
    BOOST_TEST((&A<9>(v)==&v.a[9]));
    BOOST_TEST(&A0(v)==&v.a[0]);
    BOOST_TEST(&A1(v)==&v.a[1]);
    BOOST_TEST(&A2(v)==&v.a[2]);
    BOOST_TEST(&A3(v)==&v.a[3]);
    BOOST_TEST(&A4(v)==&v.a[4]);
    BOOST_TEST(&A5(v)==&v.a[5]);
    BOOST_TEST(&A6(v)==&v.a[6]);
    BOOST_TEST(&A7(v)==&v.a[7]);
    BOOST_TEST(&A8(v)==&v.a[8]);
    BOOST_TEST(&A9(v)==&v.a[9]);
    BOOST_TEST(&X(v)==&v.a[0]);
    BOOST_TEST(&Y(v)==&v.a[1]);
    BOOST_TEST(&Z(v)==&v.a[2]);
    BOOST_TEST(&W(v)==&v.a[3]);
    }

template <class V>
void
check_read( V const & v )
    {
    BOOST_TEST_EQ((A<0>(v)), 0);
    BOOST_TEST_EQ((A<1>(v)), 1);
    BOOST_TEST_EQ((A<2>(v)), 2);
    BOOST_TEST_EQ((A<3>(v)), 3);
    BOOST_TEST_EQ((A<4>(v)), 4);
    BOOST_TEST_EQ((A<5>(v)), 5);
    BOOST_TEST_EQ((A<6>(v)), 6);
    BOOST_TEST_EQ((A<7>(v)), 7);
    BOOST_TEST_EQ((A<8>(v)), 8);
    BOOST_TEST_EQ((A<9>(v)), 9);
    BOOST_TEST_EQ(A0(v), 0);
    BOOST_TEST_EQ(A1(v), 1);
    BOOST_TEST_EQ(A2(v), 2);
    BOOST_TEST_EQ(A3(v), 3);
    BOOST_TEST_EQ(A4(v), 4);
    BOOST_TEST_EQ(A5(v), 5);
    BOOST_TEST_EQ(A6(v), 6);
    BOOST_TEST_EQ(A7(v), 7);
    BOOST_TEST_EQ(A8(v), 8);
    BOOST_TEST_EQ(A9(v), 9);
    BOOST_TEST_EQ(X(v), 0);
    BOOST_TEST_EQ(Y(v), 1);
    BOOST_TEST_EQ(Z(v), 2);
    BOOST_TEST_EQ(W(v), 3);
    }

template <class V>
void
check_write()
    {
       {
        V v(99, -1);
        A<0>(v) = 0;
        A<1>(v) = 1;
        A<2>(v) = 2;
        A<3>(v) = 3;
        A<4>(v) = 4;
        A<5>(v) = 5;
        A<6>(v) = 6;
        A<7>(v) = 7;
        A<8>(v) = 8;
        A<9>(v) = 9;
        check_read(v);
        }
       {
        V v(9, -1);
        A0(v) = 0;
        A1(v) = 1;
        A2(v) = 2;
        A3(v) = 3;
        A4(v) = 4;
        A5(v) = 5;
        A6(v) = 6;
        A7(v) = 7;
        A8(v) = 8;
        A9(v) = 9;
        check_read(v);
        }
       {
        V v(9, -1);
        X(v) = 0;
        Y(v) = 1;
        Z(v) = 2;
        W(v) = 3;
        A4(v) = 4;
        A5(v) = 5;
        A6(v) = 6;
        A7(v) = 7;
        A8(v) = 8;
        A9(v) = 9;
        check_read(v);
        }
    }

int
main()
    {
    check_read(test_qvm::vector<V1,10,float>(0,1));
    check_write<test_qvm::vector<V1,10,float> >();
#ifdef BOOST_QVM_TEST_REF_WRITE_ELEMENT
    check_write_ref<test_qvm::vector<V1,10,float> >();
#endif
    return boost::report_errors();
    }
