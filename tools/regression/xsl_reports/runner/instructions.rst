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
  you'd like to use, just put them in the same directory with ``regression.py``, e.g.:

  .. parsed-literal::

    my_boost_regressions/
        regression.py
        bjam\ *[.exe]*


Running tests
-------------

To start a regression run, simply run ``regression.py`` providing it with the following
two arguments:

- runner id (something unique of your choice that will identify your 
  results in the reports [#runnerid1]_, [#runnerid2]_)

- a particular set of toolsets you want to test with [#toolsets]_.

For example::

    python regression.py --runner=Metacomm --toolsets=gcc,vc7
    

If you are interested in seeing all available options, run ``python regression.py``
or ``python regression.py --help``. See also the `Advanced use`_ section below.
  
**Note**: If you are behind a firewall/proxy server, everything should still "just work". 
In the rare cases when it doesn't, you can explicitly specify the proxy server 
parameters through the ``--proxy`` option, e.g.:

.. parsed-literal::

    python regression.py ... **--proxy=http://www.someproxy.com:3128**


Details
-------

The regression run procedure will:

* Download the most recent tarball from http://www.meta-comm.com/engineering/boost/snapshot/,
  unpack it in the subdirectory ``boost``.

* Build ``bjam`` and ``process_jam_log`` if needed. (``process_jam_log`` is an
  utility, which extracts the test results from the log file produced by 
  Boost.Build).

* Run regression tests, process and collect the results.

* Upload the results to ftp://fx.meta-comm.com/boost-regression.


The report merger process running on MetaCommunications site every 2 hours will 
merge all submitted test runs and publish them at 
http://boost.sourceforge.net/regression-logs/developer.


Advanced use
------------

Incremental runs
................

You can run ``regression.py`` in incremental mode [#incremental]_ by simply passing 
it an identically named command-line flag:

.. parsed-literal::

      python regression.py ... **--incremental**


Dealing with misbehaved tests/compilers
.......................................

Depending on the environment/C++ runtime support library the test is compiled with, 
a test failure/termination may cause an appearance of a dialog window, requiring
human intervention to proceed. Moreover, the test (or even of the compiler itself)
can fall into infinite loop, or simply run for too long. To allow ``regression.py`` 
to take care of these obstacles, add the ``--monitored`` flag to the script 
invocation:

.. parsed-literal::

      python regression.py ... **--monitored**


That's it. Knowing your intentions, the script will be able to automatically deal 
with the listed issues [#monitored]_.


Getting sources from CVS
........................

If you already have a CVS client installed and configured, you might prefer to get
the sources directly from the Boost CVS repository. To communicate this to the 
script, you just need to pass it your SourceForge user ID using the ``--user`` 
option; for instance:

.. parsed-literal::

      python regression.py ... **--user=agurtovoy**

You can also specify the user as ``anonymous``, requesting anonymous CVS access. 
Note, though, that the files obtained this way tend to lag behind the actual CVS 
state by several hours, sometimes up to twelve. By contrast, the tarball the script 
downloads by default is at most one hour behind.


Integration with a custom driver script
.......................................

Even if you've already been using a custom driver script, and for some 
reason you don't  want ``regression.py`` to take over of the entire test cycle, 
getting your regression results into `Boost-wide reports`__ is still easy!

In fact, it's just a matter of modifying your script to perform two straightforward 
operations:

1. *Timestamp file creation* needs to be done before the CVS update/checkout.
   The file's location doesn't matter (nor does the content), as long as you know how 
   to access it later. Making your script to do something as simple as
   ``echo >timestamp`` would work just fine.

2. *Collecting and uploading logs* can be done any time after ``process_jam_log``' s
   run, and is as simple as an invocation of the local copy of
   ``$BOOST_ROOT/tools/regression/xsl_reports/runner/collect_and_upload_logs.py``
   script that was just obtained from the CVS with the rest of the sources.
   You'd need to provide ``collect_and_upload_logs.py`` with the following three
   arguments::

        --locate-root   directory to to scan for "test_log.xml" files
        --runner        runner ID (e.g. "Metacomm")
        --timestamp     path to a file which modification time will be used 
                        as a timestamp of the run ("timestamp" by default)

   For example, assuming that the run's resulting  binaries are in the
   ``$BOOST_ROOT/bin`` directory (the default Boost.Build setup), the 
   ``collect_and_upload_logs.py`` invocation might look like this::

       python $BOOST_ROOT/tools/regression/xsl_reports/runner/collect_and_upload_logs.py 
          --locate-root=$BOOST_ROOT/bin
          --runner=Metacomm
          --timestamp=timestamp


__ http://www.boost.org/regression-logs/developer/


Feedback
--------

Please send all comments/suggestions regarding this document and the testing procedure 
itself to the `Boost Testing list`__.

__ http://lists.boost.org/mailman/listinfo.cgi/boost-testing


Notes
-----

.. [#runnerid1] If you are running regressions interlacingly with a different 
   set of compilers (e.g. for Intel in the morning and GCC at the end of the day), you need 
   to provide a *different* runner id for each of these runs, e.g. ``your_name-intel``, and
   ``your_name-gcc``.

.. [#runnerid2] The limitations of the reports' format/medium impose a direct dependency
   between the number of compilers you are testing with and the amount of space available 
   for your runner id. If you are running regressions for a single compiler, please make 
   sure to choose a short enough id that does not significantly disturb the reports' layout.

.. [#toolsets] If ``--toolsets`` option is not provided, the script will try to use the 
   platform's default toolset (``gcc`` for most Unix-based systems).

.. [#incremental] By default, the script runs in what is known as *full mode*: on 
   each ``regression.py`` invocation all the files that were left in place by the 
   previous run -- including the binaries for the successfully built tests and libraries 
   -- are deleted, and everything is rebuilt once again from scratch. By contrast, in 
   *incremental mode* the already existing binaries are left intact, and only the 
   tests and libraries which source files has changed since the previous run are 
   re-built and re-tested.

   The main advantage of incremental runs is a significantly shorter turnaround time, 
   but unfortunately they don't always produce reliable results. Some type of changes
   to the codebase (changes to the bjam testing subsystem in particular)
   often require switching to a full mode for one cycle in order to produce 
   trustworthy reports. 
   
   As a general guideline, if you can afford it, testing in full mode is preferable.

.. [#monitored] Note that at the moment this functionality is available only if you 
   are running on a Windows platform. Contributions are welcome!
   
