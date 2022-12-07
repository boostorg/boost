.. _fill_it:

Fill histogram
==============

.. contents::
   :local:
   :depth: 1

Overview
--------

We will demonstrate the available options for filling an instance of the `histogram` class with
values that cater from the most simplest to the complex needs that might arise.

Basic
-----

#. Use operator()

    **Task** - Add value to a particular cell / key / bin in histogram

    .. code-block:: cpp

        histogram<int, int> h;
        h(1, 2) = 1;

#. Use operator[]

    This requires to input the indices in a format the histogram internally stores its keys,
    which is of ``std::tuple`` due to its simple interface.

    **Task** - Output value of a bin

    .. code-block:: cpp

        histogram<int, int> h;
        h(1, 2) = 1;
        h[{1, 2}] += 1; // Note the curly braces reqd. to construct a tuple
        std::cout<<h[{1, 2}]; // Output - 2
    
#. Use another histogram (copy constructor or assignment)

    **Task** - Fill a histogram using another

    .. code-block:: cpp

        histogram<int, int> A;
        /*
        Fill value in A
        */
        histogram<int, int> B(A), C;
        C = A;

#. Use a GIL image view

    You can also use GIL images to directly fill histograms.

    **Task** - Fill histogram using GIL image view

    .. code-block:: cpp

        gil::gray8_image_t img;
        /*
        Fill img ...
        */
        histogram<int> h;
        h.fill(view(img));
        // OR
        gil::fill_histogram(view(img), h, false); // false if histogram needs to be cleared before filling


Advanced
--------

#. Fill histogram using only a few dimensions of image

    **Task** - Make an histogram over Red and Blue channel of an rgb image

    .. code-block:: cpp

        gil::rgb8_image_t img;
        /*
        Fill img ...
        */
        histogram<int, int> h;
        fill_histogram<0, 2>(view(img), h, false); // 0 - red, 1 - green, 2 - blue

#. Fill histogram using GIL pixel

    **Task** - Fill histogram bin using pixel construct in GIL

    .. code-block:: cpp

        gil::gray8_image_t img;
        /*
        Fill img ...
        */
        histogram<int> h;
        gil::for_each_pixel(view(img), [](gil::gray8_pixel_t const& p){
            ++h[h.key_from_pixel(p)];
        });

