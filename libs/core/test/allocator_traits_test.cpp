/*
Copyright 2021 Glen Joseph Fernandes
(glenjofe@gmail.com)

Distributed under the Boost Software License, Version 1.0.
(http://www.boost.org/LICENSE_1_0.txt)
*/
#include <boost/core/allocator_traits.hpp>
#include <boost/core/lightweight_test_trait.hpp>

template<class T>
class creator {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef void* void_pointer;
    typedef const void* const_void_pointer;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    template<class U>
    struct rebind {
        typedef creator<U> other;
    };

    explicit creator(int value) BOOST_NOEXCEPT
        : state_(value) { }

    template<class U>
    creator(const creator<U>& other) BOOST_NOEXCEPT
        : state_(other.state) { }

    std::size_t max_size() const BOOST_NOEXCEPT {
        return static_cast<std::size_t>(-1) / sizeof(T);
    }

    T* allocate(std::size_t n, const void* = 0) {
        return static_cast<T*>(::operator new(sizeof(T) * n));
    }

    void deallocate(T* p, std::size_t) {
        ::operator delete(p);
    }

    int state() const {
        return state_;
    }

private:
    int state_;
};

template<class T, class U>
inline bool operator==(const creator<T>& lhs,
    const creator<U>& rhs) BOOST_NOEXCEPT
{
    return lhs.state() == rhs.state();
}

template<class T, class U>
inline bool operator!=(const creator<T>& lhs,
    const creator<U>& rhs) BOOST_NOEXCEPT
{
    return !(lhs == rhs);
}

int main()
{
    BOOST_TEST_TRAIT_SAME(creator<int>,
        boost::allocator_traits<creator<int> >::allocator_type);
    BOOST_TEST_TRAIT_SAME(int,
        boost::allocator_traits<creator<int> >::value_type);
    BOOST_TEST_TRAIT_SAME(int*,
        boost::allocator_traits<creator<int> >::pointer);
    BOOST_TEST_TRAIT_SAME(const int*,
        boost::allocator_traits<creator<int> >::const_pointer);
    BOOST_TEST_TRAIT_SAME(void*,
        boost::allocator_traits<creator<int> >::void_pointer);
    BOOST_TEST_TRAIT_SAME(const void*,
        boost::allocator_traits<creator<int> >::const_void_pointer);
    BOOST_TEST_TRAIT_SAME(std::ptrdiff_t,
        boost::allocator_traits<creator<int> >::difference_type);
    BOOST_TEST_TRAIT_SAME(std::size_t,
        boost::allocator_traits<creator<int> >::size_type);
    BOOST_TEST_TRAIT_FALSE((boost::allocator_traits<creator<int> >::
        propagate_on_container_copy_assignment));
    BOOST_TEST_TRAIT_FALSE((boost::allocator_traits<creator<int> >::
        propagate_on_container_move_assignment));
    BOOST_TEST_TRAIT_FALSE((boost::allocator_traits<creator<int> >::
        propagate_on_container_swap));
    BOOST_TEST_TRAIT_FALSE((boost::allocator_traits<creator<int> >::
        is_always_equal));
    BOOST_TEST_TRAIT_SAME(creator<char>,
        boost::allocator_traits<creator<int> >::
            rebind_traits<char>::allocator_type);
    creator<int> a(1);
    int* p1 = boost::allocator_traits<creator<int> >::allocate(a, 1);
    if (BOOST_TEST(p1)) {
        boost::allocator_traits<creator<int> >::construct(a, p1, 5);
        BOOST_TEST_EQ(*p1, 5);
        boost::allocator_traits<creator<int> >::destroy(a, p1);
        boost::allocator_traits<creator<int> >::deallocate(a, p1, 1);
    }
    int* p2 = boost::allocator_traits<creator<int> >::allocate(a, 1, 0);
    if (BOOST_TEST(p2)) {
        boost::allocator_traits<creator<int> >::deallocate(a, p2, 1);
    }
    BOOST_TEST_EQ(boost::allocator_traits<creator<int> >::max_size(a),
        static_cast<std::size_t>(-1) / sizeof(int));
    BOOST_TEST(boost::allocator_traits<creator<int> >::
        select_on_container_copy_construction(a) == a);
    return boost::report_errors();
}
