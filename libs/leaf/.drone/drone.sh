#!/bin/bash

# Copyright 2020 Rene Rivera, Sam Darwin
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt)

set -e
export TRAVIS_BUILD_DIR=$(pwd)
export DRONE_BUILD_DIR=$(pwd)
export TRAVIS_BRANCH=$DRONE_BRANCH
export VCS_COMMIT_ID=$DRONE_COMMIT
export GIT_COMMIT=$DRONE_COMMIT
export REPO_NAME=$DRONE_REPO
export PATH=~/.local/bin:/usr/local/bin:$PATH

if [ "$DRONE_JOB_BUILDTYPE" == "boost" ]; then

echo '==================================> INSTALL'

cd ..
git clone -b master --depth 1 https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule update --init tools/build
git submodule update --init tools/inspect
git submodule update --init libs/config
git submodule update --init tools/boostdep
mkdir -p libs/leaf
cp -r $TRAVIS_BUILD_DIR/* libs/leaf
python tools/boostdep/depinst/depinst.py leaf -I example
./bootstrap.sh
./b2 headers
cd libs/leaf

echo '==================================> SCRIPT'

echo "using $TOOLSET : : $COMPILER ;" > ~/user-config.jam
../../b2 test toolset=$TOOLSET cxxstd=$CXXSTD variant=debug,release,leaf_debug_diag0,leaf_release_diag0 ${UBSAN:+cxxflags=-fsanitize=undefined cxxflags=-fno-sanitize-recover=undefined linkflags=-fsanitize=undefined debug-symbols=on} ${LINKFLAGS:+linkflags=$LINKFLAGS}
../../b2 exception-handling=off rtti=off test toolset=$TOOLSET cxxstd=$CXXSTD variant=debug,release,leaf_debug_diag0,leaf_release_diag0 ${UBSAN:+cxxflags=-fsanitize=undefined cxxflags=-fno-sanitize-recover=undefined linkflags=-fsanitize=undefined debug-symbols=on} ${LINKFLAGS:+linkflags=$LINKFLAGS}
../../b2 threading=single test toolset=$TOOLSET cxxstd=$CXXSTD variant=debug,release,leaf_debug_diag0,leaf_release_diag0 ${UBSAN:+cxxflags=-fsanitize=undefined cxxflags=-fno-sanitize-recover=undefined linkflags=-fsanitize=undefined debug-symbols=on} ${LINKFLAGS:+linkflags=$LINKFLAGS}
../../b2 threading=single exception-handling=off rtti=off test toolset=$TOOLSET cxxstd=$CXXSTD variant=debug,release,leaf_debug_diag0,leaf_release_diag0 ${UBSAN:+cxxflags=-fsanitize=undefined cxxflags=-fno-sanitize-recover=undefined linkflags=-fsanitize=undefined debug-symbols=on} ${LINKFLAGS:+linkflags=$LINKFLAGS}

fi
