# Inspect Trunk
# Copyright Beman Dawes 2008, 2009, 2012

# Distributed under the Boost Software License, Version 1.0.
# See http://www.boost.org/LICENSE_1_0.txt

import os
import os.path
import subprocess

def cmnd(x):  # execute a command
  rc = subprocess.call(x)
  if rc != 0:
    print 'aborting command:'
    print x
    exit(rc)

if not os.path.exists('.svn'):
  print 'Error: must be run in the boost root directory'
  exit(1)
print 'Boost root directory detected'

# cleanup clears locks or other residual problems (learned this the hard way!)
print 'Clean working copy ...'
cmnd(['svn', 'cleanup'])

print 'Update working copy...'
cmnd(['svn', 'up', '--non-interactive', '--trust-server-cert'])

print 'Build inspect program...'
old_dir = os.getcwd()
os.chdir(['tools/inspect/build'])
cmnd(['bjam'])
os.chdir(old_dir)

print 'Running inspect from ' + os.getcwd()
cmnd(['dist/bin/inspect'])
