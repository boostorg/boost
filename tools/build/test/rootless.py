#!/usr/bin/python

# Copyright 2018 Rene Rivera
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild
import os

t = BoostBuild.Tester(translate_suffixes=0)

t.set_tree("rootless/test1")
t.run_build_system(status=1)
t.expect_output_lines("error: no Jamfile in current directory*")

t.set_tree("rootless/test1")
t.run_build_system(subdir="sub_root")
t.expect_addition("sub_root/bin/a.txt")

t.set_tree("rootless/test1")
t.run_build_system(subdir="sub_root", extra_args=["--build-dir=../bin"])
t.expect_output_lines("warning: the --build-dir option will be ignored")

t.set_tree("rootless/test2")
t.run_build_system(subdir="sub_root", extra_args=["--build-dir=../bin"])
t.expect_addition("bin/foo/a.txt")

t.set_tree("rootless/test3")
t.run_build_system()

t.set_tree("rootless/test3")
t.run_build_system(subdir="sub/inner")
t.expect_addition("bins/sub/inner/a.txt")

t.cleanup()
