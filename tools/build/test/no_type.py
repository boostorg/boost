#!/usr/bin/python

# Copyright 2002 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test that we cannot specify targets of unknown type as sources. This is based
# on the fact that Unix 'ar' will happily consume just about anything.

import BoostBuild

t = BoostBuild.Tester()

t.write("jamroot.jam", "static-lib a : a.foo ;")
t.write("a.foo", "")

t.run_build_system(status=1)

t.cleanup()
