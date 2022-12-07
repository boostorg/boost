// Copyright 2008-2022 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test_qvm_vector.hpp"

using namespace boost::qvm;

template <class V, bool WriteElementRef=vec_write_element_ref<V>::value>
struct
test_write_element_idx
    {
    static
    void
    test()
        {
        for( int i=0; i!=vec_traits<V>::dim; ++i )
            {
            V v(0,1);
            vec_traits<V>::write_element_idx(i,v,42);
            for( int j=0; j!=vec_traits<V>::dim; ++j )
                BOOST_TEST_EQ(
                    (vec_traits<V>::read_element_idx(j,v)),
                    (i==j ? 42 : j) );
            }
        }
    };

template <class V>
struct
test_write_element_idx<V, true>
    {
    static
    void
    test()
        {
        V v;
        for( int i=0; i!=vec_traits<V>::dim; ++v )
            BOOST_TEST_EQ(&vec_traits<V>::write_element_idx(i,v), &v.a[i]);
        }
    };

template <class V>
void
vec_index_test()
    {
    V v(0, 1);
    for( int i=0; i!=vec_traits<V>::dim; ++i )
        BOOST_TEST_EQ(vec_traits<V>::read_element_idx(i,v), i);
    test_write_element_idx<V>::test();
    }

int
main()
    {       
    using namespace boost::qvm;
    vec_index_test<test_qvm::vector<V1,4> >();
    vec_index_test<test_qvm::vector<V1,5> >();
    return boost::report_errors();
    }
