#!/usr/bin/python

# Copyright 2016 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild

t = BoostBuild.Tester(use_test_config=False)

t.write("main.cpp", """\
int main() {}
""")

t.write("Jamroot", """\
exe test : main.cpp ;
always test ;
""")

t.run_build_system()
t.expect_addition("bin/$toolset/debug*/main.obj")
t.ignore_addition('bin/*/main.*.rsp')
t.expect_addition("bin/$toolset/debug*/test.exe")
t.ignore_addition('bin/*/test.rsp')
t.expect_nothing_more()

t.run_build_system()
t.expect_touch("bin/$toolset/debug*/main.obj")
t.ignore_touch('bin/*/main.*.rsp')
t.expect_touch("bin/$toolset/debug*/test.exe")
t.ignore_touch('bin/*/test.rsp')
t.expect_nothing_more()

t.cleanup()
