# Refactoring Roadmap

[Proof of concept](https://github.com/joaquintides/fca_unordered) implementation for a fast closed-addressing implementation.

## Plan of Refactoring

* remove `ptr_node` and `ptr_bucket`
* see if the code can survive a lack of the `extra_node` or maybe we hard-code it in
* implement bucket groups as they are in `fca` but don't use them directly yet, add alongside the `buckets_` data member in `struct table`
* try to remove `bucket_info_` from the node structure (breaks all call-sites that use `get_bucket()` and dependents)
* make sure `fca` can successfully handle multi-variants at this stage + supports mutable iterators for `map`/`multimap`
* do a hard-break:
    * update code to no longer use one single linked list across all buckets (each bucket contains its own unique list)
    * integrate the `bucket_group<Node>` structure into the `table` (update iterator call-sites to include `bucket_iterator`s)

Blockers:
* how to handle `multi` variants with new `fca` prototype

## Implementation Differences

### Unordered

### Node Type

Bullet Points:
* reify node type into a single one
* come up with implementation for multi- variants
* code that touches `get_bucket()` and `*_in_group()` member functions may need updating

There are two node types in Unordered, `struct node` and `struct ptr_node`, and the node type is selected conditionally based on the Allocator's pointer type:
```c++
template <typename A, typename T, typename NodePtr, typename BucketPtr>
struct pick_node2
{
  typedef boost::unordered::detail::node<A, T> node;
  // ...
};

template <typename A, typename T>
struct pick_node2<A, T, boost::unordered::detail::ptr_node<T>*,
  boost::unordered::detail::ptr_bucket*>
{ 
  typedef boost::unordered::detail::ptr_node<T> node;
  // ...
};

template <typename A, typename T> struct pick_node
{
  typedef typename boost::remove_const<T>::type nonconst;

  typedef boost::unordered::detail::allocator_traits<
    typename boost::unordered::detail::rebind_wrap<A,
      boost::unordered::detail::ptr_node<nonconst> >::type>
    tentative_node_traits;

  typedef boost::unordered::detail::allocator_traits<
    typename boost::unordered::detail::rebind_wrap<A,
      boost::unordered::detail::ptr_bucket>::type>
    tentative_bucket_traits;

  typedef pick_node2<A, nonconst, typename tentative_node_traits::pointer,
    typename tentative_bucket_traits::pointer>
    pick;

  typedef typename pick::node node;
  typedef typename pick::bucket bucket;
  typedef typename pick::link_pointer link_pointer;
};
```

The node types are identical in terms of interface and the only difference is that `node` is chosen when the Allocator uses fancy pointers and `ptr_node` is chosen when the Allocator's pointer type is `T*`.

Nodes in Unorderd store `bucket_info_`:
```cpp
template <typename A, typename T>
struct node : boost::unordered::detail::value_base<T>
{
  link_pointer next_;
  std::size_t bucket_info_;
  node() : next_(), bucket_info_(0) {}
  // ...
};
```

`bucket_info_` maps each node back to its corresponding bucket via the member function:
```cpp
std::size_t get_bucket() const
{
  return bucket_info_ & ((std::size_t)-1 >> 1);
}
```

`bucket_info_` is also used to demarcate the start of equivalent nodes in the containers via:
```cpp
// Note that nodes start out as the first in their group, as `bucket_info_` defaults to 0.
std::size_t is_first_in_group() const
{ return !(bucket_info_ & ~((std::size_t)-1 >> 1)); }

void set_first_in_group()
{ bucket_info_ = bucket_info_ & ((std::size_t)-1 >> 1); }

void reset_first_in_group()
{ bucket_info_ = bucket_info_ | ~((std::size_t)-1 >> 1); }
```

A goal of refactoring is to simply have one node type:
```cpp
template<class T>
struct node {
  node *next;
  T    value;
};
```
that is used unconditionally. This also requires updating the code that touches the `bucket_info_` along with the code that that touches the `*_in_group()` member functions.

### Bucket Type

Bullet points:
* reify bucket structure into a single one
* figure out how to add `bucket_group`s to the table struct

Buckets are similar to nodes in that there are two variations: `template<class NodePointer> struct bucket` and `struct ptr_bucket`.

The buckets exist to contain a pointer to a node, however they contain an `enum { extra_node = true };` or `enum { extra_node = false }` to determine whether or not the code should explicitly allocate a default constructed node whose address assigned as the dummy node at the end of the bucket array.

`extra_node` is used in the creation and deletion of the bucket array but it is not inherently clear what its intended purpose is.

### Iterators

Iterators are currently templated on the type of Node they store. Because `fca` constructs iterators with two arguments, all the call-sites that instantiate iterators will need to be updated but this a straight-forward mechanical change.

Iterators are selected, as of now, via the `detail::map` and `detail::set` class templates.

For example, for `unordered_map`, `iterator` is defined as:
```cpp
typedef boost::unordered::detail::map<A, K, T, H, P> types;
typedef typename types::table table;
typedef typename table::iterator iterator;
```

The iterator is a member typedef of the `table` which is `types::table`. Examining `types` (aka `detail::map<...>`), we see:
```cpp
template <typename A, typename K, typename M, typename H, typename P>
struct map {
  // ...
  typedef boost::unordered::detail::table<types> table;
  // ...
};
```

Examining the `detail::table<types>` struct, we see:
```cpp
template <typename Types>
struct table {
  // ...
  typedef typename Types::iterator iterator;
  // ...
}
```

Collapsing all of this, we see that our iterator types are defined here:
```cpp
template <typename A, typename K, typename M, typename H, typename P>
struct map
{
  // ...
  typedef boost::unordered::detail::pick_node<A, value_type> pick;
  typedef typename pick::node node;

  typedef boost::unordered::iterator_detail::iterator<node> iterator;
  typedef boost::unordered::iterator_detail::c_iterator<node> c_iterator;
  typedef boost::unordered::iterator_detail::l_iterator<node> l_iterator;
  typedef boost::unordered::iterator_detail::cl_iterator<node>
    cl_iterator;
  // ...
};
```

This is similarly designed for `detail::set`:
```cpp
typedef boost::unordered::iterator_detail::c_iterator<node> iterator;
typedef boost::unordered::iterator_detail::c_iterator<node> c_iterator;
typedef boost::unordered::iterator_detail::cl_iterator<node> l_iterator;
typedef boost::unordered::iterator_detail::cl_iterator<node>
  cl_iterator;
```

The only difference here is that `set::iterator` is always a `c_iterator`, a `const_iterator` type.
