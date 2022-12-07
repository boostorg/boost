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
export USER=$(whoami)
export CC=${CC:-gcc}
export PATH=~/.local/bin:/usr/local/bin:$PATH
export BOOST_ROOT="$HOME/boost"
export BOOST_BUILD_PATH="$HOME/build-boost"
export TRAVIS_PULL_REQUEST=${DRONE_PULL_REQUEST:-false}
export TRAVIS_REPO_SLUG=$REPO_NAME

if [ "$DRONE_JOB_BUILDTYPE" == "boost" ]; then

echo '==================================> INSTALL'

export CACHE_NAME=$TRAVIS_OS_NAME-$TOOLSET-$STD-$JOB
export PATH=$BOOST_ROOT:$PATH
if [[ "$TRAVIS_COMPILER" =~ ^clang- ]]; then export STDLIB=stdlib=libc++ ; fi
# Creating ~/user-config.jam file
sed 's/^  //' > ~/user-config.jam << 'EOF'

import feature ;
import os ;
import regex ;
import toolset ;


local TOOLSET = [ os.environ TRAVIS_COMPILER ] ;
local toolset-parts = [ regex.split $(TOOLSET) "-" ] ;
local toolset-name = $(toolset-parts[1]) ;
local toolset-feature = $(toolset-parts[2-]:J="-") ;

local cxx ;
switch $(toolset-name) {
    case gcc   : cxx ?= [ regex.replace $(TOOLSET) "gcc" "g++" ] ;
    case clang : cxx ?= [ regex.replace $(TOOLSET) "clang" "clang++" ] ;
    case *     : EXIT "user-config: Unsupported toolset $(toolset-name)" ;
}

using $(toolset-name) : $(toolset-feature) : ccache $(cxx) ;

# Release variant with enabled asserts
variant sanitize : <optimization>speed <debug-symbols>off <inlining>full
                   <runtime-debugging>off ;

# Determining the root branch
if [[ "$TRAVIS_PULL_REQUEST" == "false" ]]; then
  export BRANCH=$TRAVIS_BRANCH
else
  # It is a pull request. Retrieve the base branch from GitHub
  GH_PR_API=https://api.github.com/repos/$TRAVIS_REPO_SLUG/pulls/$TRAVIS_PULL_REQUEST
  export BRANCH=`curl -s $GH_PR_API | jq -r .head.ref`;
fi
if [[ ! "$BRANCH" =~ ^(master|develop)$ ]]; then
  # Travis has been triggered not from our main branches.
  # Find out the base branch from the git history
  # TODO: Not implemented yet, but in most cases it will be develop branch
  export BRANCH=develop
fi
echo Root branch is $BRANCH

env
sed 's/--depth=1/--depth=9/g' `which git` > ~/git && chmod +x ~/git
~/git clone -j10 --branch=$BRANCH --depth=1 --quiet --recurse-submodules=":(exclude)$PROJECT" --shallow-submodules https://github.com/boostorg/boost.git $BOOST_ROOT
pushd $BOOST_ROOT
rm -rf $PROJECT
./bootstrap.sh --with-toolset=clang
./b2 headers
cp -rp $TRAVIS_BUILD_DIR $PROJECT
ln -s $PROJECT $TRAVIS_BUILD_DIR
cd $PROJECT
cd $JOB

echo '==================================> SCRIPT'

b2 link=shared threading=multi variant=release,sanitize toolset=$TRAVIS_COMPILER cxxstd=$STD $STDLIB warnings=extra warnings-as-errors=on

fi
