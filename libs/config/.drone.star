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
  linux_cxx("g++-4.4 98,0x", "g++", packages="g++-4.4", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-4.4', 'CXXSTD': '98,0x', }, globalenv=globalenv),
  linux_cxx("g++-4.6 03,0x", "g++", packages="g++-4.6", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-4.6', 'CXXSTD': '03,0x', }, globalenv=globalenv),
  linux_cxx("g++-4.7 03,11", "g++-4.7", packages="g++-4.7", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-4.7', 'CXXSTD': '03,11', }, globalenv=globalenv),
  linux_cxx("g++-4.8 03,11", "g++-4.8", packages="g++-4.8", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-4.8', 'CXXSTD': '03,11', }, globalenv=globalenv),
  linux_cxx("g++-4.9 03,11", "g++-4.9", packages="g++-4.9", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-4.9', 'CXXSTD': '03,11', }, globalenv=globalenv),
  linux_cxx("g++-5 03,11,14,1z", "g++-5", packages="g++-5", buildtype="boost", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-5', 'CXXSTD': '03,11,14,1z', }, globalenv=globalenv),
  linux_cxx("g++-5 gnu-03,11,14,1z", "g++-5", packages="g++-5", buildtype="boost", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-5', 'CXXSTD': '03,11,14,1z', 'CXXSTD_DIALECT': 'cxxstd-dialect=gnu', }, globalenv=globalenv),
  linux_cxx("g++-6 03,11,14,1z", "g++-6", packages="g++-6", buildtype="boost", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-6', 'CXXSTD': '03,11,14,1z', }, globalenv=globalenv),
  linux_cxx("g++-6 gnu++03,11,14,1z Job 9", "g++-6", packages="g++-6", buildtype="boost", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-6', 'CXXSTD': '03,11,14,1z', 'CXXSTD_DIALECT': 'cxxstd-dialect=gnu', }, globalenv=globalenv),
  linux_cxx("g++-7 03,11,14,17", "g++-7", packages="g++-7", buildtype="boost", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-7', 'CXXSTD': '03,11,14,17', }, globalenv=globalenv),
  linux_cxx("g++-7 gnu++03,11,14,17", "g++-7", packages="g++-7", buildtype="boost", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-7', 'CXXSTD': '03,11,14,17', 'CXXSTD_DIALECT': 'cxxstd-dialect=gnu', }, globalenv=globalenv),
  linux_cxx("g++-8 03,11,14,17", "g++-8", packages="g++-8", buildtype="boost", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-8', 'CXXSTD': '03,11,14,17', }, globalenv=globalenv),
  linux_cxx("g++-8 gnu++03,11,14,17", "g++-8", packages="g++-8", buildtype="boost", image=linuxglobalimage, environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-8', 'CXXSTD': '03,11,14,17', 'CXXSTD_DIALECT': 'cxxstd-dialect=gnu', }, globalenv=globalenv),
  linux_cxx("g++-9 03,11,14,17", "g++-9", packages="g++-9", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-9', 'CXXSTD': '03,11,14,17,2a', }, globalenv=globalenv),
  linux_cxx("g++-9 gnu++03,11,14,17", "g++-9", packages="g++-9", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-9', 'CXXSTD': '03,11,14,17,2a', 'CXXSTD_DIALECT': 'cxxstd-dialect=gnu', }, globalenv=globalenv),
  linux_cxx("g++-10 03,11,14,17,20", "g++-10", packages="g++-10", image="cppalliance/droneubuntu2004:1", buildtype="boost", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-10', 'CXXSTD': '03,11,14,17,20', }, globalenv=globalenv),
  linux_cxx("g++-10 gnu++03,11,14,17,20", "g++-10", packages="g++-10", image="cppalliance/droneubuntu2004:1", buildtype="boost", environment={'TOOLSET': 'gcc', 'COMPILER': 'g++-10', 'CXXSTD': '03,11,14,17,20', 'CXXSTD_DIALECT': 'cxxstd-dialect=gnu', }, globalenv=globalenv),
  linux_cxx("clang++-3.5 03", "clang++", packages="clang-3.5", llvm_os="precise", llvm_ver="3.5", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-3.5', 'CXXSTD': '03,11,14,1z', }, globalenv=globalenv),
  linux_cxx("clang++-3.6 03", "clang++", packages="clang-3.6", llvm_os="precise", llvm_ver="3.6", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-3.6', 'CXXSTD': '03,11,14,1z', }, globalenv=globalenv),
  linux_cxx("clang++-3.7 03", "clang++", packages="clang-3.7", llvm_os="precise", llvm_ver="3.7", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-3.7', 'CXXSTD': '03,11,14,1z', }, globalenv=globalenv),
  linux_cxx("clang++-3.8 03", "clang++-3.8", packages="clang-3.8", llvm_os="precise", llvm_ver="3.8", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-3.8', 'CXXSTD': '03,11,14,1z', }, globalenv=globalenv),
  linux_cxx("clang++-3.9 03", "clang++-3.9", packages="clang-3.9", llvm_os="precise", llvm_ver="3.9", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-3.9', 'CXXSTD': '03,11,14,1z', }, globalenv=globalenv),
  linux_cxx("clang++-4.0 03", "clang++-4.0", packages="clang-4.0", llvm_os="xenial", llvm_ver="4.0", buildtype="boost", image=linuxglobalimage, environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-4.0', 'CXXSTD': '03,11,14,1z', }, globalenv=globalenv),
  linux_cxx("clang++-5.0 03", "clang++-5.0", packages="clang-5.0", llvm_os="xenial", llvm_ver="5.0", buildtype="boost", image="cppalliance/droneubuntu1804:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-5.0', 'CXXSTD': '03,11,14,1z', }, globalenv=globalenv),
  linux_cxx("clang++-6.0 03", "clang++-6.0", packages="clang-6.0", llvm_os="xenial", llvm_ver="6.0", buildtype="boost", image="cppalliance/droneubuntu1804:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-6.0', 'CXXSTD': '03,11,14,1z', }, globalenv=globalenv),
  linux_cxx("clang++-7 03,11", "clang++-7", packages="clang-7", llvm_os="xenial", llvm_ver="7", buildtype="boost", image="cppalliance/droneubuntu1804:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-7', 'CXXSTD': '03,11,14,1z', }, globalenv=globalenv),
  linux_cxx("clang++-8 03,11", "clang++-8", packages="clang-8", llvm_os="xenial", llvm_ver="8", buildtype="boost", image="cppalliance/droneubuntu1804:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-8', 'CXXSTD': '03,11,14,17,2a', }, globalenv=globalenv),
  linux_cxx("clang++-9 03,11,14,17,2a", "clang++-9", packages="clang-9", llvm_os="xenial", llvm_ver="9", buildtype="boost", image="cppalliance/droneubuntu1804:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-9', 'CXXSTD': '03,11,14,17,2a', }, globalenv=globalenv),
  linux_cxx("clang++-10 03,11,14,17,20", "clang++-10", packages="clang-10", llvm_os="xenial", llvm_ver="10", buildtype="boost", image="cppalliance/droneubuntu1804:1", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++-10', 'CXXSTD': '03,11,14,17,20', }, globalenv=globalenv),
  osx_cxx("XCode-11.7 03,11,17,2a", "clang++", packages="", buildtype="boost", xcode_version="11.7", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++', 'CXXSTD': '03,11,14,17,2a', }, globalenv=globalenv),
  osx_cxx("XCode-10.2 03,11,17,2a", "clang++", packages="", buildtype="boost", xcode_version="10.2", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++', 'CXXSTD': '03,11,14,17,2a', }, globalenv=globalenv),
  ]

# from https://github.com/boostorg/boost-ci
load("@boost_ci//ci/drone/:functions.star", "linux_cxx","windows_cxx","osx_cxx","freebsd_cxx")
