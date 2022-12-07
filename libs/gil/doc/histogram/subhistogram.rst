.. _sub_histogram:

Making a sub-histogram
======================

Overview
--------

Sub-histogram is a subset of a histogram or one that is formed by masking out a 
few axis of the parent histogram.

GIL class histogram provides these functions as members and returns an instance of
the desired sub-class.

#. Histogram over fewer axes

    **Task** - Get a 2D histogram from a 3D RGB histogram over red and green axes

    .. code-block:: cpp

        histogram<int, int, int> h;
        gil::rgb8_image_t img;
        /*
        Fill img ...
        */
        fill_histogram(view(img), h, false);
        auto sub_h = h.sub_histogram<0, 1>(); // sub_h is a 2D histogram

    Demo output:

    .. code-block:: cpp

        h is {{1, 2, 3} : 1, 
              {1, 4, 3} : 2, 
              {1, 2, 5} : 3}
        sub_h would be {{1, 2} : 4, {1, 4} : 2}

#. Histogram using a particular key range

    **Task** - Get a 2D histogram from a 3D RGB histogram for bins whose red color lie between 10 - 20
    and blue color lie between 2 - 10

    .. code-block:: cpp

        histogram<int, int, int> h;
        gil::rgb8_image_t img;
        /*
        Fill img ...
        */
        fill_histogram(view(img), h, false);
        std::tuple<int, int, int> low, high;
        low = {10, 0, 2} // Since no check over blue channel pass any dummy value
        high = {20, 0, 10} // Since no check over blue channel pass any dummy value
        auto sub_h = h.sub_histogram<0, 2>(low, high); // In angle brackets pass the relevant dimensions in order

    Demo Output:

    .. code-block:: cpp

        h is {{11, 2, 3 } : 1,
              {1 , 4, 11} : 2,
              {1 , 2, 1 } : 3,
              {11, 3, 3 } : 4}
        sub_h would be {{11, 2, 3} : 1, {11, 3, 3} : 4}


