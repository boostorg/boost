#!/bin/bash

# Copyright 2020 Rene Rivera, Sam Darwin
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at http://boost.org/LICENSE_1_0.txt)

set -e
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

if [[ ${TRAVIS_OS_NAME} =~ [Ll]inux ]]; then
  echo Installing locales for ${TRAVIS_OS_NAME} ...
  sudo /usr/sbin/locale-gen fr_FR
  sudo /usr/sbin/locale-gen en_GB
  sudo locale -a
  echo ...done with locales
fi

if [ "$DRONE_JOB_BUILDTYPE" == "boost" ]; then

echo '==================================> INSTALL'

git clone https://github.com/boostorg/boost-ci.git boost-ci-cloned
cp -prf boost-ci-cloned/ci .
rm -rf boost-ci-cloned

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
    unset -f cd
fi

export SELF=`basename $REPO_NAME`
export BOOST_CI_TARGET_BRANCH="$TRAVIS_BRANCH"
export BOOST_CI_SRC_FOLDER=$(pwd)

. ./ci/common_install.sh

echo '==================================> SCRIPT'

$BOOST_ROOT/libs/$SELF/ci/travis/build.sh

elif [ "$DRONE_JOB_BUILDTYPE" == "fc4716b68c-0fcaf592f9" ]; then

echo '==================================> INSTALL'

git clone https://github.com/boostorg/boost-ci.git boost-ci-cloned
cp -prf boost-ci-cloned/ci .
rm -rf boost-ci-cloned

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
    unset -f cd
fi

export SELF=`basename $REPO_NAME`
export BOOST_CI_TARGET_BRANCH="$TRAVIS_BRANCH"
export BOOST_CI_SRC_FOLDER=$(pwd)

. ./ci/common_install.sh

echo '==================================> SCRIPT'

cd $BOOST_ROOT/libs/$SELF
ci/travis/codecov.sh

elif [ "$DRONE_JOB_BUILDTYPE" == "fc4716b68c-db180b7bd2" ]; then

echo '==================================> INSTALL'

git clone https://github.com/boostorg/boost-ci.git boost-ci-cloned
cp -prf boost-ci-cloned/ci .
rm -rf boost-ci-cloned

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
    unset -f cd
fi

export SELF=`basename $REPO_NAME`
export BOOST_CI_TARGET_BRANCH="$TRAVIS_BRANCH"
export BOOST_CI_SRC_FOLDER=$(pwd)

. ./ci/common_install.sh

echo '==================================> SCRIPT'

cd $BOOST_ROOT/libs/$SELF
ci/travis/valgrind.sh

elif [ "$DRONE_JOB_BUILDTYPE" == "fc4716b68c-cce9827eb5" ]; then

echo '==================================> INSTALL'

git clone https://github.com/boostorg/boost-ci.git boost-ci-cloned
cp -prf boost-ci-cloned/ci .
rm -rf boost-ci-cloned

if [ "$TRAVIS_OS_NAME" == "osx" ]; then
    unset -f cd
fi

export SELF=`basename $REPO_NAME`
export BOOST_CI_TARGET_BRANCH="$TRAVIS_BRANCH"
export BOOST_CI_SRC_FOLDER=$(pwd)

. ./ci/common_install.sh

echo '==================================> SCRIPT'

if  [ -n "${COVERITY_SCAN_NOTIFICATION_EMAIL}" -a \( "$TRAVIS_BRANCH" = "develop" -o "$TRAVIS_BRANCH" = "master" \) -a \( "$DRONE_BUILD_EVENT" = "push" -o "$DRONE_BUILD_EVENT" = "cron" \) ] ; then
    cd $BOOST_ROOT/libs/$SELF
    ci/travis/coverity.sh
fi

fi
