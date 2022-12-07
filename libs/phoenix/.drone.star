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
  linux_cxx("TOOLSET=gcc-4.4 CXXSTD=98,0x Job 0", "g++", packages="g++-4.4", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'gcc-4.4', 'CXXSTD': '98,0x', 'DRONE_JOB_UUID': 'b6589fc6ab'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-4.6 CXXSTD=03,0x Job 1", "g++", packages="g++-4.6", buildtype="boost", image="cppalliance/droneubuntu1404:1", environment={'TOOLSET': 'gcc-4.6', 'CXXSTD': '03,0x', 'DRONE_JOB_UUID': '356a192b79'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-4.7 CXXSTD=03,11 Job 2", "g++", packages="g++-4.7", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-4.7', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': 'da4b9237ba'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-4.8 CXXSTD=03,11 Job 3", "g++", packages="g++-4.8", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-4.8', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': '77de68daec'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-4.8 CXXSTD=1y Job 4", "g++", packages="g++-4.8", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-4.8', 'CXXSTD': '1y', 'DRONE_JOB_UUID': '1b64538924'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-4.9 CXXSTD=03,11 Job 5", "g++", packages="g++-4.9", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-4.9', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': 'ac3478d69a'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-4.9 CXXSTD=14 Job 6", "g++", packages="g++-4.9", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-4.9', 'CXXSTD': '14', 'DRONE_JOB_UUID': 'c1dfd96eea'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-5 CXXSTD=03,11 Job 7", "g++", packages="g++-5", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-5', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': '902ba3cda1'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-5 CXXSTD=14,17 Job 8", "g++", packages="g++-5", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-5', 'CXXSTD': '14,17', 'DRONE_JOB_UUID': 'fe5dbbcea5'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-6 CXXSTD=03,11 Job 9", "g++", packages="g++-6", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-6', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': '0ade7c2cf9'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-6 CXXSTD=14,17 Job 10", "g++", packages="g++-6", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-6', 'CXXSTD': '14,17', 'DRONE_JOB_UUID': 'b1d5781111'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-7 CXXSTD=03,11 Job 11", "g++", packages="g++-7", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-7', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': '17ba079149'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-7 CXXSTD=14,17 Job 12", "g++", packages="g++-7", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-7', 'CXXSTD': '14,17', 'DRONE_JOB_UUID': '7b52009b64'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-8 CXXSTD=03,11 Job 13", "g++", packages="g++-8", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-8', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': 'bd307a3ec3'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-8 CXXSTD=14,17 Job 14", "g++", packages="g++-8", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-8', 'CXXSTD': '14,17', 'DRONE_JOB_UUID': 'fa35e19212'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-8 CXXSTD=2a Job 15", "g++", packages="g++-8", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-8', 'CXXSTD': '2a', 'DRONE_JOB_UUID': 'f1abd67035'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-9 CXXSTD=03,11,14 SANITIZED Job 16", "g++", packages="g++-9", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-9', 'CXXSTD': '03,11,14', 'DRONE_JOB_UUID': '1574bddb75'}, globalenv=globalenv),
  linux_cxx("TOOLSET=gcc-9 CXXSTD=17,2a SANITIZED Job 17", "g++", packages="g++-9", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'gcc-9', 'CXXSTD': '17,2a', 'DRONE_JOB_UUID': '0716d9708d'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-3.5 CXXSTD=03,11 Job 18", "g++", packages="clang-3.5", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-3.5', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': '9e6a55b6b4'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-3.6 CXXSTD=03,11 Job 19", "g++", packages="clang-3.6", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-3.6', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': 'b3f0c7f6bb'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-3.6 CXXSTD=14,1z Job 20", "g++", packages="clang-3.6", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-3.6', 'CXXSTD': '14,1z', 'DRONE_JOB_UUID': '91032ad7bb'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-3.7 CXXSTD=03,11 Job 21", "g++", packages="clang-3.7", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-3.7', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': '472b07b9fc'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-3.7 CXXSTD=14,1z Job 22", "g++", packages="clang-3.7", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-3.7', 'CXXSTD': '14,1z', 'DRONE_JOB_UUID': '12c6fc06c9'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-3.8 CXXSTD=03,11 Job 23", "g++", packages="clang-3.8", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-3.8', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': 'd435a6cdd7'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-3.8 CXXSTD=14,1z Job 24", "g++", packages="clang-3.8", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-3.8', 'CXXSTD': '14,1z', 'DRONE_JOB_UUID': '4d134bc072'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-3.9 CXXSTD=03,11 Job 25", "g++", packages="clang-3.9", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-3.9', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': 'f6e1126ced'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-3.9 CXXSTD=14,1z Job 26", "g++", packages="clang-3.9", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-3.9', 'CXXSTD': '14,1z', 'DRONE_JOB_UUID': '887309d048'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-4.0 CXXSTD=03,11 Job 27", "g++", packages="clang-4.0", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-4.0', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': 'bc33ea4e26'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-4.0 CXXSTD=14,1z Job 28", "g++", packages="clang-4.0", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-4.0', 'CXXSTD': '14,1z', 'DRONE_JOB_UUID': '0a57cb53ba'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-5.0 CXXSTD=03,11 Job 29", "g++", packages="clang-5.0", llvm_os="xenial", llvm_ver="5.0", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-5.0', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': '7719a1c782'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-5.0 CXXSTD=14,17 Job 30", "g++", packages="clang-5.0", llvm_os="xenial", llvm_ver="5.0", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-5.0', 'CXXSTD': '14,17', 'DRONE_JOB_UUID': '22d200f867'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-5.0 CXXSTD=2a Job 31", "g++", packages="clang-5.0", llvm_os="xenial", llvm_ver="5.0", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-5.0', 'CXXSTD': '2a', 'DRONE_JOB_UUID': '632667547e'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-6.0 CXXSTD=03,11 Job 32", "g++", packages="clang-6.0", llvm_os="xenial", llvm_ver="6.0", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-6.0', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': 'cb4e5208b4'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-6.0 CXXSTD=14,17 Job 33", "g++", packages="clang-6.0", llvm_os="xenial", llvm_ver="6.0", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-6.0', 'CXXSTD': '14,17', 'DRONE_JOB_UUID': 'b6692ea5df'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-6.0 CXXSTD=2a Job 34", "g++", packages="clang-6.0", llvm_os="xenial", llvm_ver="6.0", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-6.0', 'CXXSTD': '2a', 'DRONE_JOB_UUID': 'f1f836cb4e'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-7 CXXSTD=03,11 Job 35", "g++", packages="clang-7", llvm_os="xenial", llvm_ver="7", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-7', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': '972a67c481'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-7 CXXSTD=14,17 Job 36", "g++", packages="clang-7", llvm_os="xenial", llvm_ver="7", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-7', 'CXXSTD': '14,17', 'DRONE_JOB_UUID': 'fc074d5013'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-7 CXXSTD=2a Job 37", "g++", packages="clang-7", llvm_os="xenial", llvm_ver="7", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-7', 'CXXSTD': '2a', 'DRONE_JOB_UUID': 'cb7a1d775e'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-8 CXXSTD=03,11,14 Job 38", "g++", packages="clang-8", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-8', 'CXXSTD': '03,11,14', 'DRONE_JOB_UUID': '5b384ce32d'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-8 CXXSTD=17,2a Job 39", "g++", packages="clang-8", buildtype="boost", image="cppalliance/droneubuntu1604:1", environment={'TOOLSET': 'clang-8', 'CXXSTD': '17,2a', 'DRONE_JOB_UUID': 'ca3512f4df'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-9 CXXSTD=03,11,14 SANITIZED Job 40", "g++", packages="clang-9", buildtype="boost", image="cppalliance/droneubuntu1804:1", environment={'TOOLSET': 'clang-9', 'CXXSTD': '03,11,14', 'DRONE_JOB_UUID': 'af3e133428'}, globalenv=globalenv),
  linux_cxx("TOOLSET=clang-9 CXXSTD=17,2a SANITIZED Job 41", "g++", packages="clang-9", buildtype="boost", image="cppalliance/droneubuntu1804:1", environment={'TOOLSET': 'clang-9', 'CXXSTD': '17,2a', 'DRONE_JOB_UUID': '761f22b2c1'}, globalenv=globalenv),
  osx_cxx("TOOLSET=clang COMPILER=clang++ CXXSTD=03,11 Job 42", "clang++", packages="", buildtype="boost", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++', 'CXXSTD': '03,11', 'DRONE_JOB_UUID': '92cfceb39d'}, globalenv=globalenv),
  osx_cxx("TOOLSET=clang COMPILER=clang++ CXXSTD=14,1z Job 43", "clang++", packages="", buildtype="boost", environment={'TOOLSET': 'clang', 'COMPILER': 'clang++', 'CXXSTD': '14,1z', 'DRONE_JOB_UUID': '0286dd552c'}, globalenv=globalenv),
  ]

# from https://github.com/boostorg/boost-ci
load("@boost_ci//ci/drone/:functions.star", "linux_cxx","windows_cxx","osx_cxx","freebsd_cxx")
