#!/usr/bin/env bash
# Boost.GIL proxy for b2 command on Travis CI (can run locally too)
set -eu

if [ ! -z ${TRAVIS+x} ] && [ "$TRAVIS" == "true" ] && [ ! -z ${DOC+x} ]; then
    echo "Documentation build requested, skipping tests build"
    exit 0
fi

if [ -z ${TOOLSET+x} ]; then
    echo "Missing environment TOOLSET"
    exit 1
fi

if [ -z ${VARIANT+x} ]; then
    echo "Missing environment VARIANT"
    exit 1
fi

if [ -z ${CXXSTD+x} ]; then
    CXXSTD=11
fi

if [ -z ${B2_OPTIONS+x} ]; then
    B2_OPTIONS=""
fi

if [ ! -z ${TRAVIS+x} ] && [ "$TRAVIS" == "true" ]; then
  JOBS="2"
elif [[ $(uname -s) == "Linux" ]]; then
  JOBS=$(lscpu -p | grep -v '^#' | sort -u -t, -k 2,4 | wc -l)
elif [[ $(uname) == "Darwin" ]]; then
  JOBS=$(sysctl -n hw.physicalcpu)
else
  JOBS=1
fi

echo "Running ./b2 -j $JOBS $B2_OPTIONS toolset=$TOOLSET variant=$VARIANT"

set -euv
./b2 -j $JOBS $B2_OPTIONS toolset=$TOOLSET variant=$VARIANT cxxstd=$CXXSTD libs/gil/test/core
./b2 -j $JOBS $B2_OPTIONS toolset=$TOOLSET variant=$VARIANT cxxstd=$CXXSTD libs/gil/test/extension/numeric
./b2 -j $JOBS $B2_OPTIONS toolset=$TOOLSET variant=$VARIANT cxxstd=$CXXSTD libs/gil/test/extension/toolbox
./b2 -j $JOBS $B2_OPTIONS toolset=$TOOLSET variant=$VARIANT cxxstd=$CXXSTD libs/gil/test/extension/io//simple
