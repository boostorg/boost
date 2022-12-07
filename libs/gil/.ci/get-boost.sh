#!/bin/sh
# Downloads Boost superproject from given Git branch
# with all submodules required by Boost.GIL.
set -e
echo "get-boost: Downloading Boost superproject from Git"

BOOST_BRANCH="master"
if [ "$1" != "master" ]; then
    BOOST_BRANCH="develop"
fi
BUILD_DIR=$2

echo "BOOST_BRANCH=$BOOST_BRANCH"
echo "BUILD_DIR=$BUILD_DIR"

# Some (e.g. CircleCI) may have ancient version of Git installed.
GIT_VERSION_MAJOR=$(git version | sed 's/git version //g' | cut -d. -f1)
GIT_VERSION_MINOR=$(git version | sed 's/git version //g' | cut -d. -f2)
if test $GIT_VERSION_MAJOR -gt 2 || test $GIT_VERSION_MAJOR -eq 2 -a $GIT_VERSION_MINOR -gt 8; then
    GIT_SUBMODULE_OPTS="--jobs 4"
fi

# Use a reasonably large depth to prevent intermittent update failures due to
# commits being on a submodule's master before the superproject is updated.
git clone -b $BOOST_BRANCH --depth 10 https://github.com/boostorg/boost.git boost-root
cd boost-root

# List all (recursive) dependencies explicitly to control any new additions
# Direct (of GIL headers only)
git submodule --quiet update --init $GIT_SUBMODULE_OPTS \
    tools/build \
    tools/boost_install \
    tools/boostdep \
    libs/headers \
    libs/algorithm \
    libs/array \
    libs/assert \
    libs/concept_check \
    libs/config \
    libs/core \
    libs/crc \
    libs/filesystem \
    libs/integer \
    libs/iterator \
    libs/mp11 \
    libs/mpl \
    libs/preprocessor \
    libs/type_traits \
    libs/variant2
# Transitive (of GIL tests too)
git submodule --quiet update --init $GIT_SUBMODULE_OPTS \
    libs/align \
    libs/atomic \
    libs/bind \
    libs/chrono \
    libs/container \
    libs/container_hash \
    libs/conversion \
    libs/detail \
    libs/exception \
    libs/function \
    libs/function_types \
    libs/fusion \
    libs/intrusive \
    libs/io \
    libs/lambda \
    libs/math \
    libs/move \
    libs/optional \
    libs/predef \
    libs/range \
    libs/ratio \
    libs/rational \
    libs/regex \
    libs/static_assert \
    libs/smart_ptr \
    libs/system \
    libs/throw_exception \
    libs/timer \
    libs/tuple \
    libs/type_index \
    libs/typeof \
    libs/unordered \
    libs/utility \
    libs/winapi

if [ -d ./.git ]; then
    echo "get-boost: git log --stat -1"
    git --no-pager log --stat -1
fi

echo "get-boost: Deleting $PWD/libs/gil cloned with Boost superproject"
rm -rf libs/gil

echo "get-boost: Copying $BUILD_DIR to $PWD/libs/gil"
cp -r $BUILD_DIR libs/gil

if [ -d libs/gil/.git ]; then
    current_pwd=`pwd`
    cd libs/gil
    echo "get-boost: git log --stat -1"
    git --no-pager log --stat -1
    cd $current_pwd
fi

echo "get-boost: Done"
