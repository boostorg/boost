#!/usr/bin/python

# Copyright 2021 Dmitry Arkhipov (grisumbras@gmail.com)
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

# Regression test: it was possible that due to evaluation of conditional
# requirements, two different values of non-free features were present in a
# property set.

import BoostBuild

t = BoostBuild.Tester()

t.write("a.cpp", "")

t.write("jamroot.jam", """
alias dep : : <build>no ;
make a : dep : maker ;

actions maker
{
    this-command-is-unlikely-to-succeed ;
}

""")

t.run_build_system()

t.cleanup()
