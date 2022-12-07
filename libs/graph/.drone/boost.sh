#!/bin/bash

set -ex
export TRAVIS_BUILD_DIR=$(pwd)
export DRONE_BUILD_DIR=$(pwd)
export TRAVIS_BRANCH=$DRONE_BRANCH
export VCS_COMMIT_ID=$DRONE_COMMIT
export GIT_COMMIT=$DRONE_COMMIT
export PATH=~/.local/bin:/usr/local/bin:$PATH

echo '==================================> BEFORE_INSTALL'

. .drone/before-install.sh

echo '==================================> INSTALL'

cd ..
git clone -b $TRAVIS_BRANCH --depth 1 https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule update --init tools/build
git submodule update --init libs/config
git submodule update --init tools/boost_install
git submodule update --init libs/headers
git submodule update --init tools/boostdep
cp -r $TRAVIS_BUILD_DIR/* libs/graph
python tools/boostdep/depinst/depinst.py graph
./bootstrap.sh
./b2 headers

echo '==================================> BEFORE_SCRIPT'

. $DRONE_BUILD_DIR/.drone/before-script.sh

echo '==================================> SCRIPT'

echo "using $TOOLSET : : $COMPILER : $OPTIONS ;" > ~/user-config.jam
(cd libs/config/test && ../../../b2 cxxstd=$CXXSTD config_info_travis_install toolset=$TOOLSET && ./config_info_travis)
(cd libs/graph/test && ../../../b2 -j3 cxxstd=$CXXSTD toolset=$TOOLSET define=CI_SUPPRESS_KNOWN_ISSUES)

echo '==================================> AFTER_SUCCESS'

. $DRONE_BUILD_DIR/.drone/after-success.sh
