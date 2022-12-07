#!/usr/bin/python

# Copyright 2002 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild

t = BoostBuild.Tester(pass_toolset=0)

t.write("test.jam", """
actions unbuilt { }
unbuilt all ;
ECHO "Hi" ;
""")

t.run_build_system(["-ftest.jam", "-d0"], stdout="Hi\n")
t.cleanup()
