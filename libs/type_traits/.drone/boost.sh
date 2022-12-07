#!/bin/bash

set -ex
export TRAVIS_BUILD_DIR=$(pwd)
export DRONE_BUILD_DIR=$(pwd)
export TRAVIS_BRANCH=$DRONE_BRANCH
export VCS_COMMIT_ID=$DRONE_COMMIT
export GIT_COMMIT=$DRONE_COMMIT
export REPO_NAME=$DRONE_REPO
export PATH=~/.local/bin:/usr/local/bin:$PATH

echo '==================================> BEFORE_INSTALL'

. .drone/before-install.sh

echo '==================================> INSTALL'

cd ..
git clone -b $TRAVIS_BRANCH --depth 1 https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule update --init tools/build
git submodule update --init tools/boost_install
git submodule update --init libs/headers
git submodule update --init libs/config
git submodule update --init libs/assert
git submodule update --init libs/bind
git submodule update --init libs/core
git submodule update --init libs/detail
git submodule update --init libs/function
git submodule update --init libs/integer
git submodule update --init libs/move
git submodule update --init libs/mpl
git submodule update --init libs/preprocessor
git submodule update --init libs/static_assert
git submodule update --init libs/throw_exception
git submodule update --init libs/type_index
git submodule update --init libs/utility
cp -r $TRAVIS_BUILD_DIR/* libs/type_traits
./bootstrap.sh
./b2 headers

echo '==================================> BEFORE_SCRIPT'

. $DRONE_BUILD_DIR/.drone/before-script.sh

echo '==================================> SCRIPT'

echo "using $TOOLSET : : $COMPILER ;" > ~/user-config.jam
IFS=','
for CXXLOCAL in $CXXSTD; do  (cd libs/config/test && ../../../b2 config_info_travis_install toolset=$TOOLSET cxxstd=$CXXLOCAL $CXXSTD_DIALECT && echo With Standard Version $CXXLOCAL && ./config_info_travis && rm ./config_info_travis)  done
unset IFS
./b2 libs/type_traits/test toolset=$TOOLSET cxxstd=$CXXSTD $CXXSTD_DIALECT

echo '==================================> AFTER_SUCCESS'

. $DRONE_BUILD_DIR/.drone/after-success.sh
