.. _cumulative_histogram:

Making a cumulative histogram
=============================

Overview
--------

A cumulative histogram is a histogram in which each bin stores the count / frequency of itself
as well as all the bins with keys 'smaller' than the particular bin.
As such, a notion of ordering among its keys should be existant in the histogram.

The GIL histogram class has the ability to convert itself into its cumulative version. 

Since the container needs to first get an ordering
over the keys a key sorting takes place before calculating the cumulative histogram. 

Example:

    .. code-block:: cpp

        histogram<int, float> h;
        /*
        Fill histogram ...
        */
        auto h1 = cumulative_histogram(h);

Tip: *In case you need to store the cumulative histogram elsewhere, consider creating a copy of the histogram
and then call the function*.