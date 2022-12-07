# Use, modification, and distribution are
# subject to the Boost Software License, Version 1.0. (See accompanying
# file LICENSE.txt)
#
# Copyright Rene Rivera 2020.

# For Drone CI we use the Starlark scripting language to reduce duplication.
# As the yaml syntax for Drone CI is rather limited.
#
#
globalenv={'PROJECT': 'libs/spirit'}
linuxglobalimage="cppalliance/droneubuntu1804:1"
windowsglobalimage="cppalliance/dronevs2019"

def main(ctx):
  return [
  linux_cxx("STD=14  JOB=test/x3 Job 0", "clang-10", packages="clang-10 libc++-10-dev libc++abi-10-dev jq", llvm_os="bionic", llvm_ver="10", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '14', 'JOB': 'test/x3', 'TRAVIS_COMPILER': 'clang-10', 'DRONE_JOB_UUID': 'b6589fc6ab'}, globalenv=globalenv),
  linux_cxx("STD=14  JOB=test/x3 Job 1", "gcc-10", packages="g++-10 jq", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '14', 'JOB': 'test/x3', 'TRAVIS_COMPILER': 'gcc-10', 'DRONE_JOB_UUID': '356a192b79'}, globalenv=globalenv),
  linux_cxx("STD=11  JOB=test/qi Job 2", "clang-10", packages="clang-10 libc++-10-dev libc++abi-10-dev jq", llvm_os="bionic", llvm_ver="10", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '11', 'JOB': 'test/qi', 'TRAVIS_COMPILER': 'clang-10', 'DRONE_JOB_UUID': 'da4b9237ba'}, globalenv=globalenv),
  linux_cxx("STD=11  JOB=test/karma Job 3", "clang-10", packages="clang-10 libc++-10-dev libc++abi-10-dev jq", llvm_os="bionic", llvm_ver="10", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '11', 'JOB': 'test/karma', 'TRAVIS_COMPILER': 'clang-10', 'DRONE_JOB_UUID': '77de68daec'}, globalenv=globalenv),
  linux_cxx("STD=11  JOB=test/lex Job 4", "clang-10", packages="clang-10 libc++-10-dev libc++abi-10-dev jq", llvm_os="bionic", llvm_ver="10", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '11', 'JOB': 'test/lex', 'TRAVIS_COMPILER': 'clang-10', 'DRONE_JOB_UUID': '1b64538924'}, globalenv=globalenv),
  linux_cxx("STD=11  JOB=test/support Job 5", "clang-10", packages="clang-10 libc++-10-dev libc++abi-10-dev jq", llvm_os="bionic", llvm_ver="10", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '11', 'JOB': 'test/support', 'TRAVIS_COMPILER': 'clang-10', 'DRONE_JOB_UUID': 'ac3478d69a'}, globalenv=globalenv),
  linux_cxx("STD=11  JOB=repository/test Job 6", "clang-10", packages="clang-10 libc++-10-dev libc++abi-10-dev jq", llvm_os="bionic", llvm_ver="10", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '11', 'JOB': 'repository/test', 'TRAVIS_COMPILER': 'clang-10', 'DRONE_JOB_UUID': 'c1dfd96eea'}, globalenv=globalenv),
  # Not building  #
  #  linux_cxx("STD=03  JOB=test/qi Job 7", "clang-10", packages="clang-10 libc++-10-dev libc++abi-10-dev jq", llvm_os="bionic", llvm_ver="10", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '03', 'JOB': 'test/qi', 'TRAVIS_COMPILER': 'clang-10', 'DRONE_JOB_UUID': '902ba3cda1'}, globalenv=globalenv),
  # Not building  #
  #  linux_cxx("STD=03  JOB=test/karma Job 8", "clang-10", packages="clang-10 libc++-10-dev libc++abi-10-dev jq", llvm_os="bionic", llvm_ver="10", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '03', 'JOB': 'test/karma', 'TRAVIS_COMPILER': 'clang-10', 'DRONE_JOB_UUID': 'fe5dbbcea5'}, globalenv=globalenv),
  linux_cxx("STD=03  JOB=test/lex Job 9", "clang-10", packages="clang-10 libc++-10-dev libc++abi-10-dev jq", llvm_os="bionic", llvm_ver="10", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '03', 'JOB': 'test/lex', 'TRAVIS_COMPILER': 'clang-10', 'DRONE_JOB_UUID': '0ade7c2cf9'}, globalenv=globalenv),
  linux_cxx("STD=03  JOB=test/support Job 10", "clang-10", packages="clang-10 libc++-10-dev libc++abi-10-dev jq", llvm_os="bionic", llvm_ver="10", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '03', 'JOB': 'test/support', 'TRAVIS_COMPILER': 'clang-10', 'DRONE_JOB_UUID': 'b1d5781111'}, globalenv=globalenv),
  linux_cxx("STD=03  JOB=repository/test Job 11", "clang-10", packages="clang-10 libc++-10-dev libc++abi-10-dev jq", llvm_os="bionic", llvm_ver="10", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '03', 'JOB': 'repository/test', 'TRAVIS_COMPILER': 'clang-10', 'DRONE_JOB_UUID': '17ba079149'}, globalenv=globalenv),
  linux_cxx("STD=11  JOB=test/qi Job 12", "gcc-10", packages="g++-10 jq", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '11', 'JOB': 'test/qi', 'TRAVIS_COMPILER': 'gcc-10', 'DRONE_JOB_UUID': '7b52009b64'}, globalenv=globalenv),
  linux_cxx("STD=11  JOB=test/karma Job 13", "gcc-10", packages="g++-10 jq", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '11', 'JOB': 'test/karma', 'TRAVIS_COMPILER': 'gcc-10', 'DRONE_JOB_UUID': 'bd307a3ec3'}, globalenv=globalenv),
  linux_cxx("STD=11  JOB=test/lex Job 14", "gcc-10", packages="g++-10 jq", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '11', 'JOB': 'test/lex', 'TRAVIS_COMPILER': 'gcc-10', 'DRONE_JOB_UUID': 'fa35e19212'}, globalenv=globalenv),
  linux_cxx("STD=11  JOB=test/support Job 15", "gcc-10", packages="g++-10 jq", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '11', 'JOB': 'test/support', 'TRAVIS_COMPILER': 'gcc-10', 'DRONE_JOB_UUID': 'f1abd67035'}, globalenv=globalenv),
  linux_cxx("STD=11  JOB=repository/test Job 16", "gcc-10", packages="g++-10 jq", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '11', 'JOB': 'repository/test', 'TRAVIS_COMPILER': 'gcc-10', 'DRONE_JOB_UUID': '1574bddb75'}, globalenv=globalenv),
  # Not building  #
  #  linux_cxx("STD=03  JOB=test/qi Job 17", "gcc-10", packages="g++-10 jq", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '03', 'JOB': 'test/qi', 'TRAVIS_COMPILER': 'gcc-10', 'DRONE_JOB_UUID': '0716d9708d'}, globalenv=globalenv),
  # Not building  #
  #  linux_cxx("STD=03  JOB=test/karma Job 18", "gcc-10", packages="g++-10 jq", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '03', 'JOB': 'test/karma', 'TRAVIS_COMPILER': 'gcc-10', 'DRONE_JOB_UUID': '9e6a55b6b4'}, globalenv=globalenv),
  linux_cxx("STD=03  JOB=test/lex Job 19", "gcc-10", packages="g++-10 jq", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '03', 'JOB': 'test/lex', 'TRAVIS_COMPILER': 'gcc-10', 'DRONE_JOB_UUID': 'b3f0c7f6bb'}, globalenv=globalenv),
  linux_cxx("STD=03  JOB=test/support Job 20", "gcc-10", packages="g++-10 jq", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '03', 'JOB': 'test/support', 'TRAVIS_COMPILER': 'gcc-10', 'DRONE_JOB_UUID': '91032ad7bb'}, globalenv=globalenv),
  linux_cxx("STD=03  JOB=repository/test Job 21", "gcc-10", packages="g++-10 jq", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '03', 'JOB': 'repository/test', 'TRAVIS_COMPILER': 'gcc-10', 'DRONE_JOB_UUID': '472b07b9fc'}, globalenv=globalenv),
  # Not building  #
  #  linux_cxx("STD=03  JOB=classic/test Job 22", "clang-10", packages="clang-10 libc++-10-dev libc++abi-10-dev jq", llvm_os="bionic", llvm_ver="10", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '03', 'JOB': 'classic/test', 'TRAVIS_COMPILER': 'clang-10', 'DRONE_JOB_UUID': '12c6fc06c9'}, globalenv=globalenv),
  # Not building  #
  #  linux_cxx("STD=03  JOB=classic/test Job 23", "gcc-10", packages="g++-10 jq", buildtype="boost", buildscript="drone", image=linuxglobalimage, environment={'STD': '03', 'JOB': 'classic/test', 'TRAVIS_COMPILER': 'gcc-10', 'DRONE_JOB_UUID': 'd435a6cdd7'}, globalenv=globalenv),
    ]

# from https://github.com/boostorg/boost-ci
load("@boost_ci//ci/drone/:functions.star", "linux_cxx","windows_cxx","osx_cxx","freebsd_cxx")
