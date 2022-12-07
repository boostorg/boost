#!/usr/bin/python
# coding: utf-8
#
# Copyright 2017-2018 Steven Watanabe
# Copyright 2020 René Ferdinand Rivera Morell
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

from MockProgram import *

command('ar', 'rsc', output_file('bin/gcc-gnu-4.8.3/debug/link-static/libl1.a'), input_file('bin/gcc-gnu-4.8.3/debug/link-static/lib.o'))
command('ar', 'rsc', output_file('bin/gcc-gnu-4.8.3/debug/link-static/runtime-link-static/libl1.a'), input_file('bin/gcc-gnu-4.8.3/debug/link-static/runtime-link-static/lib.o'))
command('ar', 'rsc', output_file('bin/gcc-darwin-4.2.1/debug/link-static/target-os-darwin/libl1.a'), input_file('bin/gcc-darwin-4.2.1/debug/link-static/target-os-darwin/lib.o'))
command('ar', 'rsc', output_file('bin/gcc-darwin-4.2.1/debug/link-static/runtime-link-static/target-os-darwin/libl1.a'), input_file('bin/gcc-darwin-4.2.1/debug/link-static/runtime-link-static/target-os-darwin/lib.o'))
command('ar', 'rsc', output_file('bin/clang-darwin-3.9.0/debug/link-static/target-os-darwin/libl1.a'), input_file('bin/clang-darwin-3.9.0/debug/link-static/target-os-darwin/lib.o'))
command('ar', 'rsc', output_file('bin/clang-darwin-3.9.0/debug/link-static/runtime-link-static/target-os-darwin/libl1.a'), input_file('bin/clang-darwin-3.9.0/debug/link-static/runtime-link-static/target-os-darwin/lib.o'))
command('ar', 'rsc', output_file('bin/intel-darwin-10.2/debug/link-static/target-os-darwin/libl1.a'), input_file('bin/intel-darwin-10.2/debug/link-static/target-os-darwin/lib.o'))
command('ar', 'rsc', output_file('bin/intel-darwin-10.2/debug/link-static/runtime-link-static/target-os-darwin/libl1.a'), input_file('bin/intel-darwin-10.2/debug/link-static/runtime-link-static/target-os-darwin/lib.o'))
command('ar', 'rsc', output_file('bin/clang-linux-3.9.0/debug/link-static/libl1.a'), input_file('bin/clang-linux-3.9.0/debug/link-static/lib.o'))
command('ar', 'rsc', output_file('bin/clang-linux-3.9.0/debug/link-static/runtime-link-static/libl1.a'), input_file('bin/clang-linux-3.9.0/debug/link-static/runtime-link-static/lib.o'))
command('ar', 'rcu', output_file('bin/clang-vxworks-4.0.1/debug/link-static/libl1.a'), input_file('bin/clang-vxworks-4.0.1/debug/link-static/lib.o'))
command('ar', 'rcu', output_file('bin/clang-vxworks-4.0.1/debug/link-static/runtime-link-static/libl1.a'), input_file('bin/clang-vxworks-4.0.1/debug/link-static/runtime-link-static/lib.o'))
command('ar', 'rsc', output_file('bin/clang-linux-3.9.0/debug/link-static/target-os-windows/libl1.lib'), input_file('bin/clang-linux-3.9.0/debug/link-static/target-os-windows/lib.obj'))

main()
