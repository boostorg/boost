/////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Olaf Krzikalla 2004-2006.
// (C) Copyright Ion Gaztanaga  2006-2013.
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/intrusive for documentation.
//
/////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_INTRUSIVE_DETAIL_ITESTVALUE_HPP
#define BOOST_INTRUSIVE_DETAIL_ITESTVALUE_HPP

#include <iostream>
#include <boost/intrusive/options.hpp>
#include <boost/container_hash/hash.hpp>
#include <boost/intrusive/pointer_traits.hpp>
#include "nonhook_node.hpp"
#include "int_holder.hpp"
#include <boost/intrusive/detail/get_value_traits.hpp>
#include <boost/container/vector.hpp>

struct testvalue_filler
{
   void *dummy_[3];
};

template<class Hooks>
struct testvalue
   :  testvalue_filler
   ,  Hooks::base_hook_type
   ,  Hooks::auto_base_hook_type
{
   typename Hooks::member_hook_type        node_;
   typename Hooks::auto_member_hook_type   auto_node_;
   typename Hooks::nonhook_node_member_type nhn_member_;

   int_holder value_;

   testvalue()
   {}

   explicit testvalue(int i)
      :  value_(i)
   {}

   testvalue (const testvalue& src)
      :  value_(src.value_)
   {}

   // testvalue is used in boost::container::vector and thus prev and next
   // have to be handled appropriately when copied:
   testvalue & operator= (const testvalue& src)
   {
      Hooks::base_hook_type::operator=(static_cast<const typename Hooks::base_hook_type&>(src));
      Hooks::auto_base_hook_type::operator=(static_cast<const typename Hooks::auto_base_hook_type&>(src));
      this->node_       = src.node_;
      this->auto_node_  = src.auto_node_;
      this->nhn_member_ = src.nhn_member_;
      value_ = src.value_;
      return *this;
   }

   void swap_nodes(testvalue &other)
   {
      Hooks::base_hook_type::swap_nodes(static_cast<typename Hooks::base_hook_type&>(other));
      Hooks::auto_base_hook_type::swap_nodes(static_cast<typename Hooks::auto_base_hook_type&>(other));
      node_.swap_nodes(other.node_);
      auto_node_.swap_nodes(other.auto_node_);
      nhn_member_.swap_nodes(other.nhn_member_);
   }

   bool is_linked() const
   {
      return Hooks::base_hook_type::is_linked() ||
      Hooks::auto_base_hook_type::is_linked() ||
      node_.is_linked() ||
      auto_node_.is_linked() ||
      nhn_member_.is_linked();
   }

   const int_holder &get_int_holder() const
   {  return value_; }

   int int_value() const
   {  return value_.int_value();  }

   ~testvalue()
   {}

   bool operator< (const testvalue &other) const
   {  return value_ < other.value_;  }

   bool operator> (const testvalue &other) const
   {  return value_ > other.value_;  }

   bool operator==(const testvalue &other) const
   {  return value_ == other.value_;  }

   bool operator!=(const testvalue &other) const
   {  return value_ != other.value_;  }

   friend bool operator< (int other1, const testvalue &other2)
   {  return other1 < other2.value_.int_;  }

   friend bool operator> (int other1, const testvalue &other2)
   {  return other1 > other2.value_.int_;  }

   friend bool operator< (const testvalue &other1, int other2)
   {  return other1.value_.int_ < other2;  }

   friend bool operator> (const testvalue &other1, int other2)
   {  return other1.value_.int_ > other2;  }

   friend bool operator== (int other1, const testvalue &other2)
   {  return other1 == other2.value_.int_;  }

   friend bool operator== (const testvalue &other1, int other2)
   {  return other1.value_.int_ == other2;  }

   friend bool operator!= (int other1, const testvalue &other2)
   {  return other1 != other2.value_.int_;  }

   friend bool operator!= (const testvalue &other1, int other2)
   {  return other1.value_.int_ != other2;  }

   friend std::size_t hash_value(const testvalue&t)
   {
      boost::hash<int> hasher;
      return hasher((&t)->int_value());
   }
   /*
   static std::size_t priority_hash(const testvalue &t)
   {  return boost::hash<int>()((&t)->int_value()); }

   static std::size_t priority_hash(int i)
   {  return boost::hash<int>()(i); }

   template <class T, class U>
   static bool priority_order_impl(const T& t1, const U& t2)
   {
      std::size_t hash1 = (priority_hash)(t1);
      boost::hash_combine(hash1, -hash1);
      std::size_t hash2 = (priority_hash)(t2);
      boost::hash_combine(hash2, -hash2);
      return hash1 < hash2;
   }

   friend bool priority_order(const testvalue &t1, int t2)
   {  return (priority_order_impl)(t1, t2);  }

   friend bool priority_order(int t1, const testvalue &t2)
   {  return (priority_order_impl)(t1, t2);  }
*/
   template < typename Node_Algorithms >
   friend void swap_nodes(testvalue& lhs, testvalue& rhs)
   {  lhs.swap_nodes(rhs);  }

   friend std::ostream& operator<<
      (std::ostream& s, const testvalue& t)
   {  return s << t.value_.int_value();   }
};

/*
bool priority_order(int t1, int t2)
{
   std::size_t hash1 = boost::hash<int>()(t1);
   boost::hash_combine(hash1, &t1);
   std::size_t hash2 = boost::hash<int>()(t2);
   boost::hash_combine(hash2, &t2);
   return hash1 < hash2;
}
*/

struct even_odd
{
   template < typename key_type_1, typename key_type_2 >
   bool operator()
      (const key_type_1& v1
      ,const key_type_2& v2) const
   {
      if (((&v1)->int_value() & 1) == ((&v2)->int_value() & 1))
         return (&v1)->int_value() < (&v2)->int_value();
      else
         return (&v2)->int_value() & 1;
   }
};

struct is_even
{
   template <typename key_type>
   bool operator()
      (const key_type& v1) const
   {  return ((&v1)->int_value() & 1) == 0;  }
};

struct is_odd
{
   template <typename key_type>
   bool operator()
      (const key_type& v1) const
   {  return ((&v1)->int_value() & 1) != 0;  }
};

template <typename>
struct ValueContainer;

template < class Hooks>
struct ValueContainer< testvalue<Hooks> >
{
    typedef boost::container::vector< testvalue<Hooks> > type;
};

template < typename Pointer >
class heap_node_holder
{
   typedef boost::intrusive::pointer_traits<Pointer> ptrtraits_t;
   typedef typename ptrtraits_t::element_type element_type;
   typedef Pointer pointer;
   typedef typename boost::intrusive::pointer_rebind<pointer, const element_type>::type const_pointer;

   public:
   heap_node_holder()
      : m_ptr(ptrtraits_t::pointer_to(*new element_type))
   {}

   ~heap_node_holder()
   { delete &*m_ptr;   }

   const_pointer get_node() const
   { return m_ptr; }

   pointer get_node()
   { return m_ptr; }

   private:
   pointer m_ptr;
};

template<class Hooks>
struct testvalue_traits
   : public Hooks
{
   typedef testvalue< Hooks > value_type;

   //base
   typedef typename boost::intrusive::detail::get_base_value_traits
         < value_type
         , typename Hooks::base_hook_type
         >::type base_value_traits;
   //auto-base
   typedef typename boost::intrusive::detail::get_base_value_traits
         < value_type
         , typename Hooks::auto_base_hook_type
         >::type auto_base_value_traits;
   //member
   typedef typename boost::intrusive::detail::get_member_value_traits
         < boost::intrusive::member_hook
            < value_type
            , typename Hooks::member_hook_type
            , &value_type::node_
            >
         >::type member_value_traits;
   //auto-member
   typedef typename boost::intrusive::detail::get_member_value_traits
         < boost::intrusive::member_hook
            < value_type
            , typename Hooks::auto_member_hook_type
            , &value_type::auto_node_
            >
         >::type auto_member_value_traits;
   //nonmember
   typedef boost::intrusive::nonhook_node_member_value_traits
         < value_type
         , typename Hooks::nonhook_node_member_type
         , &value_type::nhn_member_
         , boost::intrusive::safe_link
         > nonhook_value_traits;
};

#endif
