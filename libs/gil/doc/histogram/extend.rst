.. _extend_support:

Extending the class
===================

.. contents::
   :local:
   :depth: 1

User defined Axes
-----------------

In case you need a histogram with an axes of an arbitrary type that is not identified by
the C++ Standard Library, you need to provide a overload for the hashing function that is 
used in the histogram class.

GIL's histogram class uses ``boost::hash_combine`` in a sub routine to generate a hash from
the key.

So we need to provide an overload of ``boost::hash_combine`` for the purpose.

For example, let's consider you need a histogram with an axis over class Test.

.. code-block:: cpp

    // File : ./test.hpp
    #include <cstddef>
    #include <functional>

    struct Test 
    {
        int a{0};
        Test() = default;
        Test(int c) : a(c) {}
        bool operator==(Test const& other) const
        {
            return (a == other.a);
        }
    };

    namespace boost {
        std::size_t hash_value(Test const& t)
        {
            // Replace with your hashing code
            std::hash<int> hasher;
            return hasher(t.a);
        }
    }

Now lets get to the usage example.

.. code-block:: cpp 

    #include <test.hpp> 
    #include <boost/gil.hpp>
    #include <iostream>
    // Mind the order of include i.e. test.hpp before boost/gil.hpp

    using namespace boost::gil;

    int main() 
    {
        boost::gil::histogram<Test> h;
        Test t(1);
        h(t) = 1;
        std::cout<<h(t);
    }

