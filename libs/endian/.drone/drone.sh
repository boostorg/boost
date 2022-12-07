#!/bin/bash

# Copyright 2022 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

set -ex

DRONE_BUILD_DIR=$(pwd)

BOOST_BRANCH=develop
if [ "$DRONE_BRANCH" = "master" ]; then BOOST_BRANCH=master; fi

cd ..
git clone -b $BOOST_BRANCH --depth 1 https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule update --init tools/boostdep
cp -r $DRONE_BUILD_DIR/* libs/$LIBRARY
python tools/boostdep/depinst/depinst.py $LIBRARY
./bootstrap.sh
./b2 -d0 headers

echo "using $TOOLSET : : $COMPILER ;" > ~/user-config.jam
./b2 -j3 libs/$LIBRARY/test toolset=$TOOLSET cxxstd=$CXXSTD variant=debug,release ${ADDRMD:+address-model=$ADDRMD} ${UBSAN:+undefined-sanitizer=norecover debug-symbols=on} ${ASAN:+address-sanitizer=norecover debug-symbols=on} ${LINKFLAGS:+linkflags=$LINKFLAGS}
