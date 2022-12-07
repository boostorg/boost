// Copyright 2022 Christian Mazakas.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// clang-format off
#include "../helpers/prefix.hpp"
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>
#include "../helpers/postfix.hpp"
// clang-format on

#include "../helpers/test.hpp"
#include "../objects/test.hpp"

#include <memory>

struct hash1
{
  template <class Key> std::size_t operator()(Key const&) const { return 1337; }
};

struct hash2
{
  template <class Key> std::size_t operator()(Key const&) const { return 7331; }
};

struct equal1
{
  template <class T> bool operator==(T const&) const { return true; }
};

struct equal2
{
  template <class T> bool operator==(T const&) const { return true; }
};

///////////////////////////////////////////////////////////////////////////////
// we define two duplicated allocators here, each one having the same smart
// pointer type
// we want to prove in our test that different allocators with the same pointers
// (either fancy or raw) work equally well for our SCARY iterators
//
template <class T> struct allocator1
{
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
  template <class> friend struct allocator1;
#endif

public:
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef test::void_ptr void_pointer;
  typedef test::void_const_ptr const_void_pointer;
  typedef test::ptr<T> pointer;
  typedef test::const_ptr<T> const_pointer;
  typedef T& reference;
  typedef T const& const_reference;
  typedef T value_type;

  template <class U> struct rebind
  {
    typedef allocator1<U> other;
  };

  allocator1() {}

  template <class Y> allocator1(allocator1<Y> const&) {}

  allocator1(allocator1 const&) {}

  ~allocator1() {}

  pointer address(reference r) { return pointer(&r); }

  const_pointer address(const_reference r) { return const_pointer(&r); }

  pointer allocate(size_type n)
  {
    pointer p(static_cast<T*>(::operator new(n * sizeof(T))));
    return p;
  }

  pointer allocate(size_type n, void const*)
  {
    pointer ptr(static_cast<T*>(::operator new(n * sizeof(T))));
    return ptr;
  }

  void deallocate(pointer p, size_type)
  {
    ::operator delete((void*)p.operator->());
  }

#if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
  template <class U, class V> void construct(U* p, V const& v) { new (p) U(v); }
#else
  template <class U, class... Args>
  void construct(U* p, BOOST_FWD_REF(Args)... args)
  {
    new (p) U(boost::forward<Args>(args)...);
  }
#endif

  // msvc-12.0 and msvc-14.0 seem to eliminate the destructor call as we're only
  // ever using it with an int with has a trivial destructor so it eliminates
  // the code and generates a warning about an unused variable
  //
  template <class U>
  void destroy(U* p)
  {
    (void)p;
    p->~U();
  }

  size_type max_size() const { return (std::numeric_limits<size_type>::max)(); }

  bool operator==(allocator1 const&) const { return true; }
  bool operator!=(allocator1 const&) const { return false; }

  enum
  {
    is_select_on_copy = false,
    is_propagate_on_swap = false,
    is_propagate_on_assign = false,
    is_propagate_on_move = false
  };
};

template <class T> struct allocator2
{
#ifdef BOOST_NO_MEMBER_TEMPLATE_FRIENDS
public:
#else
  template <class> friend struct allocator2;
#endif

public:
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;
  typedef test::void_ptr void_pointer;
  typedef test::void_const_ptr const_void_pointer;
  typedef test::ptr<T> pointer;
  typedef test::const_ptr<T> const_pointer;
  typedef T& reference;
  typedef T const& const_reference;
  typedef T value_type;

  template <class U> struct rebind
  {
    typedef allocator2<U> other;
  };

  allocator2() {}

  template <class Y> allocator2(allocator2<Y> const&) {}

  allocator2(allocator2 const&) {}

  ~allocator2() {}

  pointer address(reference r) { return pointer(&r); }

  const_pointer address(const_reference r) { return const_pointer(&r); }

  pointer allocate(size_type n)
  {
    pointer p(static_cast<T*>(::operator new(n * sizeof(T))));
    return p;
  }

  pointer allocate(size_type n, void const*)
  {
    pointer ptr(static_cast<T*>(::operator new(n * sizeof(T))));
    return ptr;
  }

  void deallocate(pointer p, size_type) { ::operator delete((void*)p.ptr_); }

#if defined(BOOST_NO_CXX11_VARIADIC_TEMPLATES)
  template <class U, class V> void construct(U* p, V const& v) { new (p) U(v); }
#else
  template <class U, class... Args>
  void construct(U* p, BOOST_FWD_REF(Args)... args)
  {
    new (p) U(boost::forward<Args>(args)...);
  }
#endif

  // msvc-12.0 and msvc-14.0 seem to eliminate the destructor call as we're only
  // ever using it with an int with has a trivial destructor so it eliminates
  // the code and generates a warning about an unused variable
  //
  template <class U>
  void destroy(U* p)
  {
    (void)p;
    p->~U();
  }

  size_type max_size() const { return (std::numeric_limits<size_type>::max)(); }

  bool operator==(allocator2 const&) const { return true; }
  bool operator!=(allocator2 const&) const { return false; }

  enum
  {
    is_select_on_copy = false,
    is_propagate_on_swap = false,
    is_propagate_on_assign = false,
    is_propagate_on_move = false
  };
};

template <class C1, class C2> void scary_test()
{
  C1 x;
  C2 y;

  typename C2::iterator begin(x.begin());
  BOOST_TEST(begin == x.end());

  typename C2::const_iterator cbegin(x.cbegin());
  BOOST_TEST(cbegin == x.cend());

  BOOST_ASSERT(x.bucket_count() > 0);

  typename C2::local_iterator lbegin(x.begin(0));
  BOOST_TEST(lbegin == x.end(0));

  typename C2::const_local_iterator clbegin(x.cbegin(0));
  BOOST_TEST(clbegin == x.cend(0));
}

template <
  template <class Key, class T, class Hash, class KeyEqual, class Allocator>
  class Map>
void map_scary_test()
{
  typedef std::pair<int const, int> value_type;
  typedef std::allocator<value_type> std_allocator_type;

  typedef Map<int, int, hash1, std::equal_to<int>, std_allocator_type>
    hash1_unordered_map;
  typedef Map<int, int, hash2, std::equal_to<int>, std_allocator_type>
    hash2_unordered_map;

  typedef Map<int, int, boost::hash<int>, equal1, std_allocator_type>
    equal1_unordered_map;
  typedef Map<int, int, boost::hash<int>, equal2, std_allocator_type>
    equal2_unordered_map;

  // test allocators with a raw pointer as their `::pointer` type
  //
  typedef Map<int, int, boost::hash<int>, std::equal_to<int>,
    test::allocator1<value_type> >
    alloc1_unordered_map;
  typedef Map<int, int, boost::hash<int>, std::equal_to<int>,
    std_allocator_type>
    std_alloc_unordered_map;

  // test allocators with a fancy pointer as their `::pointer` type
  //
  typedef Map<int, int, boost::hash<int>, std::equal_to<int>,
    allocator1<value_type> >
    fancy1_unordered_map;
  typedef Map<int, int, boost::hash<int>, std::equal_to<int>,
    allocator2<value_type> >
    fancy2_unordered_map;

  scary_test<alloc1_unordered_map, std_alloc_unordered_map>();
  scary_test<hash1_unordered_map, hash2_unordered_map>();
  scary_test<equal1_unordered_map, equal2_unordered_map>();
  scary_test<fancy1_unordered_map, fancy2_unordered_map>();
}

template <template <class Key, class Hash, class KeyEqual, class Allocator>
  class Set>
void set_scary_test()
{
  typedef int value_type;
  typedef std::allocator<value_type> std_allocator_type;

  typedef Set<int, hash1, std::equal_to<int>, std_allocator_type>
    hash1_unordered_set;
  typedef Set<int, hash2, std::equal_to<int>, std_allocator_type>
    hash2_unordered_set;

  typedef Set<int, boost::hash<int>, equal1, std_allocator_type>
    equal1_unordered_set;
  typedef Set<int, boost::hash<int>, equal2, std_allocator_type>
    equal2_unordered_set;

  // test allocators with a raw pointer as their `::pointer` type
  //
  typedef Set<int, boost::hash<int>, std::equal_to<int>,
    test::allocator1<value_type> >
    alloc1_unordered_set;
  typedef Set<int, boost::hash<int>, std::equal_to<int>, std_allocator_type>
    std_alloc_unordered_set;

  // test allocators with a fancy pointer as their `::pointer` type
  //
  typedef Set<int, boost::hash<int>, std::equal_to<int>,
    allocator1<value_type> >
    fancy1_unordered_set;
  typedef Set<int, boost::hash<int>, std::equal_to<int>,
    allocator2<value_type> >
    fancy2_unordered_set;

  scary_test<alloc1_unordered_set, std_alloc_unordered_set>();
  scary_test<hash1_unordered_set, hash2_unordered_set>();
  scary_test<equal1_unordered_set, equal2_unordered_set>();
  scary_test<fancy1_unordered_set, fancy2_unordered_set>();
}

UNORDERED_AUTO_TEST (scary_tests) {
  map_scary_test<boost::unordered_map>();
  map_scary_test<boost::unordered_multimap>();

  set_scary_test<boost::unordered_set>();
  set_scary_test<boost::unordered_multiset>();
}

RUN_TESTS()
