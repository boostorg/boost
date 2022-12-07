Overview
========

.. contents::
   :local:
   :depth: 1

Description
-----------

The histogram class is built on top of std::unordered_map to keep it compatible with other
STL algorithms. It can support any number of axes (known at compile time i.e. during class
instantiation). Suitable conversion routines from GIL image constructs to the histogram bin 
key are shipped with the class itself.


Tutorials
---------
The following flow is recommended:
    #. :ref:`create_histogram`
    #. :ref:`fill_it`
    #. :ref:`sub_histogram`
    #. :ref:`cumulative_histogram`
    #. :ref:`stl_compatibility`
    #. :ref:`extend_support`
    #. :ref:`limitations`

.. note:: To try out these tutorials you need to get a clone of the repository, since it is not yet released.
