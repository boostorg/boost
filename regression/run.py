#!/usr/bin/python

# Copyright Redshift Software, Inc. 2007
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import os.path
import sys

#~ The directory this file is in.
root = os.path.abspath(os.path.dirname(os.path.dirname(os.path.realpath(__file__))))
print '--- %s' % root

#~ Bootstrap
sys.path.insert(0,root)

#~ Launch runner
from src.regression import runner
runner(root)
