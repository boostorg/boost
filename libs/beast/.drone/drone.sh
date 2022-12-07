#!/bin/bash

# Copyright 2020 Rene Rivera, Sam Darwin
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt)

set -xe

export DRONE_BUILD_DIR=$(pwd)
export VCS_COMMIT_ID=$DRONE_COMMIT
export GIT_COMMIT=$DRONE_COMMIT
export REPO_NAME=$DRONE_REPO
export USER=$(whoami)
export CC=${CC:-gcc}
export PATH=~/.local/bin:/usr/local/bin:$PATH
export TRAVIS_BUILD_DIR=$(pwd)
export TRAVIS_BRANCH=$DRONE_BRANCH
export TRAVIS_EVENT_TYPE=$DRONE_BUILD_EVENT

common_install () {
  git clone https://github.com/boostorg/boost-ci.git boost-ci-cloned --depth 1
  cp -prf boost-ci-cloned/ci .
  rm -rf boost-ci-cloned

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
    unset -f cd
    echo "macos - set up homebrew openssl"
    export OPENSSL_ROOT=/usr/local/opt/openssl 

cat > ~/user-config.jam <<EOF
import os ;
local OPENSSL_ROOT = [ os.environ OPENSSL_ROOT ] ;
project
  : requirements
    <include>/usr/local/opt/openssl/include
    <variant>debug:<library-path>/usr/local/opt/openssl/lib
    <target-os>windows<variant>debug:<library-path>/usr/local/opt/openssl/debug/lib
    <variant>release:<library-path>/usr/local/opt/openssl/lib
  ;
EOF

  fi

  export SELF=`basename $REPO_NAME`
  export BOOST_CI_TARGET_BRANCH="$TRAVIS_BRANCH"
  export BOOST_CI_SRC_FOLDER=$(pwd)

  . ./ci/common_install.sh
}

if  [ "$DRONE_BEFORE_INSTALL" == "beast_coverage" ]; then
    LCOV_VERSION=${LCOV_VERSION:-1.14}
    pip install --user https://github.com/codecov/codecov-python/archive/master.zip
    wget http://downloads.sourceforge.net/ltp/lcov-$LCOV_VERSION.tar.gz
    tar -xvf lcov-$LCOV_VERSION.tar.gz
    cd lcov-$LCOV_VERSION
    make install
    cd ..
fi

if [ "$DRONE_JOB_BUILDTYPE" == "boost" ]; then

echo '==================================> INSTALL'

common_install

echo '==================================> SCRIPT'

if [ -n "$COMPILER" ] && [ -n "$B2_TOOLSET" ]; then
  echo "using $B2_TOOLSET : : $COMPILER ;" >> ~/user-config.jam
fi

. $BOOST_ROOT/libs/$SELF/ci/build.sh

elif [ "$DRONE_JOB_BUILDTYPE" == "boost_v1" ]; then

# version based on the earlier boost.beast .travis.yml configuration

echo '==================================> INSTALL'

export SELF=`basename $REPO_NAME`
export BEAST_RETRY=False
export TRAVIS=False

BOOST_BRANCH=develop
if [ "$DRONE_BRANCH" == "master" ]; then
  BOOST_BRANCH=master
fi
echo BOOST_BRANCH: $BOOST_BRANCH
cd ..
git clone -b $BOOST_BRANCH --depth 1 https://github.com/boostorg/boost.git boost-root
cd boost-root
export BOOST_ROOT=$(pwd)
export PATH=$PATH:$BOOST_ROOT
# git submodule update --init tools/boostdep
# python tools/boostdep/depinst/depinst.py --git_args "--jobs 3" $SELF
git submodule update --init --depth 20 --jobs 4
rm -rf libs/$SELF
cp -r $DRONE_BUILD_DIR libs/$SELF
./bootstrap.sh
cp libs/beast/tools/user-config.jam ~/user-config.jam
echo "using $TOOLSET : : $COMPILER : $CXX_FLAGS ;" >> ~/user-config.jam
# ./b2 -d0 headers

echo '==================================> SCRIPT'

cd $BOOST_ROOT
libs/beast/tools/retry.sh libs/beast/tools/build-and-test.sh

elif [ "$DRONE_JOB_BUILDTYPE" == "codecov" ]; then

echo '==================================> INSTALL'

common_install

echo '==================================> SCRIPT'

cd $BOOST_ROOT/libs/$SELF
ci/travis/codecov.sh

elif [ "$DRONE_JOB_BUILDTYPE" == "valgrind" ]; then

echo '==================================> INSTALL'

common_install

echo '==================================> SCRIPT'

cd $BOOST_ROOT/libs/$SELF
VALGRIND_OPTS="$VALGRIND_OPTS --suppressions=$BOOST_ROOT/libs/$SELF/tools/valgrind.supp"
echo "VALGRIND_OPTS is $VALGRIND_OPTS"
ci/travis/valgrind.sh

elif [ "$DRONE_JOB_BUILDTYPE" == "coverity" ]; then

echo '==================================> INSTALL'

common_install

echo '==================================> SCRIPT'

if  [ -n "${COVERITY_SCAN_NOTIFICATION_EMAIL}" -a \( "$TRAVIS_BRANCH" = "develop" -o "$TRAVIS_BRANCH" = "master" \) -a \( "$DRONE_BUILD_EVENT" = "push" -o "$DRONE_BUILD_EVENT" = "cron" \) ] ; then
cd $BOOST_ROOT/libs/$SELF
ci/travis/coverity.sh
fi

elif [ "$DRONE_JOB_BUILDTYPE" == "docs" ]; then

export BOOST_CI_SRC_FOLDER=$(pwd)

echo '==================================> INSTALL'
pwd
cd ..
mkdir -p $HOME/cache && cd $HOME/cache
if [ ! -d doxygen ]; then git clone -b 'Release_1_8_15' --depth 1 https://github.com/doxygen/doxygen.git && echo "not-cached" ; else echo "cached" ; fi
cd doxygen
cmake -H. -Bbuild -DCMAKE_BUILD_TYPE=Release
cd build
sudo make install
cd ../..
if [ ! -f saxonhe.zip ]; then wget -O saxonhe.zip https://sourceforge.net/projects/saxon/files/Saxon-HE/9.9/SaxonHE9-9-1-4J.zip/download && echo "not-cached" ; else echo "cached" ; fi
unzip -o saxonhe.zip
sudo rm /usr/share/java/Saxon-HE.jar
sudo cp saxon9he.jar /usr/share/java/Saxon-HE.jar
cd ..
BOOST_BRANCH=develop && [ "$DRONE_BRANCH" == "master" ] && BOOST_BRANCH=master || true
git clone -b $BOOST_BRANCH https://github.com/boostorg/boost.git boost-root --depth 1
cd boost-root
export BOOST_ROOT=$(pwd)
git submodule update --init libs/context
git submodule update --init tools/boostbook
git submodule update --init tools/boostdep
git submodule update --init tools/docca
git submodule update --init tools/quickbook
rsync -av $BOOST_CI_SRC_FOLDER/ libs/json
python tools/boostdep/depinst/depinst.py ../tools/quickbook
./bootstrap.sh
./b2 headers

echo '==================================> COMPILE'

echo "using doxygen ; using boostbook ; using saxonhe ;" > tools/build/src/user-config.jam
./b2 -j3 libs/json/doc//boostrelease

fi

