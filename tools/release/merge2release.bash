#!/bin/bash
# Copyright Frederic Bron 2011
# Distributed under the Boost Software License, Version 1.0.  See http://www.boost.org/LICENSE_1_0.txt

set -x

function merge() {
	item=$1; shift
	LC_ALL=C svn merge "$@"\
		https://svn.boost.org/svn/boost/branches/release/$item\
		https://svn.boost.org/svn/boost/trunk/$item
}

if [ $# -lt 1 ]; then
	echo "Usage: merge2release.bash library-name [svn-options]"
	echo "Options include --dry-run"
	exit
fi
if [ -z "$BOOST_RELEASE" ]; then
	echo "environment variable BOOST_RELEASE should point to the boost/branches/release directory"
	exit
fi

lib=$1
shift

cd $BOOST_RELEASE
path=`pwd`
cd $path/boost
merge boost/$lib.hpp "$@"
cd $path/boost/$lib
merge boost/$lib "$@"
cd $path/libs/$lib
merge libs/$lib "$@"
