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

export CHECKOUT_PATH=`pwd`;
if [ -n "$GCC_VERSION" ]; then export CXX="g++-${GCC_VERSION}" CC="gcc-${GCC_VERSION}"; fi
if [ -n "$CLANG_VERSION" ]; then export CXXFLAGS="${CXXFLAGS} -stdlib=libstdc++" CXX="clang++-${CLANG_VERSION}" CC="clang-${CLANG_VERSION}"; fi
export DEPS_DIR="${TRAVIS_BUILD_DIR}/deps"
mkdir ${DEPS_DIR} && cd ${DEPS_DIR}
mkdir usr
export PATH=${DEPS_DIR}/usr/bin:${PATH}
if [[ "$MAC_OSX" == "true" ]]; then
  export CMAKE_URL="http://www.cmake.org/files/v3.17/cmake-3.17.0-Darwin-x86_64.tar.gz"
else
  export CMAKE_URL="http://www.cmake.org/files/v3.17/cmake-3.17.0-Linux-x86_64.tar.gz"
fi
wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=3 -xz -C usr

echo $PATH
$CXX --version
which $CXX
true $CC --version
true which $CC
which cmake
cmake --version
export CXXFLAGS="${CXXFLAGS} -Wall"

echo '==================================> SCRIPT'

cd $CHECKOUT_PATH
export ASANVARIANT="false"
if [[ "$ASAN" == "on" ]]; then export ASANVARIANT="true"; fi
for build_type in Debug Release; do
  for asan_type in $ASANVARIANT; do
    build_dir="build-$build_type-asan-$asan_type"
    mkdir $build_dir
    cd $build_dir
    if [[ "$asan_type" == "true" ]]; then 
      CXXFLAGS="$CXXFLAGS" cmake -DUSE_ASAN=true -DBOOST_BRANCH=$TRAVIS_BRANCH -DCMAKE_BUILD_TYPE=$build_type ..
    else
      cmake -DBOOST_BRANCH=$TRAVIS_BRANCH -DCMAKE_BUILD_TYPE=$build_type ..
    fi
    VERBOSE=1 make -j4 && CTEST_OUTPUT_ON_FAILURE=1 CTEST_PARALLEL_LEVEL=4 ASAN_OPTIONS=alloc_dealloc_mismatch=0 make check
    if [ $? -ne 0 ]
    then
      exit 1
    fi
    cd ..
    rm -rf $build_dir
  done
done

fi
