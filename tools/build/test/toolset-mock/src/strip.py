#!/usr/bin/python
#
# Copyright 2017 Steven Watanabe
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

from MockProgram import *

command('strip', '-S', '-x', input_file('bin/darwin-4.2.1/release/target-os-darwin/test'))

main()
