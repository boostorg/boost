// Copyright 2008-2022 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test_qvm_matrix.hpp"

using namespace boost::qvm;

template <class M, bool WriteElementRef=mat_write_element_ref<M>::value>
struct
test_write_element_idx
    {
    static
    void
    test()
        {
        for( int r1=0; r1!=mat_traits<M>::rows; ++r1 )
            for( int c1=0; c1!=mat_traits<M>::cols; ++c1 )
                {
                M m(0,1);
                mat_traits<M>::write_element_idx(r1,c1,m,42);
                int i = 0;
                for( int r2=0; r2!=mat_traits<M>::rows; ++r2 )
                    for( int c2=0; c2!=mat_traits<M>::cols; ++c2,++i )
                        BOOST_TEST_EQ(
                            (mat_traits<M>::read_element_idx(r2,c2,m)),
                            (r1==r2 && c1==c2 ? 42 : i) );
                }
        }
    };

template <class M>
struct
test_write_element_idx<M, true>
    {
    static
    void
    test()
        {
        M m;
        for( int r=0; r!=mat_traits<M>::rows; ++r )
            for( int c=0; c!=mat_traits<M>::cols; ++c )
                BOOST_TEST_EQ(&mat_traits<M>::write_element_idx(r,c,m), &m.a[r][c]);
        }
    };

template <class M>
void
mat_index_test()
    {
    M m(0, 1);
    int i = 0;
    for( int r=0; r!=mat_traits<M>::rows; ++r )
        for( int c=0; c!=mat_traits<M>::cols; ++c,++i )
            BOOST_TEST_EQ(mat_traits<M>::read_element_idx(r,c,m), i);
    test_write_element_idx<M>::test();
    }

int
main()
    {       
    using namespace boost::qvm;
    mat_index_test<test_qvm::matrix<M1,4,4> >();
    mat_index_test<test_qvm::matrix<M1,5,5> >();
    return boost::report_errors();
    }
