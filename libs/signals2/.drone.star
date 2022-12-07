# Use, modification, and distribution are
# subject to the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt)
#
# Copyright Rene Rivera 2020.

# For Drone CI we use the Starlark scripting language to reduce duplication.
# As the yaml syntax for Drone CI is rather limited.
#
#
globalenv={'B2_VARIANT': 'variant=release,debug'}
linuxglobalimage="cppalliance/droneubuntu1604:1"
windowsglobalimage="cppalliance/dronevs2019"

def main(ctx):
  return [
  linux_cxx("B2_TOOLSET=gcc-4.8 B2_CXXSTD=03,11 Job 0", "g++-4.8", packages="g++-4.8", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'B2_TOOLSET': 'gcc-4.8', 'B2_CXXSTD': '03,11', 'DRONE_JOB_UUID': 'b6589fc6ab'}, globalenv=globalenv),
  linux_cxx("B2_TOOLSET=gcc-4.9 B2_CXXSTD=03,11 Job 1", "g++-4.9", packages="g++-4.9", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'B2_TOOLSET': 'gcc-4.9', 'B2_CXXSTD': '03,11', 'DRONE_JOB_UUID': '356a192b79'}, globalenv=globalenv),
  linux_cxx("B2_TOOLSET=gcc-5 B2_CXXSTD=11 Job 2", "g++-5", packages="g++-5", buildtype="boost", image=linuxglobalimage, environment={'B2_TOOLSET': 'gcc-5', 'B2_CXXSTD': '11', 'DRONE_JOB_UUID': 'da4b9237ba'}, globalenv=globalenv),
  linux_cxx("B2_TOOLSET=gcc-6 B2_CXXSTD=11,14 Job 3", "g++-6", packages="g++-6", buildtype="boost", image=linuxglobalimage, environment={'B2_TOOLSET': 'gcc-6', 'B2_CXXSTD': '11,14', 'DRONE_JOB_UUID': '77de68daec'}, globalenv=globalenv),
  linux_cxx("B2_TOOLSET=gcc-7 B2_CXXSTD=11,14,17 Job 4", "g++-7", packages="g++-7", buildtype="boost", image=linuxglobalimage, environment={'B2_TOOLSET': 'gcc-7', 'B2_CXXSTD': '11,14,17', 'DRONE_JOB_UUID': '1b64538924'}, globalenv=globalenv),
  linux_cxx("B2_TOOLSET=gcc-8 B2_CXXSTD=14,17,2a Job 5", "g++-8", packages="g++-8", buildtype="boost", image=linuxglobalimage, environment={'B2_TOOLSET': 'gcc-8', 'B2_CXXSTD': '14,17,2a', 'DRONE_JOB_UUID': 'ac3478d69a'}, globalenv=globalenv),
  linux_cxx("B2_TOOLSET=clang-3.8 B2_CXXSTD=03,11,14 Job 6", "clang++-3.8", packages="clang-3.8 libstdc++-6-dev", llvm_os="xenial", llvm_ver="3.8", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'B2_TOOLSET': 'clang-3.8', 'B2_CXXSTD': '03,11,14', 'DRONE_JOB_UUID': 'c1dfd96eea'}, globalenv=globalenv),
  linux_cxx("B2_TOOLSET=clang-4.0 B2_CXXSTD=11,14 Job 7", "clang++-4.0", packages="clang-4.0 libstdc++-6-dev", llvm_os="xenial", llvm_ver="4.0", buildtype="boost", image=linuxglobalimage, environment={'B2_TOOLSET': 'clang-4.0', 'B2_CXXSTD': '11,14', 'DRONE_JOB_UUID': '902ba3cda1'}, globalenv=globalenv),
  linux_cxx("B2_TOOLSET=clang-5.0 B2_CXXSTD=11,14,17 Job 8", "clang++-5.0", packages="clang-5.0 libstdc++-7-dev", llvm_os="xenial", llvm_ver="5.0", buildtype="boost", image=linuxglobalimage, environment={'B2_TOOLSET': 'clang-5.0', 'B2_CXXSTD': '11,14,17', 'DRONE_JOB_UUID': 'fe5dbbcea5'}, globalenv=globalenv),
  linux_cxx("B2_TOOLSET=clang-6.0 B2_CXXSTD=14,17,2a Job 9", "clang++-6.0", packages="clang-6.0 libc6-dbg libc++-dev libstdc++-8-dev", llvm_os="xenial", llvm_ver="6.0", buildtype="boost", image=linuxglobalimage, environment={'B2_TOOLSET': 'clang-6.0', 'B2_CXXSTD': '14,17,2a', 'DRONE_JOB_UUID': '0ade7c2cf9'}, globalenv=globalenv),
  linux_cxx("B2_TOOLSET=clang-7 B2_CXXSTD=14,17,2a Job 10", "clang++-7", packages="clang-7 libc6-dbg libc++-dev libstdc++-8-dev", llvm_os="xenial", llvm_ver="7", buildtype="boost", image=linuxglobalimage, environment={'B2_TOOLSET': 'clang-7', 'B2_CXXSTD': '14,17,2a', 'DRONE_JOB_UUID': 'b1d5781111'}, globalenv=globalenv),
  linux_cxx("B2_TOOLSET=clang-8 B2_CXXSTD=14,17,2a Job 11", "clang++-8", packages="clang-8 libc6-dbg libc++-dev libstdc++-8-dev", llvm_os="xenial", llvm_ver="8", buildtype="boost", image=linuxglobalimage, environment={'B2_TOOLSET': 'clang-8', 'B2_CXXSTD': '14,17,2a', 'DRONE_JOB_UUID': '17ba079149'}, globalenv=globalenv),
  linux_cxx("B2_TOOLSET=clang-6.0 B2_CXXSTD=03,11,14,17,2a Job 12", "clang++-6.0", packages="clang-6.0 libc6-dbg libc++-dev libstdc++-8-dev", llvm_os="xenial", llvm_ver="6.0", buildtype="boost", image=linuxglobalimage, environment={'B2_TOOLSET': 'clang-6.0', 'B2_CXXSTD': '03,11,14,17,2a', 'B2_CXXFLAGS': '-stdlib=libc++', 'DRONE_JOB_UUID': '7b52009b64'}, globalenv=globalenv),
  osx_cxx("B2_TOOLSET=clang B2_CXXSTD=03,11,17 Job 13", "g++", packages="", buildtype="boost", environment={'B2_TOOLSET': 'clang', 'B2_CXXSTD': '03,11,17', 'DRONE_JOB_UUID': 'bd307a3ec3'}, globalenv=globalenv),
  linux_cxx("CMAKE_TEST=True Job 14", "g++", packages="binutils-gold gdb libc6-dbg", buildtype="b5847f804b-a16f359ef1", image=linuxglobalimage, environment={'CMAKE_TEST': 'True', 'DRONE_JOB_UUID': 'fa35e19212'}, globalenv=globalenv),
  linux_cxx("COMMENT=codecov.io B2_CXXSTD=03,11 B2_TOOLSET Job 15", "g++-8", packages="g++-8", buildtype="b5847f804b-bbb3de2b00", image=linuxglobalimage, environment={'COMMENT': 'codecov.io', 'B2_CXXSTD': '03,11', 'B2_TOOLSET': 'gcc-8', 'B2_DEFINES': 'define=BOOST_NO_STRESS_TEST=1', 'DRONE_JOB_UUID': 'f1abd67035'}, globalenv=globalenv),
  linux_cxx("COMMENT=ubsan B2_VARIANT=variant=debug B2_TOO Job 16", "g++-8", packages="g++-8", buildtype="boost", image=linuxglobalimage, environment={'COMMENT': 'ubsan', 'B2_VARIANT': 'variant=debug', 'B2_TOOLSET': 'gcc-8', 'B2_CXXSTD': '03,11,14,17,2a', 'B2_DEFINES': 'define=BOOST_NO_STRESS_TEST=1', 'B2_CXXFLAGS': 'cxxflags=-fno-omit-frame-pointer cxxflags=-fsanitize=undefined cxxflags=-fno-sanitize-recover=all', 'B2_LINKFLAGS': 'linkflags=-fsanitize=undefined linkflags=-fno-sanitize-recover=all linkflags=-fuse-ld=gold', 'UBSAN_OPTIONS': 'print_stacktrace=1', 'DRONE_JOB_UUID': '1574bddb75'}, globalenv=globalenv),
  linux_cxx("COMMENT=valgrind B2_TOOLSET=clang-6.0 B2_CXXS Job 17", "clang++-6.0", packages="clang-6.0 libc6-dbg libc++-dev libstdc++-8-dev", llvm_os="xenial", llvm_ver="6.0", buildtype="b5847f804b-db180b7bd2", image=linuxglobalimage, environment={'COMMENT': 'valgrind', 'B2_TOOLSET': 'clang-6.0', 'B2_CXXSTD': '03,11,14,17,2a', 'B2_DEFINES': 'define=BOOST_NO_STRESS_TEST=1', 'B2_VARIANT': 'variant=debug', 'B2_TESTFLAGS': 'testing.launcher=valgrind', 'VALGRIND_OPTS': '--error-exitcode=1', 'DRONE_JOB_UUID': '0716d9708d'}, globalenv=globalenv),
  linux_cxx("COMMENT=Coverity Scan B2_TOOLSET=clang Job 18", "g++", packages="binutils-gold gdb libc6-dbg", buildtype="b5847f804b-cce9827eb5", image=linuxglobalimage, environment={'COMMENT': 'Coverity Scan', 'B2_TOOLSET': 'clang', 'DRONE_JOB_UUID': '9e6a55b6b4'}, globalenv=globalenv),
  ]

# from https://github.com/boostorg/boost-ci
load("@boost_ci//ci/drone/:functions.star", "linux_cxx","windows_cxx","osx_cxx","freebsd_cxx")
