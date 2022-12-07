// Copyright 2008-2022 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if __cplusplus <= 199711L

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
#   include <boost/qvm/mat_operations.hpp>
#endif

#include <boost/qvm/mat_traits_array.hpp>
#include <boost/core/lightweight_test.hpp>

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
    BOOST_QVM_STATIC_ASSERT((!is_mat<std::array<int,3>>::value));
    BOOST_QVM_STATIC_ASSERT((!is_mat<std::array<std::array<std::array<int,3>,3>,3>>::value));
    BOOST_QVM_STATIC_ASSERT((is_mat<std::array<std::array<int,3>,4>>::value));
    BOOST_QVM_STATIC_ASSERT((is_mat<std::array<std::array<int const,3>,4>>::value));
    BOOST_QVM_STATIC_ASSERT((is_mat<std::array<std::array<int,3>,4> const>::value));
    BOOST_QVM_STATIC_ASSERT((is_mat<std::array<std::array<int const,3>,4> const>::value));
    BOOST_QVM_STATIC_ASSERT((mat_traits<std::array<std::array<int,3>,4>>::rows==3));
    BOOST_QVM_STATIC_ASSERT((mat_traits<std::array<std::array<int,3>,4>>::cols==4));
    same_type<mat_traits<std::array<std::array<int,3>,4>>::scalar_type,int>();
    same_type< mat<int,3,3>, deduce_mat<std::array<std::array<int,3>,3>>::type >();
    same_type< mat<int const,3,3>, deduce_mat<std::array<std::array<int const,3>,3>>::type >();
    same_type< mat<int,3,3>, deduce_mat<std::array<std::array<int,3>,3> const>::type >();
    same_type< mat<int const,3,3>, deduce_mat<std::array<std::array<int const,3>,3> const>::type >();
    std::array<std::array<int,3>,3> arr1 = {std::array<int,3>{00,01,02},std::array<int,3>{10,11,12},std::array<int,3>{20,21,22}};
    std::array<std::array<int const,3>,3> arr2 = {std::array<int const,3>{00,01,02},std::array<int const,3>{10,11,12},std::array<int const,3>{20,21,22}};
    std::array<std::array<int,3>,3> const arr3 = {std::array<int,3>{00,01,02},std::array<int,3>{10,11,12},std::array<int,3>{20,21,22}};
    std::array<std::array<int const,3>,3> const arr4 = {std::array<int const,3>{00,01,02},std::array<int const,3>{10,11,12},std::array<int const,3>{20,21,22}};
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element<0,0>(arr1)==00));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element<0,1>(arr1)==01));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element<0,2>(arr1)==02));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element<1,0>(arr1)==10));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element<1,1>(arr1)==11));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element<1,2>(arr1)==12));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element<2,0>(arr1)==20));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element<2,1>(arr1)==21));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element<2,2>(arr1)==22));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element<0,0>(arr2)==00));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element<0,1>(arr2)==01));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element<0,2>(arr2)==02));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element<1,0>(arr2)==10));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element<1,1>(arr2)==11));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element<1,2>(arr2)==12));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element<2,0>(arr2)==20));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element<2,1>(arr2)==21));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element<2,2>(arr2)==22));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element<0,0>(arr3)==00));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element<0,1>(arr3)==01));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element<0,2>(arr3)==02));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element<1,0>(arr3)==10));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element<1,1>(arr3)==11));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element<1,2>(arr3)==12));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element<2,0>(arr3)==20));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element<2,1>(arr3)==21));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element<2,2>(arr3)==22));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element<0,0>(arr4)==00));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element<0,1>(arr4)==01));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element<0,2>(arr4)==02));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element<1,0>(arr4)==10));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element<1,1>(arr4)==11));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element<1,2>(arr4)==12));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element<2,0>(arr4)==20));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element<2,1>(arr4)==21));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element<2,2>(arr4)==22));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element_idx(0,0,arr1)==00));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element_idx(0,1,arr1)==01));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element_idx(0,2,arr1)==02));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element_idx(1,0,arr1)==10));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element_idx(1,1,arr1)==11));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element_idx(1,2,arr1)==12));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element_idx(2,0,arr1)==20));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element_idx(2,1,arr1)==21));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3>>::read_element_idx(2,2,arr1)==22));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element_idx(0,0,arr2)==00));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element_idx(0,1,arr2)==01));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element_idx(0,2,arr2)==02));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element_idx(1,0,arr2)==10));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element_idx(1,1,arr2)==11));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element_idx(1,2,arr2)==12));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element_idx(2,0,arr2)==20));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element_idx(2,1,arr2)==21));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3>>::read_element_idx(2,2,arr2)==22));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element_idx(0,0,arr3)==00));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element_idx(0,1,arr3)==01));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element_idx(0,2,arr3)==02));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element_idx(1,0,arr3)==10));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element_idx(1,1,arr3)==11));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element_idx(1,2,arr3)==12));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element_idx(2,0,arr3)==20));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element_idx(2,1,arr3)==21));
    BOOST_TEST((mat_traits<std::array<std::array<int,3>,3> const>::read_element_idx(2,2,arr3)==22));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element_idx(0,0,arr4)==00));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element_idx(0,1,arr4)==01));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element_idx(0,2,arr4)==02));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element_idx(1,0,arr4)==10));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element_idx(1,1,arr4)==11));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element_idx(1,2,arr4)==12));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element_idx(2,0,arr4)==20));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element_idx(2,1,arr4)==21));
    BOOST_TEST((mat_traits<std::array<std::array<int const,3>,3> const>::read_element_idx(2,2,arr4)==22));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element<0,0>(arr1)==&arr1[0][0]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element<0,1>(arr1)==&arr1[0][1]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element<0,2>(arr1)==&arr1[0][2]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element<1,0>(arr1)==&arr1[1][0]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element<1,1>(arr1)==&arr1[1][1]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element<1,2>(arr1)==&arr1[1][2]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element<2,0>(arr1)==&arr1[2][0]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element<2,1>(arr1)==&arr1[2][1]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element<2,2>(arr1)==&arr1[2][2]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element_idx(0,0,arr1)==&arr1[0][0]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element_idx(0,1,arr1)==&arr1[0][1]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element_idx(0,2,arr1)==&arr1[0][2]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element_idx(1,0,arr1)==&arr1[1][0]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element_idx(1,1,arr1)==&arr1[1][1]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element_idx(1,2,arr1)==&arr1[1][2]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element_idx(2,0,arr1)==&arr1[2][0]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element_idx(2,1,arr1)==&arr1[2][1]));
    BOOST_TEST((&mat_traits<std::array<std::array<int,3>,3>>::write_element_idx(2,2,arr1)==&arr1[2][2]));
    return boost::report_errors();
    }

#endif
