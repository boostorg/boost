#!/usr/bin/python

# Copyright (C) Vladimir Prus 2010.
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test that build failure results in non-zero exit status

import BoostBuild

# Create a temporary working directory.
t = BoostBuild.Tester()

# Create the needed files.
t.write("jamroot.jam", """
exe hello : hello.cpp ;
""")

t.write("hello.cpp", """
int main() {
""")

t.run_build_system(status=1)

t.cleanup()
