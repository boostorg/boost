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
  linux_cxx("TOOLSET=gcc COMPILER=g++-6 CXXSTD=14,1z Job 0", "g++-6", packages="g++-6", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-6', 'CXXSTD': '14,1z', 'DRONE_JOB_UUID': 'b6589fc6ab'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc COMPILER=g++-7 CXXSTD=14,17 Job 1", "g++-7", packages="g++-7", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-7', 'CXXSTD': '14,17', 'DRONE_JOB_UUID': '356a192b79'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc COMPILER=g++-8 CXXSTD=14,17 Job 2", "g++-8", packages="g++-8", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-8', 'CXXSTD': '14,17', 'DRONE_JOB_UUID': 'da4b9237ba'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc COMPILER=g++-9 CXXSTD=14,17,2a Job 3", "g++-9", packages="g++-9", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-9', 'CXXSTD': '14,17,2a', 'DRONE_JOB_UUID': '77de68daec'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-3.9 CXXSTD=14, Job 4", "clang++-3.9", packages="clang-3.9", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-3.9', 'CXXSTD': '14,1z', 'DRONE_JOB_UUID': '1b64538924'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-4.0 CXXSTD=14, Job 5", "clang++-4.0", packages="clang-4.0 libstdc++-6-dev", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-4.0', 'CXXSTD': '14,1z', 'DRONE_JOB_UUID': 'ac3478d69a'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-5.0 CXXSTD=14, Job 6", "clang++-5.0", packages="clang-5.0 libstdc++-7-dev", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-5.0', 'CXXSTD': '14,1z', 'DRONE_JOB_UUID': 'c1dfd96eea'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-6.0 CXXSTD=14, Job 7", "clang++-6.0", packages="clang-6.0 libstdc++-8-dev", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-6.0', 'CXXSTD': '14,17', 'DRONE_JOB_UUID': '902ba3cda1'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-7 CXXSTD=14,17 Job 8", "clang++-7", packages="clang-7", llvm_os="xenial", llvm_ver="7", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-7', 'CXXSTD': '14,17,2a', 'DRONE_JOB_UUID': 'fe5dbbcea5'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-8 CXXSTD=14,17 Job 9", "clang++-8", packages="clang-8", llvm_os="xenial", llvm_ver="8", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-8', 'CXXSTD': '14,17,2a', 'DRONE_JOB_UUID': '0ade7c2cf9'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang COMPILER=clang++-libc++ CXXSTD= Job 10", "clang++-libc++", packages="libc++-dev", buildtype="boost", buildscript="drone", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-libc++', 'CXXSTD': '14,1z', 'DRONE_JOB_UUID': 'b1d5781111'}, globalenv=globalenv),
  osx_cxx("TOOLSET=clang COMPILER=clang++ CXXSTD=14,1z Job 11", "clang++", packages="", buildtype="boost", buildscript="drone", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++', 'CXXSTD': '14,1z', 'DRONE_JOB_UUID': '17ba079149'}, globalenv=globalenv),
    ]

# from https://github.com/boostorg/boost-ci
load("@boost_ci//ci/drone/:functions.star", "linux_cxx","windows_cxx","osx_cxx","freebsd_cxx")
