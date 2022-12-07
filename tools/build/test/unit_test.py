#!/usr/bin/python

# Copyright 2003, 2004 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

#  Test the unit_test rule.

import BoostBuild

t = BoostBuild.Tester(use_test_config=False)

# Create the needed files.
t.write("jamroot.jam", """
using testing ;
lib helper : helper.cpp ;
unit-test test : test.cpp : <library>helper ;
""")

t.write("test.cpp", """
void helper();
int main() { helper(); }
""")

t.write("helper.cpp", """
void
#if defined(_WIN32)
__declspec(dllexport)
#endif
helper() {}
""")

t.run_build_system(["link=static"])
t.expect_addition("bin/$toolset/debug/link-static*/test.passed")

t.cleanup()
