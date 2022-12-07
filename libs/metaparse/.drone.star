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
  linux_cxx("CXX_VER=3.4 TOOLSET=clang STD=c++98 Job 0", "clang++", packages="doxygen xsltproc", buildtype="4e3fa5de4a-f44b5bccc0", buildscript="drone", image=linuxglobalimage, environment={'CXX_VER': '3.4', 'TOOLSET': 'clang', 'STD': 'c++98', 'DRONE_JOB_UUID': 'b6589fc6ab'}, globalenv=globalenv),
  linux_cxx("CXX_VER=3.5.2 TOOLSET=clang STD=c++98 Job 1", "clang++", packages="doxygen xsltproc", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'CXX_VER': '3.5.2', 'TOOLSET': 'clang', 'STD': 'c++98', 'DRONE_JOB_UUID': '356a192b79'}, globalenv=globalenv),
  linux_cxx("CXX_VER=3.5.2 TOOLSET=clang STD=c++11 Job 2", "clang++", packages="doxygen xsltproc", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'CXX_VER': '3.5.2', 'TOOLSET': 'clang', 'STD': 'c++11', 'DRONE_JOB_UUID': 'da4b9237ba'}, globalenv=globalenv),
  linux_cxx("CXX_VER=3.6.2 TOOLSET=clang STD=c++98 Job 3", "clang++", packages="doxygen xsltproc", buildtype="fea35f4805-f44b5bccc0", buildscript="drone", image=linuxglobalimage, environment={'CXX_VER': '3.6.2', 'TOOLSET': 'clang', 'STD': 'c++98', 'DRONE_JOB_UUID': '77de68daec'}, globalenv=globalenv),
  linux_cxx("CXX_VER=3.6.2 TOOLSET=clang STD=c++11 Job 4", "clang++", packages="doxygen xsltproc", buildtype="fea35f4805-f44b5bccc0", buildscript="drone", image=linuxglobalimage, environment={'CXX_VER': '3.6.2', 'TOOLSET': 'clang', 'STD': 'c++11', 'DRONE_JOB_UUID': '1b64538924'}, globalenv=globalenv),
  linux_cxx("CXX_VER=4.6 TOOLSET=gcc STD=c++98 Job 5", "g++", packages="g++-4.6 doxygen xsltproc", buildtype="dfd0ac5626-f44b5bccc0", buildscript="drone", image=linuxglobalimage, environment={'CXX_VER': '4.6', 'TOOLSET': 'gcc', 'STD': 'c++98', 'DRONE_JOB_UUID': 'ac3478d69a'}, globalenv=globalenv),
  linux_cxx("CXX_VER=4.7 TOOLSET=gcc STD=c++98 Job 6", "g++", packages="g++-4.7 doxygen xsltproc", buildtype="0e39902f2e-f44b5bccc0", buildscript="drone", image=linuxglobalimage, environment={'CXX_VER': '4.7', 'TOOLSET': 'gcc', 'STD': 'c++98', 'DRONE_JOB_UUID': 'c1dfd96eea'}, globalenv=globalenv),
  linux_cxx("CXX_VER=4.7 TOOLSET=gcc STD=c++11 Job 7", "g++", packages="g++-4.7 doxygen xsltproc", buildtype="0e39902f2e-f44b5bccc0", buildscript="drone", image=linuxglobalimage, environment={'CXX_VER': '4.7', 'TOOLSET': 'gcc', 'STD': 'c++11', 'DRONE_JOB_UUID': '902ba3cda1'}, globalenv=globalenv),
  linux_cxx("CXX_VER=4.8 TOOLSET=gcc STD=c++98 Job 8", "g++", packages="g++-4.8 doxygen xsltproc", buildtype="91b94bb329-f44b5bccc0", buildscript="drone", image=linuxglobalimage, environment={'CXX_VER': '4.8', 'TOOLSET': 'gcc', 'STD': 'c++98', 'DRONE_JOB_UUID': 'fe5dbbcea5'}, globalenv=globalenv),
  linux_cxx("CXX_VER=4.8 TOOLSET=gcc STD=c++11 Job 9", "g++", packages="g++-4.8 doxygen xsltproc", buildtype="91b94bb329-f44b5bccc0", buildscript="drone", image=linuxglobalimage, environment={'CXX_VER': '4.8', 'TOOLSET': 'gcc', 'STD': 'c++11', 'DRONE_JOB_UUID': '0ade7c2cf9'}, globalenv=globalenv),
  linux_cxx("CXX_VER=4.9 TOOLSET=gcc STD=c++98 Job 10", "g++", packages="g++-4.9 doxygen xsltproc", buildtype="bb699d113a-f44b5bccc0", buildscript="drone", image=linuxglobalimage, environment={'CXX_VER': '4.9', 'TOOLSET': 'gcc', 'STD': 'c++98', 'DRONE_JOB_UUID': 'b1d5781111'}, globalenv=globalenv),
  linux_cxx("CXX_VER=4.9 TOOLSET=gcc STD=c++11 Job 11", "g++", packages="g++-4.9 doxygen xsltproc", buildtype="bb699d113a-f44b5bccc0", buildscript="drone", image=linuxglobalimage, environment={'CXX_VER': '4.9', 'TOOLSET': 'gcc', 'STD': 'c++11', 'DRONE_JOB_UUID': '17ba079149'}, globalenv=globalenv),
    ]

# from https://github.com/boostorg/boost-ci
load("@boost_ci//ci/drone/:functions.star", "linux_cxx","windows_cxx","osx_cxx","freebsd_cxx")
