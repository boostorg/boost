#!/bin/bash

# Copyright 2020 Rene Rivera, Sam Darwin
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt)

set -xe

export TRAVIS_BUILD_DIR=$(pwd)
export DRONE_BUILD_DIR=$(pwd)
export TRAVIS_BRANCH=$DRONE_BRANCH
export TRAVIS_EVENT_TYPE=$DRONE_BUILD_EVENT
export VCS_COMMIT_ID=$DRONE_COMMIT
export GIT_COMMIT=$DRONE_COMMIT
export REPO_NAME=$DRONE_REPO
export USER=$(whoami)
export CC=${CC:-gcc}
export PATH=~/.local/bin:/usr/local/bin:$PATH

common_install () {
  git clone https://github.com/boostorg/boost-ci.git boost-ci-cloned --depth 1
  cp -prf boost-ci-cloned/ci .
  rm -rf boost-ci-cloned

  if [ "$TRAVIS_OS_NAME" == "osx" ]; then
      unset -f cd
  fi

  export SELF=`basename $REPO_NAME`
  export BOOST_CI_TARGET_BRANCH="$TRAVIS_BRANCH"
  export BOOST_CI_SRC_FOLDER=$(pwd)

  . ./ci/common_install.sh
}

if [ "$DRONE_JOB_BUILDTYPE" == "boost" ]; then

echo '==================================> INSTALL'

common_install

echo '==================================> SCRIPT'

$BOOST_ROOT/libs/$SELF/ci/travis/build.sh

elif [ "$DRONE_JOB_BUILDTYPE" == "docs" ]; then

echo '==================================> INSTALL'

export SELF=`basename $REPO_NAME`

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
BOOST_BRANCH=develop && [ "$TRAVIS_BRANCH" == "master" ] && BOOST_BRANCH=master || true
git clone -b $BOOST_BRANCH https://github.com/boostorg/boost.git boost-root --depth 1
cd boost-root
export BOOST_ROOT=$(pwd)
git submodule update --init libs/context
git submodule update --init tools/boostbook
git submodule update --init tools/boostdep
git submodule update --init tools/docca
git submodule update --init tools/quickbook
rsync -av $TRAVIS_BUILD_DIR/ libs/$SELF
python tools/boostdep/depinst/depinst.py ../tools/quickbook
./bootstrap.sh
./b2 headers

echo '==================================> SCRIPT'

echo "using doxygen ; using boostbook ; using saxonhe ;" > tools/build/src/user-config.jam
./b2 -j3 libs/$SELF/doc//boostrelease

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
ci/travis/valgrind.sh

elif [ "$DRONE_JOB_BUILDTYPE" == "standalone" ]; then

echo '==================================> INSTALL'

# Installing cmake with apt-get, so not required here:
# pip install --user cmake

echo '==================================> SCRIPT'

export CXXFLAGS="-Wall -Wextra -Werror -std=c++17"
mkdir __build_17
cd __build_17
cmake -DBOOST_JSON_STANDALONE=1 ..
cmake --build .
ctest -V .
export CXXFLAGS="-Wall -Wextra -Werror -std=c++2a"
mkdir ../__build_2a
cd ../__build_2a
cmake -DBOOST_JSON_STANDALONE=1 ..
cmake --build .
ctest -V .

elif [ "$DRONE_JOB_BUILDTYPE" == "coverity" ]; then

echo '==================================> INSTALL'

common_install

echo '==================================> SCRIPT'

if [ $VARIANT = "process_valgrind" ];
then export USE_VALGRIND="testing.launcher=valgrind valgrind=on";
fi ;

if  [ -n "${COVERITY_SCAN_NOTIFICATION_EMAIL}" -a \( "$TRAVIS_BRANCH" = "develop" -o "$TRAVIS_BRANCH" = "master" \) -a \( "$DRONE_BUILD_EVENT" = "push" -o "$DRONE_BUILD_EVENT" = "cron" \) ] ; then
cd $BOOST_ROOT/libs/$SELF
ci/travis/coverity.sh
fi

elif [ "$DRONE_JOB_BUILDTYPE" == "cmake-superproject" ]; then

echo '==================================> INSTALL'

common_install

echo '==================================> COMPILE'

export CXXFLAGS="-Wall -Wextra -Werror"

mkdir __build_static
cd __build_static
cmake -DBOOST_ENABLE_CMAKE=1 -DBUILD_TESTING=ON -DBoost_VERBOSE=1 \
    -DBOOST_INCLUDE_LIBRARIES=$SELF ..
cmake --build .
ctest --output-on-failure -R boost_$SELF

cd ..

mkdir __build_shared
cd __build_shared
cmake -DBOOST_ENABLE_CMAKE=1 -DBUILD_TESTING=ON -DBoost_VERBOSE=1 \
    -DBOOST_INCLUDE_LIBRARIES=$SELF -DBUILD_SHARED_LIBS=ON ..
cmake --build .
ctest --output-on-failure -R boost_$SELF

elif [ "$DRONE_JOB_BUILDTYPE" == "cmake1" ]; then

echo '==================================> INSTALL'

pip install --user cmake

echo '==================================> SCRIPT'

export SELF=`basename $REPO_NAME`
BOOST_BRANCH=develop && [ "$DRONE_BRANCH" == "master" ] && BOOST_BRANCH=master || true
echo BOOST_BRANCH: $BOOST_BRANCH
cd ..
git clone -b $BOOST_BRANCH --depth 1 https://github.com/boostorg/boost.git boost-root
cd boost-root
mkdir -p libs/$SELF
cp -r $DRONE_BUILD_DIR/* libs/$SELF
# git submodule update --init tools/boostdep
git submodule update --init --recursive

cd libs/$SELF

../../../b2 -sBOOST_BUILD_PATH=.
../../../b2 $MULTITHREAD with-valgrind address-model=64 architecture=x86 $USE_VALGRIND toolset=$TOOLSET cxxflags="--coverage -DBOOST_TRAVISCI_BUILD -std=$CXX_STANDARD" linkflags="--coverage" -sBOOST_BUILD_PATH=. $REPORT_CI
../../../b2 $MULTITHREAD without-valgrind address-model=64 architecture=x86 toolset=$TOOLSET cxxflags="--coverage -DBOOST_TRAVISCI_BUILD -std=$CXX_STANDARD" linkflags="--coverage" -sBOOST_BUILD_PATH=. $REPORT_CI



fi