#!/usr/bin/env bash

set -e

# cxxstd=2a,17,14,11 \

./b2 --user-config=./user-config.jam \
  toolset=clang,gcc \
  asio.mode=dflt,nodep,nots,ts,nodep-nots,nodep-ts \
  variant=release \
  cxxstd=17,14,11 \
  -j`grep processor /proc/cpuinfo | wc -l` \
  -q \
  libs/beast/test libs/beast/example
