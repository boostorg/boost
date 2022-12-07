#!/usr/bin/python
#
# Copyright 2017 Steven Watanabe
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

from MockProgram import *

command('libtool', '-static', '-o', output_file('bin/darwin-4.2.1/debug/link-static/target-os-darwin/libl1.a'), input_file('bin/darwin-4.2.1/debug/link-static/target-os-darwin/lib.o'))
command('libtool', '-static', '-o', output_file('bin/darwin-4.2.1/debug/link-static/runtime-link-static/target-os-darwin/libl1.a'), input_file('bin/darwin-4.2.1/debug/link-static/runtime-link-static/target-os-darwin/lib.o'))

main()
