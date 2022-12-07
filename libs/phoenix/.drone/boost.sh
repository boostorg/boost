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

BOOST_BRANCH=develop && [ "$TRAVIS_BRANCH" == "master" ] && BOOST_BRANCH=master || true
cd ..
git clone --depth 1 -b $BOOST_BRANCH https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule init libs/algorithm
git submodule init libs/any
git submodule init libs/array
git submodule init libs/assert
git submodule init libs/assign
git submodule init libs/atomic
git submodule init libs/bind
git submodule init libs/chrono
git submodule init libs/circular_buffer
git submodule init libs/concept_check
git submodule init libs/config
git submodule init libs/container
git submodule init libs/container_hash
git submodule init libs/conversion
git submodule init libs/core
git submodule init libs/date_time
git submodule init libs/detail
git submodule init libs/endian
git submodule init libs/exception
git submodule init libs/filesystem
git submodule init libs/foreach
git submodule init libs/format
git submodule init libs/function
git submodule init libs/function_types
git submodule init libs/functional
git submodule init libs/fusion
git submodule init libs/integer
git submodule init libs/intrusive
git submodule init libs/io
git submodule init libs/iostreams
git submodule init libs/iterator
git submodule init libs/lambda
git submodule init libs/lexical_cast
git submodule init libs/locale
git submodule init libs/log
git submodule init libs/math
git submodule init libs/move
git submodule init libs/mp11
git submodule init libs/mpl
git submodule init libs/multi_index
git submodule init libs/numeric/conversion
git submodule init libs/optional
git submodule init libs/parameter
git submodule init libs/pool
git submodule init libs/predef
git submodule init libs/preprocessor
git submodule init libs/property_tree
git submodule init libs/proto
git submodule init libs/ptr_container
git submodule init libs/python
git submodule init libs/random
git submodule init libs/range
git submodule init libs/ratio
git submodule init libs/rational
git submodule init libs/regex
git submodule init libs/serialization
git submodule init libs/signals2
git submodule init libs/smart_ptr
git submodule init libs/spirit
git submodule init libs/static_assert
git submodule init libs/system
git submodule init libs/thread
git submodule init libs/throw_exception
git submodule init libs/tokenizer
git submodule init libs/tti
git submodule init libs/tuple
git submodule init libs/type_index
git submodule init libs/type_traits
git submodule init libs/typeof
git submodule init libs/unordered
git submodule init libs/utility
git submodule init libs/variant
git submodule init libs/winapi
git submodule init libs/xpressive
git submodule init libs/headers tools/boost_install tools/build
git submodule update --jobs 8
rm -rf libs/phoenix
cp -rp $TRAVIS_BUILD_DIR libs/phoenix
ln -s $(pwd)/libs/phoenix $TRAVIS_BUILD_DIR
./bootstrap.sh
./b2 headers
if [ $TRAVIS_OS_NAME = osx ]; then
  export PATH="/usr/local/opt/ccache/libexec:$PATH"
  true brew install ccache
fi

echo "using ${TOOLSET%%-*} : ${TOOLSET#*-} : ccache `echo $TOOLSET | sed 's/clang/clang++/;s/gcc/g++/'` ;" > ~/user-config.jam

echo '==================================> BEFORE_SCRIPT'

. $DRONE_BUILD_DIR/.drone/before-script.sh

echo '==================================> SCRIPT'

./b2 libs/phoenix/test toolset=$TOOLSET cxxstd=$CXXSTD ${SANITIZED+cxxflags=-fsanitize=address,undefined} ${SANITIZED+'linkflags=-fsanitize=address,undefined -fuse-ld=gold'}

echo '==================================> AFTER_SUCCESS'

. $DRONE_BUILD_DIR/.drone/after-success.sh
