#!/usr/bin/python

# Copyright 2007 Rene Rivera.
# Copyright 2011 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild

t = BoostBuild.Tester(pass_toolset=0)

t.write("file.jam", """\
actions quietly .a. { $(ACTION) }

rule .a.
{
    DEPENDS $(<) : $(>) ;
}

NOTFILE subtest ;
.a. subtest_a : subtest ;
DEPENDS all : subtest_a ;
""")

t.run_build_system(["-ffile.jam", "-sACTION=invalid"], status=1)

t.cleanup()
