Running Boost regression tests
==============================

Requirements
------------

* Python 2.3

Installation
------------

1. Download boostrap from http:// and put it in the directory where you want all the 
regression test files to be placed

2. Run bootstrap.py

3. [Optional] If you already have bjam you want to use just put it in the same directory 
with bootstrap.py file

4. [Optional] If you already have process_jam_log you want to use just put it in the same directory 
with bootstrap.py file

Running
-------

* Run regression.py with the following parameters 

  * ... - the id of the runner (e.g. metacomm, metacomm-2)
  * ... - the tag 
  * 
  

The regression run procedure will

* Download the most recent tarball from BoostCOnsulting
* Unpack it 
* Build bjam if needed. 
* Build process_jam_log if needed. process_jam_log is a utility, which extracts the test results from log file produced by Boost.Build
* Run regression tests, process and collect the results
* Upload the results to ftp://fx.meta-comm.com


The report merger process running on MetaCommunications site every 2 hours, will merge all
submitted test runs and publish the results at
