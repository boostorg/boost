# Use, modification, and distribution are
# subject to the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt)
#
# Copyright Rene Rivera 2020.

# For Drone CI we use the Starlark scripting language to reduce duplication.
# As the yaml syntax for Drone CI is rather limited.
#
#
globalenv={}
linuxglobalimage="cppalliance/droneubuntu1404:1"
windowsglobalimage="cppalliance/dronevs2019"

def main(ctx):
  return [
  osx_cxx("ASAN=off LIBCXX=off CXXFLAGS='-DBOOST_HOF_HAS Job 0", "clang++", packages="", buildtype="boost", buildscript="drone", environment={'CXXFLAGS': '-DBOOST_HOF_HAS_VARIABLE_TEMPLATES=0', 'ASAN': 'off', 'LIBCXX': 'off', 'DRONE_JOB_UUID': 'b6589fc6ab'}, globalenv=globalenv),
  linux_cxx("CLANG_VERSION=3.5 ASAN=on LIBCXX=on Job 1", "clang-3.5", packages="util-linux g++-4.9 clang-3.5 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'CLANG_VERSION': '3.5', 'ASAN': 'on', 'LIBCXX': 'on', 'DRONE_JOB_UUID': '356a192b79'}, globalenv=globalenv),
  linux_cxx("CLANG_VERSION=3.5 ASAN=on LIBCXX=off Job 2", "clang-3.5", packages="util-linux g++-4.9 clang-3.5 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'CLANG_VERSION': '3.5', 'ASAN': 'on', 'LIBCXX': 'off', 'DRONE_JOB_UUID': 'da4b9237ba'}, globalenv=globalenv),
  linux_cxx("CLANG_VERSION=3.8 ASAN=off LIBCXX=on BUILD_EX Job 3", "clang-3.8", packages="util-linux g++-4.9 clang-3.8 valgrind python", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'CLANG_VERSION': '3.8', 'ASAN': 'off', 'LIBCXX': 'on', 'BUILD_EXAMPLES': 'on', 'DRONE_JOB_UUID': '77de68daec'}, globalenv=globalenv),
  linux_cxx("CLANG_VERSION=3.8 ASAN=off LIBCXX=off BUILD_E Job 4", "clang-3.8", packages="util-linux g++-4.9 clang-3.8 valgrind python", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'CLANG_VERSION': '3.8', 'ASAN': 'off', 'LIBCXX': 'off', 'BUILD_EXAMPLES': 'on', 'DRONE_JOB_UUID': '1b64538924'}, globalenv=globalenv),
  linux_cxx("CLANG_VERSION=3.8 ASAN=off LIBCXX=off CXX11=o Job 5", "clang-3.8", packages="util-linux g++-4.9 clang-3.8 valgrind python", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'CLANG_VERSION': '3.8', 'ASAN': 'off', 'LIBCXX': 'off', 'CXX11': 'on', 'DRONE_JOB_UUID': 'ac3478d69a'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=7 ASAN=off LIBCXX=off Job 6", "gcc-7", packages="g++-7 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '7', 'ASAN': 'off', 'LIBCXX': 'off', 'DRONE_JOB_UUID': 'c1dfd96eea'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=6 ASAN=off LIBCXX=off Job 7", "gcc-6", packages="g++-6 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '6', 'ASAN': 'off', 'LIBCXX': 'off', 'DRONE_JOB_UUID': '902ba3cda1'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=5 ASAN=off LIBCXX=off Job 8", "gcc-5", packages="g++-5 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '5', 'ASAN': 'off', 'LIBCXX': 'off', 'DRONE_JOB_UUID': 'fe5dbbcea5'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=4.9 ASAN=off LIBCXX=off Job 9", "gcc-4.9", packages="g++-4.9 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '4.9', 'ASAN': 'off', 'LIBCXX': 'off', 'DRONE_JOB_UUID': '0ade7c2cf9'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=4.8 ASAN=off LIBCXX=off Job 10", "gcc-4.8", packages="g++-4.8 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '4.8', 'ASAN': 'off', 'LIBCXX': 'off', 'DRONE_JOB_UUID': 'b1d5781111'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=4.7 ASAN=off LIBCXX=off Job 11", "gcc-4.7", packages="g++-4.7 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '4.7', 'ASAN': 'off', 'LIBCXX': 'off', 'DRONE_JOB_UUID': '17ba079149'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=4.6 ASAN=off LIBCXX=off Job 12", "gcc-4.6", packages="g++-4.6 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '4.6', 'ASAN': 'off', 'LIBCXX': 'off', 'DRONE_JOB_UUID': '7b52009b64'}, globalenv=globalenv),
    ]

# from https://github.com/boostorg/boost-ci
load("@boost_ci//ci/drone/:functions.star", "linux_cxx","windows_cxx","osx_cxx","freebsd_cxx")
