#!/bin/sh
#
# shell script for running the boost regression test suite and generating
# a html table of results.
# This version requires a recent copy of gcc installed on the system, without 
# it you will need to do a little editing.
#
# Start by setting the following shell variables as required:

#
# BOOST_ROOT points to the root of you boost installation:
#
export BOOST_ROOT=/home/jm/boost
#
# tool_subpath points to the subdirectory containing your built
# helper tools, this should be boilerplate for gcc:
#
tool_subpath=gcc/release/runtime-link-dynamic
#
# exe_suffix the suffix used by exectable files:
#
exe_suffix=
#
# bjam points to your built bjam executable, should be boilerplate:
#
bjam=$BOOST_ROOT/tools/build/jam_src/bin/bjam
#
# paths to built helper programs, should be boilerplate:
#
process_jam_log=$BOOST_ROOT/tools/regression/build/bin/process_jam_log$exe_suffix/$tool_subpath/process_jam_log$exe_suffix
compiler_status=$BOOST_ROOT/tools/regression/build/bin/compiler_status$exe_suffix/$tool_subpath/compiler_status$exe_suffix




#
# STEP 1:
# rebuild bjam if required:
#
echo building bjam:
cd $BOOST_ROOT/tools/build/jam_src && \
make CC=gcc YACC="" LOCATE_TARGET=bin

# error check:
if [ $? != 0 ]; then
	echo "bjam build failed."
	exit 256
fi

#
# STEP 2:
# rebuild the regression test helper programs if required:
#
echo building regression test helper programs
cd $BOOST_ROOT/tools/regression/build && \
$bjam -sTOOLS=gcc -sBUILD=release -sLOCATE_TARGET=bin

# error check:
if [ $? != 0 ]; then
	echo "helper program build failed."
	exit 256
fi

#
# STEP 3:
# run the regression tests:
#
echo running the gcc regression tests
cd $BOOST_ROOT/status
$bjam -sTOOLS=gcc -sLOCATE_TARGET=bin --dump-tests test 2>&1 | tee regress.log

#
# STEP 4:
# post process the results:
#
echo processing the regression test results for gcc:
cat regress.log | $process_jam_log

# error check:
if [ $? != 0 ]; then
	echo "Failed regression log post processing."
	exit 256
fi

#
# STEP 5:
# repeat steps 3 and 4 for each additional toolset:
#

#
# STEP 6:
# create the html table:
#
echo generating html tables:
$compiler_status $BOOST_ROOT cs-$(uname).html cs-$(uname)-links.html

# error check:
if [ $? != 0 ]; then
	echo "Failed HTML result table generation."
	exit 256
fi

echo done!


