#!/usr/bin/python
# coding: utf-8
#
# Copyright 2017 Steven Watanabe
# Copyright 2020 René Ferdinand Rivera Morell
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# validates the clang_linux toolset using a mock of clang

from TestToolset import test_toolset

test_toolset("clang-linux", "3.9.0", [
    ["target-os=linux"],
    ["target-os=linux", "release", "strip=on"],
    ["target-os=linux", "threading=multi"],
    ["target-os=linux", "link=static"],
    ["target-os=linux", "link=static", "runtime-link=static"],
    ["target-os=linux", "architecture=x86", "address-model=32"]])

test_toolset("clang-linux", "3.9.0", [
    ["target-os=windows"],
    ["target-os=windows", "release", "strip=on"],
    ["target-os=windows", "threading=multi"],
    ["target-os=windows", "link=static"],
    ["target-os=windows", "architecture=x86", "address-model=32"]
    ])
