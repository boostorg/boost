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

gem install asciidoctor || echo "ERROR. Failed to install asciidoctor"
gem install coderay || echo "ERROR. Failed to install coderay"
cd ..
git clone -b master --depth 1 https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule update --init tools/build
git submodule update --init tools/inspect
git submodule update --init libs/config
git submodule update --init tools/boostdep
rm -rf libs/exception
mkdir libs/exception
cp -r $TRAVIS_BUILD_DIR/* libs/exception
python tools/boostdep/depinst/depinst.py exception
./bootstrap.sh
./b2 headers
cd libs/exception

echo '==================================> BEFORE_SCRIPT'

. $DRONE_BUILD_DIR/.drone/before-script.sh

echo '==================================> SCRIPT'

echo "using $TOOLSET : : $COMPILER ;" > ~/user-config.jam
../../b2 test toolset=$TOOLSET cxxstd=$CXXSTD variant=debug,release ${UBSAN:+cxxflags=-fsanitize=address,undefined cxxflags=-fno-sanitize-recover=address,undefined linkflags=-fsanitize=address,undefined debug-symbols=on} ${LINKFLAGS:+linkflags=$LINKFLAGS}
../../b2 exception-handling=off rtti=off test toolset=$TOOLSET cxxstd=$CXXSTD variant=debug,release ${UBSAN:+cxxflags=-fsanitize=address,undefined cxxflags=-fno-sanitize-recover=address,undefined linkflags=-fsanitize=address,undefined debug-symbols=on} ${LINKFLAGS:+linkflags=$LINKFLAGS}

echo '==================================> AFTER_SUCCESS'

. $DRONE_BUILD_DIR/.drone/after-success.sh
