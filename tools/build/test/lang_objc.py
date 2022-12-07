#!/usr/bin/env python

# Copyright Nikita Kniazev 2021.
# Distributed under the Boost Software License, Version 1.0. (See
# accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild

t = BoostBuild.Tester()

t.write("jamroot.jam", """
obj a : hello.m ;
obj b : hello.mm ;
""")

t.write("hello.m", '''\
@interface Foo
@end
''')
t.write("hello.mm", '''\
@interface Foo
@end

class Bar {};
''')

t.run_build_system()
t.expect_addition("bin/$toolset/debug*/a.obj")
t.expect_addition("bin/$toolset/debug*/b.obj")
t.expect_nothing_more()

t.cleanup()
