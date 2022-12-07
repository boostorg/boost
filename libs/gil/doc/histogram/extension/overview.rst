Overview
========

.. contents::
   :local:
   :depth: 1

Description
-----------
Apart from the default class supplied by Boost.GIL, there are also provisions to
use external containers like from std::vector, std::map, boost::histogram etc. These
are provided as extensions. 


Extensions
----------
Currently the following are available:
    #. std::vector (1D histogram support)
    #. std::map (1D histogram support)
    #. std::array (1D histogram support)
    #. std::unordered_map (1D histogram support)
    #. boost::histogram


Adding an external container
----------------------------
The workflow should be:
    #. Provide overloads for fill_histogram(must), make_histogram(optional) etc. in a new file preferably named after the container type in extensions/histogram/.
    #. Add tests to test/extensions/histogram.
    #. Add docs to docs/histogram/extensions.
    #. Other cmake, Jamfile, config etc. file changes.

