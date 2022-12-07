#!/usr/bin/python

# Copyright (C) Vladimir Prus 2005.
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild

t = BoostBuild.Tester(use_test_config=False)

t.write("jamroot.jam", "using some_tool ;")
t.write("some_tool.jam", """\
import project ;
project.initialize $(__name__) ;
rule init ( ) { }
""")

t.write("some_tool.py", """\
from b2.manager import get_manager
get_manager().projects().initialize(__name__)
def init():
    pass
""")

t.write("sub/a.cpp", "int main() {}\n")
t.write("sub/jamfile.jam", "exe a : a.cpp ;")

t.run_build_system(subdir="sub")
t.expect_addition("sub/bin/$toolset/debug*/a.exe")

t.cleanup()
