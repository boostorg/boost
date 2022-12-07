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
#   include <boost/qvm/quat_operations.hpp>
#endif

#include <boost/qvm/quat_traits_array.hpp>
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
    BOOST_QVM_STATIC_ASSERT((is_quat<std::array<int,4>>::value));
    BOOST_QVM_STATIC_ASSERT((is_quat<std::array<int const,4>>::value));
    BOOST_QVM_STATIC_ASSERT((is_quat<std::array<int,4> const>::value));
    BOOST_QVM_STATIC_ASSERT((is_quat<std::array<int const,4> const>::value));
    BOOST_QVM_STATIC_ASSERT((!is_quat<std::array<std::array<int,4>,4>>::value));
    BOOST_QVM_STATIC_ASSERT((!is_quat<std::array<std::array<std::array<int,4>,4>,4>>::value));
    same_type<quat_traits<std::array<int,4>>::scalar_type,int>();
    same_type< quat<int>, deduce_quat<std::array<int,4>>::type >();
    std::array<int,4> arr1 = {0,1,2,3};
    std::array<int const,4> arr2 = {0,1,2,3};
    std::array<int,4> const arr3 = {0,1,2,3};
    std::array<int const,4> const arr4 = {0,1,2,3};
    BOOST_TEST((quat_traits<std::array<int,4>>::read_element<0>(arr1)==0));
    BOOST_TEST((quat_traits<std::array<int,4>>::read_element<1>(arr1)==1));
    BOOST_TEST((quat_traits<std::array<int,4>>::read_element<2>(arr1)==2));
    BOOST_TEST((quat_traits<std::array<int,4>>::read_element<3>(arr1)==3));
    BOOST_TEST((quat_traits<std::array<int const,4>>::read_element<0>(arr2)==0));
    BOOST_TEST((quat_traits<std::array<int const,4>>::read_element<1>(arr2)==1));
    BOOST_TEST((quat_traits<std::array<int const,4>>::read_element<2>(arr2)==2));
    BOOST_TEST((quat_traits<std::array<int const,4>>::read_element<3>(arr2)==3));
    BOOST_TEST((quat_traits<std::array<int,4> const>::read_element<0>(arr3)==0));
    BOOST_TEST((quat_traits<std::array<int,4> const>::read_element<1>(arr3)==1));
    BOOST_TEST((quat_traits<std::array<int,4> const>::read_element<2>(arr3)==2));
    BOOST_TEST((quat_traits<std::array<int,4> const>::read_element<3>(arr3)==3));
    BOOST_TEST((quat_traits<std::array<int const,4> const>::read_element<0>(arr4)==0));
    BOOST_TEST((quat_traits<std::array<int const,4> const>::read_element<1>(arr4)==1));
    BOOST_TEST((quat_traits<std::array<int const,4> const>::read_element<2>(arr4)==2));
    BOOST_TEST((quat_traits<std::array<int const,4> const>::read_element<3>(arr4)==3));
    BOOST_TEST((quat_traits<std::array<int,4>>::read_element_idx(0,arr1)==0));
    BOOST_TEST((quat_traits<std::array<int,4>>::read_element_idx(1,arr1)==1));
    BOOST_TEST((quat_traits<std::array<int,4>>::read_element_idx(2,arr1)==2));
    BOOST_TEST((quat_traits<std::array<int,4>>::read_element_idx(3,arr1)==3));
    BOOST_TEST((quat_traits<std::array<int const,4>>::read_element_idx(0,arr2)==0));
    BOOST_TEST((quat_traits<std::array<int const,4>>::read_element_idx(1,arr2)==1));
    BOOST_TEST((quat_traits<std::array<int const,4>>::read_element_idx(2,arr2)==2));
    BOOST_TEST((quat_traits<std::array<int const,4>>::read_element_idx(3,arr2)==3));
    BOOST_TEST((quat_traits<std::array<int,4> const>::read_element_idx(0,arr3)==0));
    BOOST_TEST((quat_traits<std::array<int,4> const>::read_element_idx(1,arr3)==1));
    BOOST_TEST((quat_traits<std::array<int,4> const>::read_element_idx(2,arr3)==2));
    BOOST_TEST((quat_traits<std::array<int,4> const>::read_element_idx(3,arr3)==3));
    BOOST_TEST((quat_traits<std::array<int const,4> const>::read_element_idx(0,arr4)==0));
    BOOST_TEST((quat_traits<std::array<int const,4> const>::read_element_idx(1,arr4)==1));
    BOOST_TEST((quat_traits<std::array<int const,4> const>::read_element_idx(2,arr4)==2));
    BOOST_TEST((quat_traits<std::array<int const,4> const>::read_element_idx(3,arr4)==3));
    BOOST_TEST((&quat_traits<std::array<int,4>>::write_element<0>(arr1)==&arr1[0]));
    BOOST_TEST((&quat_traits<std::array<int,4>>::write_element<1>(arr1)==&arr1[1]));
    BOOST_TEST((&quat_traits<std::array<int,4>>::write_element<2>(arr1)==&arr1[2]));
    BOOST_TEST((&quat_traits<std::array<int,4>>::write_element<3>(arr1)==&arr1[3]));
    BOOST_TEST((&quat_traits<std::array<int,4>>::write_element_idx(0,arr1)==&arr1[0]));
    BOOST_TEST((&quat_traits<std::array<int,4>>::write_element_idx(1,arr1)==&arr1[1]));
    BOOST_TEST((&quat_traits<std::array<int,4>>::write_element_idx(2,arr1)==&arr1[2]));
    BOOST_TEST((&quat_traits<std::array<int,4>>::write_element_idx(3,arr1)==&arr1[3]));
    return boost::report_errors();
    }

#endif
