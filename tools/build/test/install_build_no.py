#!/usr/bin/python

# Copyright 2021 Dmitry Arkhipov (grisumbras@gmail.com)
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

# Check that <build>no in usage-requirements of dependencies does not affect
# install rule, i.e. a skipped installed target does not affect insallation of
# other targets.

import BoostBuild

t = BoostBuild.Tester()

t.write("a.cpp", "int main() {}\n")

t.write("jamroot.jam", """
make x : : maker : <build>no ;
exe a : a.cpp ;
install install : x a ;
""")

t.run_build_system()
t.expect_addition("install/a.exe")

t.cleanup()
