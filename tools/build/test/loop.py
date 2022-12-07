#!/usr/bin/python

# Copyright 2003 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild
import string

t = BoostBuild.Tester()

t.write("jamroot.jam", """\
lib main : main.cpp l ;
lib l : l.cpp main ;
""")

t.write("main.cpp", "")
t.write("l.cpp", "")

t.run_build_system(["--no-error-backtrace"], status=1)
t.fail_test(t.stdout().find(
    "error: Recursion in main target references") == -1)

t.cleanup()
