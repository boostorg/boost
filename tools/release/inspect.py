# Inspect Boost source tree and upload results to ftp site
# Copyright Beman Dawes 2008, 2009, 2012

# Distributed under the Boost Software License, Version 1.0.
# See http://www.boost.org/LICENSE_1_0.txt

import os
import os.path
import subprocess
from ftplib import FTP
import argparse

def cmnd(x):  # execute a command
  rc = subprocess.call(x)
  if rc != 0:
    print 'aborting command:'
    print x
    exit(rc)
    
argpsr = argparse.ArgumentParser(
  description='Inspect Boost source tree and upload results to ftp site',
  epilog='Example: python inspect.py --user foo --pass bar --file inspect-trunk.html')
argpsr.add_argument('--host', help='URL of ftp site. Default: boost.cowic.de',
                    default='boost.cowic.de')
argpsr.add_argument('--user', help='User name for ftp site', required=True)
argpsr.add_argument('--password', help='Password for ftp site', required=True)
argpsr.add_argument('--filename', help='Filename for ftp site.', required=True)
args = argpsr.parse_args()
filepath = 'temp/' + args.filename  

if not os.path.exists('.svn'):
  print 'Error: must be run in the boost root directory'
  exit(1)
print 'Boost root directory detected'

if not os.path.exists('temp'):
  print 'Create temp directory...'
  os.mkdir('temp')
  open('temp/boost-no-inspect', 'w').close()

# cleanup clears locks or other residual problems (learned this the hard way!)
print 'Clean working copy ...'
cmnd(['svn', 'cleanup'])

print 'Update working copy...'
cmnd(['svn', 'up', '--non-interactive', '--trust-server-cert'])

print 'Build inspect program...'
cmnd(['bjam', 'tools/inspect/build'])

print 'Running inspect from ' + os.getcwd()
subprocess.call('dist/bin/inspect', stdout=open(filepath, 'w'))

print 'Upload web page via ftp...'
ftp = FTP(args.host, args.user, args.password)
ftp.retrlines('LIST')
ftp.storlines('STOR ' + args.filename, open(filepath)) 
ftp.retrlines('LIST')
ftp.quit()

print "Inspect and upload complete"

