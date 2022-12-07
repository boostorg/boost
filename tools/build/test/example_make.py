#!/usr/bin/python

# Copyright (C) Vladimir Prus 2006.
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test the 'make' example.

import BoostBuild
import sys

t = BoostBuild.Tester(['example.python.interpreter=%s' % sys.executable])
t.set_tree("../example/make")
t.run_build_system()
t.expect_addition(["bin/main.cpp"])
t.cleanup()
