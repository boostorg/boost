#!/usr/bin/env python

# Copyright 2020 Nikita Kniazev
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# Tests the force-include feature

import BoostBuild

t = BoostBuild.Tester(use_test_config=False)

t.write("Jamroot.jam", """
obj test-cpp : test.cpp : <force-include>one.h <force-include>two.h ;
obj test-c : test.c : <force-include>one.h <force-include>two.h ;
""")

for name in ("test.cpp", "test.c"):
    t.write(name, """
#ifndef ONE
#error Cannot compile without ONE
#endif
#ifndef TWO
#error Cannot compile without TWO
#endif
""")

t.write("one.h", """
#define ONE
""")

t.write("two.h", """
#define TWO
""")

t.run_build_system()
t.expect_addition("bin/$toolset/debug*/test-cpp.obj")
t.expect_addition("bin/$toolset/debug*/test-c.obj")

t.cleanup()
