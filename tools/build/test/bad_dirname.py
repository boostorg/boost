#!/usr/bin/python

# Copyright 2003 Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

# Regression test: when directory of project root contained regex
# metacharacters, B2 failed to work. Bug reported by Michael Stevens.

import BoostBuild

t = BoostBuild.Tester()

t.write("bad[abc]dirname/jamfile.jam", """
""")

t.write("bad[abc]dirname/jamroot.jam", """
""")

t.run_build_system(subdir="bad[abc]dirname")

t.cleanup()
