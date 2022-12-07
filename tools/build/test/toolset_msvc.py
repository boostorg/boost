#!/usr/bin/python
#
# Copyright 2022 Nikita Kniazev
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

from TestToolset import test_toolset

test_toolset("msvc", "14.3", [
    ["target-os=windows"],
    ["target-os=windows", "release"],
    ["target-os=windows", "threading=single"],
    ["target-os=windows", "link=static"],
    ["target-os=windows", "link=static", "runtime-link=static"],
    ["target-os=windows", "windows-api=store"],
    ["target-os=windows", "windows-api=phone"],
])
