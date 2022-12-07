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
  osx_cxx("ASAN=on MAC_OSX=true Job 0", "clang++", packages="", buildtype="boost", buildscript="drone", environment={'ASAN': 'on', 'MAC_OSX': 'true', 'DRONE_JOB_UUID': 'b6589fc6ab'}, globalenv=globalenv),
  osx_cxx("ASAN=on MAC_OSX=true Job 1", "clang++", packages="", buildtype="boost", buildscript="drone", environment={'ASAN': 'on', 'MAC_OSX': 'true', 'DRONE_JOB_UUID': '356a192b79'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=6 ASAN=off Job 2", "gcc-6", packages="g++-6 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '6', 'ASAN': 'off', 'DRONE_JOB_UUID': 'da4b9237ba'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=7 ASAN=off Job 3", "gcc-7", packages="g++-7 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '7', 'ASAN': 'off', 'DRONE_JOB_UUID': '77de68daec'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=8 ASAN=off Job 4", "gcc-8", packages="g++-8 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '8', 'ASAN': 'off', 'DRONE_JOB_UUID': '1b64538924'}, globalenv=globalenv),
    ]

# from https://github.com/boostorg/boost-ci
load("@boost_ci//ci/drone/:functions.star", "linux_cxx","windows_cxx","osx_cxx","freebsd_cxx")
