Running Boost Regression Tests
==============================

Requirements
------------

* Python 2.3

That's it! You don't even need a CVS client installed.

Installation
------------

* Download regression driver ``regression.py`` from here__ (http://tinyurl.com/4fp4g)
  and put it in the directory where you want all the regression 
  test files to be placed.

__ http://cvs.sourceforge.net/viewcvs.py/*checkout*/boost/boost/tools/regression/xsl_reports/runner/regression.py


* **Optional**: If you already have ``bjam`` and/or ``process_jam_log`` executables
  you'd like to use, just put them in the same directory with ``regression.py``, e.g.::

   my_boost_regressions/
      regression.py
      bjam.exe


Running tests
-------------

* To start a regression run, simply run ``regression.py`` providing it with the only
  required option, runner id (something unique of your choice that will identify your 
  results in the reports [#runnerid1]_, [#runnerid2]_). For example::

    python regression.py --runner=Metacomm
  
  You can specify a particular set of toolsets you want to test with by passing them as 
  a comma-separated list using the ``--toolsets`` option:
  
  .. parsed-literal::

     python regression.py --runner=Metacomm **--toolsets=gcc,vc7**
  
  
  If you are interested in seeing all available options, run ``python regression.py``
  or ``python regression.py --help``.
  
  **Note**: If you are behind a firewall/proxy server, everything should still "just work". 
  In the rare cases when it doesn't, you can explicitly specify the proxy server 
  parameters through the ``--proxy`` option, e.g.:

  .. parsed-literal::

     python regression.py --runner=Metacomm **--proxy=http://www.someproxy.com:3128**


.. [#runnerid1] If you are running regressions interlacingly with a different 
   set of compilers (e.g. for Intel in the morning and GCC at the end of the day), you need 
   to provide a *different* runner id for each of these runs, e.g. ``your_name-intel``, and
   ``your_name-gcc``.

.. [#runnerid2] The limitations of the reports' format/medium impose a direct dependency
   between the number of compilers you are testing with and the amount of space available 
   for your runner id. If you are running regressions for a single compiler, please make 
   sure to choose a short enough id that does not significantly disturb the reports' picture.

Details
-------

The regression run procedure will:

* Download the most recent tarball from http://www.boost-consulting.com, 
  unpack it in the subdirectory ``boost``.

* Build ``bjam`` and ``process_jam_log`` if needed. (``process_jam_log`` is an
  utility, which extracts the test results from the log file produced by 
  Boost.Build)

* Run regression tests, process and collect the results.

* Upload the results to ftp://fx.meta-comm.com/boost-regression.


The report merger process running on MetaCommunications site every 2 hours will 
merge all submitted test runs and publish them at 
http://boost.sourceforge.net/regression-logs/developer.


Advanced use
------------

Incremental runs
................

You can run ``regression.py`` in *incremental mode* [#incremental]_ by simply passing 
it an identically named command line flag:

.. parsed-literal::

      python regression.py --runner=Metacomm **--incremental**



Message dialogs/access violations
.................................

TODO

Getting source from CVS
.......................

TODO


.. [#incremental] By default the script runs in what is known as *full run mode*: on 
   each script invocation all the files that were left in place by the previous run 
   -- in particular, binaries for successfully built tests and libraries -- are 
   deleted, and rebuilt once again from scratch. By contrast, in *incremental run* 
   mode the already existing binaries are left intact, and only tests and libraries 
   which source files has changed since the previous run are re-built and re-tested.
