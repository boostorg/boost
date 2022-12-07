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
  osx_cxx("ASAN=on MAC_OSX=true Job 0", "clang++", packages="", buildtype="boost", buildscript="drone", xcode_version="8.3", environment={'ASAN': 'on', 'MAC_OSX': 'true', 'DRONE_JOB_UUID': 'b6589fc6ab'}, globalenv=globalenv),
  osx_cxx("ASAN=on MAC_OSX=true Job 1", "clang++", packages="", buildtype="boost", buildscript="drone", xcode_version="9.4", environment={'ASAN': 'on', 'MAC_OSX': 'true', 'DRONE_JOB_UUID': '356a192b79'}, globalenv=globalenv),
  osx_cxx("ASAN=on MAC_OSX=true Job 2", "clang++", packages="", buildtype="boost", buildscript="drone", xcode_version="10.3", environment={'ASAN': 'on', 'MAC_OSX': 'true', 'DRONE_JOB_UUID': 'da4b9237ba'}, globalenv=globalenv),
  osx_cxx("ASAN=on MAC_OSX=true Job 3", "clang++", packages="", buildtype="boost", buildscript="drone", xcode_version="11.4", environment={'ASAN': 'on', 'MAC_OSX': 'true', 'DRONE_JOB_UUID': '77de68daec'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=6 ASAN=off Job 4", "gcc-6", packages="g++-6 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '6', 'ASAN': 'off', 'DRONE_JOB_UUID': '1b64538924'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=7 ASAN=off Job 5", "gcc-7", packages="g++-7 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '7', 'ASAN': 'off', 'DRONE_JOB_UUID': 'ac3478d69a'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=8 ASAN=off Job 6", "gcc-8", packages="g++-8 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '8', 'ASAN': 'off', 'DRONE_JOB_UUID': 'c1dfd96eea'}, globalenv=globalenv),
  linux_cxx("GCC_VERSION=9 ASAN=off Job 7", "gcc-9", packages="g++-9 valgrind", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'GCC_VERSION': '9', 'ASAN': 'off', 'DRONE_JOB_UUID': '902ba3cda1'}, globalenv=globalenv),
    ]

# from https://github.com/boostorg/boost-ci
load("@boost_ci//ci/drone/:functions.star", "linux_cxx","windows_cxx","osx_cxx","freebsd_cxx")
