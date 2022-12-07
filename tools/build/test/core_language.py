#!/usr/bin/python

# Copyright 2002, 2003 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild

t = BoostBuild.Tester(pass_toolset=0)
t.set_tree("core-language")
t.run_build_system(["-ftest.jam"])
t.cleanup()
