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
  linux_cxx("TOOLSET=gcc CXXSTD=03,11 Job 0", "g++", packages="", buildtype="boost", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': 'b6589fc6ab'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang CXXSTD=03,11,14,1z Job 1", "clang++", packages="libstdc++-4.9-dev", buildtype="boost", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'CXXSTD': '03,11,14,1z', 'DRONE_JOB_UUID': '356a192b79'}, globalenv=globalenv),
  osx_cxx("TOOLSET=clang CXXSTD=03,11,14,1z Job 2", "clang++", packages="", buildtype="boost", environment={'TOOLSET': 'clang', 'CXXSTD': '03,11,14,1z', 'DRONE_JOB_UUID': 'da4b9237ba'}, globalenv=globalenv),
  ]

# from https://github.com/boostorg/boost-ci
load("@boost_ci//ci/drone/:functions.star", "linux_cxx","windows_cxx","osx_cxx","freebsd_cxx")
