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


CC=$CXX

export CHECKOUT_PATH=`pwd`;
if [ -n "$GCC_VERSION" ]; then export CXX="g++-${GCC_VERSION}" CC="gcc-${GCC_VERSION}"; fi
if [ -n "$CLANG_VERSION" ]; then export CXX="clang++-${CLANG_VERSION}" CC="clang-${CLANG_VERSION}"; fi
export DEPS_DIR="${TRAVIS_BUILD_DIR}/deps"
mkdir ${DEPS_DIR} && cd ${DEPS_DIR}
mkdir usr
export PATH=${DEPS_DIR}/usr/bin:${PATH}
CMAKE_URL="http://www.cmake.org/files/v3.3/cmake-3.3.1-Linux-x86_64.tar.gz"
if [[ "$LIBCXX" == "on" ]]; then wget --no-check-certificate --quiet -O - ${CMAKE_URL} | tar --strip-components=1 -xz -C usr; fi

$CXX --version || true
which $CXX || true
$CC --version || true
which $CC || true
which cmake || true
cmake --version || true
if [[ "$LIBCXX" == "on" ]]; then
  if [[ "${CLANG_VERSION}" == "3.5" ]]; then LLVM_VERSION="3.5.2"; fi
  if [[ "${CLANG_VERSION}" == "3.6" ]]; then LLVM_VERSION="3.6.2"; fi
  if [[ "${CLANG_VERSION}" == "3.7" ]]; then LLVM_VERSION="3.7.0"; fi
  if [[ "${CLANG_VERSION}" == "3.8" ]]; then LLVM_VERSION="3.8.1"; fi
  LLVM_URL="http://llvm.org/releases/${LLVM_VERSION}/llvm-${LLVM_VERSION}.src.tar.xz"
  LIBCXX_URL="http://llvm.org/releases/${LLVM_VERSION}/libcxx-${LLVM_VERSION}.src.tar.xz"
  LIBCXXABI_URL="http://llvm.org/releases/${LLVM_VERSION}/libcxxabi-${LLVM_VERSION}.src.tar.xz"

  mkdir -p llvm llvm/build llvm/projects/libcxx llvm/projects/libcxxabi
  wget --quiet -O - ${LLVM_URL} | tar --strip-components=1 -x --xz -C llvm
  wget --quiet -O - ${LIBCXX_URL} | tar --strip-components=1 -x --xz -C llvm/projects/libcxx
  wget --quiet -O - ${LIBCXXABI_URL} | tar --strip-components=1 -x --xz -C llvm/projects/libcxxabi
  (cd llvm/build && cmake .. -DCMAKE_INSTALL_PREFIX=${DEPS_DIR}/usr)
  (cd llvm/build/projects/libcxx && make install -j2)
  (cd llvm/build/projects/libcxxabi && make install -j2)
  export LDFLAGS="-L${DEPS_DIR}/usr/lib -lc++ -lc++abi"
  export CFLAGS="${CFLAGS} -stdlib=libc++ -I${DEPS_DIR}/usr/include/c++/v1"
  export CXXFLAGS="${CXXFLAGS} -stdlib=libc++ -I${DEPS_DIR}/usr/include/c++/v1"
  export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${DEPS_DIR}/usr/lib"
fi

ls -lR ${DEPS_DIR}/usr
export CXXFLAGS="${CXXFLAGS} -Wall -Wextra -Werror"
if [ -n "$CLANG_VERSION" ]; then export CXXFLAGS="${CXXFLAGS} -Wnewline-eof -Wabi -Wabstract-final-class -Waddress -Waddress-of-array-temporary -Waddress-of-temporary -Waggregate-return -Wambiguous-macro -Wambiguous-member-template -Wanonymous-pack-parens -Warray-bounds -Warray-bounds-pointer-arithmetic -Wassign-enum -Watomic-properties -Wattributes -Wavailability -Wbackslash-newline-escape -Wbad-array-new-length -Wbad-function-cast -Wbind-to-temporary-copy -Wbuiltin-macro-redefined -Wbuiltin-requires-header -Wcast-align -Wcast-qual -Wchar-align -Wcomments -Wcompare-distinct-pointer-types -Wconditional-type-mismatch -Wconditional-uninitialized -Wconfig-macros -Wconstant-logical-operand -Wconstexpr-not-const -Wconversion-null -Wcovered-switch-default -Wctor-dtor-privacy -Wdangling-field -Wdangling-initializer-list -Wdelete-incomplete -Wdeprecated -Wdivision-by-zero -Wduplicate-decl-specifier -Wduplicate-enum -Wduplicate-method-arg -Wduplicate-method-match -Wdynamic-class-memaccess -Wempty-body -Wendif-labels -Wenum-compare -Wexplicit-ownership-type -Wextern-initializer -Wfloat-equal -Wgnu-array-member-paren-init -Wheader-guard -Wheader-hygiene -Widiomatic-parentheses -Wignored-attributes -Wimplicit-conversion-floating-point-to-bool -Wimplicit-exception-spec-mismatch -Wimplicit-fallthrough -Wincompatible-library-redeclaration -Wincompatible-pointer-types -Winherited-variadic-ctor -Winit-self -Winline -Wint-conversions -Wint-to-pointer-cast -Winteger-overflow -Winvalid-constexpr -Winvalid-noreturn -Winvalid-offsetof -Winvalid-pch -Winvalid-pp-token -Winvalid-source-encoding -Winvalid-token-paste -Wloop-analysis -Wmain -Wmain-return-type -Wmalformed-warning-check -Wmethod-signatures -Wmismatched-parameter-types -Wmismatched-return-types -Wmissing-declarations -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-sysroot -Wmissing-variable-declarations -Wnarrowing -Wnested-externs -Wsign-compare -Wnon-pod-varargs -Wnon-virtual-dtor -Wnull-arithmetic -Wnull-character -Wnull-dereference -Wodr -Wold-style-definition -Wout-of-line-declaration -Wover-aligned -Woverflow -Woverriding-method-mismatch -Wpacked -Wpointer-sign -Wpointer-to-int-cast -Wpointer-type-mismatch -Wpredefined-identifier-outside-function -Wredundant-decls -Wreinterpret-base-class -Wreserved-user-defined-literal -Wreturn-stack-address -Wsection -Wsequence-point -Wserialized-diagnostics -Wshadow -Wshift-count-negative -Wshift-count-overflow -Wshift-overflow -Wshift-sign-overflow -Wsign-promo -Wsizeof-pointer-memaccess -Wstack-protector -Wstatic-float-init -Wstring-compare -Wstrlcpy-strlcat-size -Wstrncat-size -Wswitch-default -Wswitch-enum -Wsynth -Wtautological-compare -Wtentative-definition-incomplete-type -Wthread-safety -Wtype-limits -Wtype-safety -Wtypedef-redefinition -Wtypename-missing -Wundef -Wundefined-inline -Wundefined-reinterpret-cast -Wunicode -Wunicode-whitespace -Wunreachable-code -Wunused-exception-parameter -Wunused-macros -Wunused-member-function -Wunused-volatile-lvalue -Wused-but-marked-unused -Wuser-defined-literals -Wvarargs -Wvector-conversions -Wvexing-parse -Wvisibility -Wvla -Wweak-template-vtables -Wweak-vtables -Wwrite-strings"; fi

echo '==================================> SCRIPT'

export INSTALL_PREFIX_PATH="${TRAVIS_BUILD_DIR}/usr"
mkdir -p "$INSTALL_PREFIX_PATH"
export PKG_CONFIG_PATH="$INSTALL_PREFIX_PATH/lib/pkgconfig"
cd $CHECKOUT_PATH
export ASANFLAGS="-fsanitize=address,undefined,integer -fno-omit-frame-pointer -fno-sanitize=unsigned-integer-overflow"
if [[ "$CXX11" == "on" ]]; then export CXXFLAGS="${CXXFLAGS} -std=c++11"; fi
export ASANVARIANT="off"
if [[ "$ASAN" == "on" ]]; then export ASANVARIANT="${ASANVARIANT} on"; fi
for build_type in debug minsizerel release; do
  for asan_type in $ASANVARIANT; do
    build_dir="build-$build_type-asan-$asan_type"
    mkdir $build_dir
    cd $build_dir
    if [[ "$asan_type" == "on" ]]; then 
      CXXFLAGS="$CXXFLAGS $ASANFLAGS" cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DBUILD_EXAMPLES=$BUILD_EXAMPLES -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX_PATH ..
    else
      cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DBUILD_EXAMPLES=$BUILD_EXAMPLES -DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX_PATH ..
    fi
    CTEST_OUTPUT_ON_FAILURE=1 CTEST_PARALLEL_LEVEL=4 make -j4 check
    if [ $? -ne 0 ]
    then
      exit 1
    fi
    if [[ "$build_type" == "release" ]]; then make install; fi
    cd ..
    rm -rf $build_dir
  done
done

pkg-config boost_hof --cflags
cd include
HEADERS=$(echo boost/hof.hpp boost/hof/*.hpp)
cd ..
for file in $HEADERS; do echo -e "#include <$file>\nint main() {}" | $CXX -std=gnu++0x `pkg-config boost_hof --cflags` -x c++ -S -; done
rm -rf "$INSTALL_PREFIX_PATH"

fi
