Running Boost regression tests
==============================

Requirements
------------

* Python 2.3

Installation
------------

* Download and boostrap file from
  http://www.boost.org/status/bootstrap.py and put it in the directory
  where you want all the regression test files to be placed.

* Run bootstrap.py. This will download Download the most recent
  tarball for the tag from http://www.boost-consulting.com and unpack it 
  in the subdirectory boost

* **Optional** If you already have bjam and process_jam_log you want
  to use just put them in the same directory with bootstrap.py file.

The resulting directory will look like::  

   boost_regression/
      boost/
      bootstrap.py
      bjam.exe
      
      
Running regression tests
------------------------

* Run regression.py with the following parameters 

  * ... - the id of the runner. 

  * ... - the tag identifies the CVS branch or tarball under test. The
    name of the tag is provided by the release manager
  
::

  boostrap.py --runner=metacomm --tag=CVS-HEAD

The regression run procedure will:

* Build bjam and process_jam_log if needed. (process_jam_log is a
  utility, which extracts the test results from log file produced by
  Boost.Build)

* Run regression tests, process and collect the results

* Upload the results to ftp://fx.meta-comm.com/boost-regression.


The report merger process running on MetaCommunications site every 2 hours will merge all
submitted test runs and publish them at http://boost.sourceforge.net/regression-logs/developer
