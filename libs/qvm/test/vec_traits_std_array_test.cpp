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
#   include <boost/qvm/vec_operations.hpp>
#endif

#include <boost/qvm/vec_traits_array.hpp>
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
    BOOST_QVM_STATIC_ASSERT((is_vec<std::array<int,3>>::value));
    BOOST_QVM_STATIC_ASSERT((is_vec<std::array<int const,3>>::value));
    BOOST_QVM_STATIC_ASSERT((is_vec<std::array<int,3> const>::value));
    BOOST_QVM_STATIC_ASSERT((is_vec<std::array<int const,3> const>::value));
    BOOST_QVM_STATIC_ASSERT((!is_vec<std::array<std::array<int,3>,3>>::value));
    BOOST_QVM_STATIC_ASSERT((!is_vec<std::array<std::array<std::array<int,3>,3>,3>>::value));
    BOOST_QVM_STATIC_ASSERT((vec_traits<std::array<int,3>>::dim==3));
    BOOST_QVM_STATIC_ASSERT((vec_traits<std::array<int const,3>>::dim==3));
    BOOST_QVM_STATIC_ASSERT((vec_traits<std::array<int,3> const>::dim==3));
    BOOST_QVM_STATIC_ASSERT((vec_traits<std::array<int const,3> const>::dim==3));
    same_type<vec_traits<std::array<int,3>>::scalar_type,int>();
    same_type<vec_traits<std::array<int const,3>>::scalar_type,int const>();
    same_type<vec_traits<std::array<int,3> const>::scalar_type,int>();
    same_type<vec_traits<std::array<int const,3> const>::scalar_type,int const>();
    same_type< vec<int,3>, deduce_vec<std::array<int,3>>::type >();
    same_type< vec<int const,3>, deduce_vec<std::array<int const,3>>::type >();
    same_type< vec<int,3>, deduce_vec<std::array<int,3> const>::type >();
    same_type< vec<int const,3>, deduce_vec<std::array<int const,3> const>::type >();
    std::array<int,3> arr1 = {0,1,2};
    std::array<int const,3> arr2 = {0,1,2};
    std::array<int,3> const arr3 = {0,1,2};
    std::array<int const,3> const arr4 = {0,1,2};
    BOOST_TEST((vec_traits<std::array<int,3>>::read_element<0>(arr1)==0));
    BOOST_TEST((vec_traits<std::array<int,3>>::read_element<1>(arr1)==1));
    BOOST_TEST((vec_traits<std::array<int,3>>::read_element<2>(arr1)==2));
    BOOST_TEST((vec_traits<std::array<int const,3>>::read_element<0>(arr2)==0));
    BOOST_TEST((vec_traits<std::array<int const,3>>::read_element<1>(arr2)==1));
    BOOST_TEST((vec_traits<std::array<int const,3>>::read_element<2>(arr2)==2));
    BOOST_TEST((vec_traits<std::array<int,3> const>::read_element<0>(arr3)==0));
    BOOST_TEST((vec_traits<std::array<int,3> const>::read_element<1>(arr3)==1));
    BOOST_TEST((vec_traits<std::array<int,3> const>::read_element<2>(arr3)==2));
    BOOST_TEST((vec_traits<std::array<int const,3> const>::read_element<0>(arr4)==0));
    BOOST_TEST((vec_traits<std::array<int const,3> const>::read_element<1>(arr4)==1));
    BOOST_TEST((vec_traits<std::array<int const,3> const>::read_element<2>(arr4)==2));
    BOOST_TEST((vec_traits<std::array<int,3>>::read_element_idx(0,arr1)==0));
    BOOST_TEST((vec_traits<std::array<int,3>>::read_element_idx(1,arr1)==1));
    BOOST_TEST((vec_traits<std::array<int,3>>::read_element_idx(2,arr1)==2));
    BOOST_TEST((vec_traits<std::array<int const,3>>::read_element_idx(0,arr2)==0));
    BOOST_TEST((vec_traits<std::array<int const,3>>::read_element_idx(1,arr2)==1));
    BOOST_TEST((vec_traits<std::array<int const,3>>::read_element_idx(2,arr2)==2));
    BOOST_TEST((vec_traits<std::array<int,3> const>::read_element_idx(0,arr3)==0));
    BOOST_TEST((vec_traits<std::array<int,3> const>::read_element_idx(1,arr3)==1));
    BOOST_TEST((vec_traits<std::array<int,3> const>::read_element_idx(2,arr3)==2));
    BOOST_TEST((vec_traits<std::array<int const,3> const>::read_element_idx(0,arr4)==0));
    BOOST_TEST((vec_traits<std::array<int const,3> const>::read_element_idx(1,arr4)==1));
    BOOST_TEST((vec_traits<std::array<int const,3> const>::read_element_idx(2,arr4)==2));
    BOOST_TEST((&vec_traits<std::array<int,3>>::write_element<0>(arr1)==&arr1[0]));
    BOOST_TEST((&vec_traits<std::array<int,3>>::write_element<1>(arr1)==&arr1[1]));
    BOOST_TEST((&vec_traits<std::array<int,3>>::write_element<2>(arr1)==&arr1[2]));
    BOOST_TEST((&vec_traits<std::array<int,3>>::write_element_idx(0,arr1)==&arr1[0]));
    BOOST_TEST((&vec_traits<std::array<int,3>>::write_element_idx(1,arr1)==&arr1[1]));
    BOOST_TEST((&vec_traits<std::array<int,3>>::write_element_idx(2,arr1)==&arr1[2]));
    return boost::report_errors();
    }

#endif
