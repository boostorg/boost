#!/bin/sh

set -e

cdir=${PWD}
cd $1
$HOME/temp/.b2/bin/b2 -d+2 ${B2_ARGS}
echo "-----"
find . -perm -111 -type f -exec file "{}" "+"
echo "-----"
find . -name "*.o" -exec file "{}" "+"
echo "-----"
find . -name "*.a" -exec file "{}" "+"
echo "-----"
find . -name "*.so" -exec file "{}" "+"
echo "-----"
find . -name "*.dylib" -exec file "{}" "+"
echo "-----"
cd ${cdir}

# Use, modification, and distribution are
# subject to the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt)
#
# Copyright Rene Rivera 2022.
