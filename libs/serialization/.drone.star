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
linuxglobalimage="cppalliance/droneubuntu1604:1"
windowsglobalimage="cppalliance/dronevs2019"

def main(ctx):
  return [
  linux_cxx("TOOLSET=gcc LINK=static,shared Job 0", "g++", packages="", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'LINK': 'static,shared', 'TRAVIS_COMPILER': 'g++', 'DRONE_JOB_UUID': 'b6589fc6ab'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc LINK=static,shared Job 1", "g++", packages="g++-5", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'LINK': 'static,shared', 'TRAVIS_COMPILER': 'g++-5', 'DRONE_JOB_UUID': '356a192b79'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc LINK=static,shared Job 2", "g++", packages="g++-6", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'LINK': 'static,shared', 'TRAVIS_COMPILER': 'g++-6', 'DRONE_JOB_UUID': 'da4b9237ba'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc LINK=static,shared Job 3", "g++", packages="g++-7", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'LINK': 'static,shared', 'TRAVIS_COMPILER': 'g++-7', 'DRONE_JOB_UUID': '77de68daec'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang LINK=static,shared Job 4", "clang++", packages="", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'LINK': 'static,shared', 'TRAVIS_COMPILER': 'clang++', 'DRONE_JOB_UUID': '1b64538924'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang LINK=static,shared Job 5", "clang++", packages="libc++-dev", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'LINK': 'static,shared', 'TRAVIS_COMPILER': 'clang++-libc++', 'DRONE_JOB_UUID': 'ac3478d69a'}, globalenv=globalenv),
  osx_cxx("TOOLSET=clang LINK=static,shared Job 6", "clang++", packages="", buildtype="boost", buildscript="drone", environment={'TOOLSET': 'clang', 'LINK': 'static,shared', 'TRAVIS_COMPILER': 'clang++', 'DRONE_JOB_UUID': 'c1dfd96eea'}, globalenv=globalenv),
  osx_cxx("TOOLSET=clang LINK=static,shared Job 7", "clang++", packages="", buildtype="boost", buildscript="drone", xcode_version="10", environment={'TOOLSET': 'clang', 'LINK': 'static,shared', 'TRAVIS_COMPILER': 'clang++', 'DRONE_JOB_UUID': '902ba3cda1'}, globalenv=globalenv),
    ]

# from https://github.com/boostorg/boost-ci
load("@boost_ci//ci/drone/:functions.star", "linux_cxx","windows_cxx","osx_cxx","freebsd_cxx")
