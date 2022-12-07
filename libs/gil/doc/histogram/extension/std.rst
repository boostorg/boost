.. _std:

STD extension
=============

Supported Types:
----------------

#. std\:\:vector (1D)
#. std\:\:map (1D)
#. std\:\:unordered_map (1D)
#. std\:\:array (1D)


Usage
-----

#. **fill_histogram()**

    .. code-block:: cpp
    
        // Demo for std::vector
        std::vector<int> v;
        gil::gray8_image_t img;
        /*
        Fill image ...
        */
        gil::fill_histogram(view(img), v, false);

#. **cumulative_histogram()**

    .. code-block:: cpp

        // Demo for std::vector
        std::vector<int> v;
        /*
        Fill vector...
        */
        gil::cumulative_histogram(v);






