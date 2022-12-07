#!/usr/bin/python

# Copyright (C) Vladimir Prus 2005.
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# Basic tests for the 'notfile' rule.

import BoostBuild
import os

t = BoostBuild.Tester()

t.write("jamroot.jam", """\
import notfile ;
notfile say : "echo hi" ;
exe hello : hello.cpp ;
notfile hello_valgrind : @valgrind : hello ;
actions valgrind { valgrind $(>[1]) }
""")

t.write("hello.cpp", """\
#include <iostream>
int main() { std::cout << "Hello!\\n"; }
""")

t.run_build_system(["-n", "-d+2"])

t.fail_test(t.stdout().find("echo hi") == -1)

name = t.adjust_names("bin/$toolset/debug*/hello.exe")[0]
name = os.path.join(*name.split("/"))
t.expect_output_lines(" valgrind *%s " % name)

t.cleanup()
