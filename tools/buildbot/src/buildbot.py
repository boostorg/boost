#! /usr/bin/python

#~ Copyright Redshift Software, Inc. 2006-2007
#~ Distributed under the Boost Software License, Version 1.0. 
#~ (See accompanying file LICENSE_1_0.txt or copy at 
#~ http://www.boost.org/LICENSE_1_0.txt)

import sys
import os
import os.path
import urllib

from boost.patchwork import def_modules

#~ The directory this file is in.
root = os.path.abspath(os.path.dirname(os.path.realpath(__file__)))

#~ Download current packages.
#~ if not os.path.isdir(os.path.join(root,'_packages')):
    #~ os.mkdir(os.path.join(root,'_packages'))
#~ if not os.path.isdir(os.path.join(root,'_packages')):
    #~ sys.exit('No _packages directory found.')
#~ urllib.urlretrieve('',os.path.join(root,'_packages',''))
#~ urllib.urlretrieve('',os.path.join(root,'_packages',''))

#~ Bootstrap Boost package.
#~ sys.path.insert(0,os.path.join(root,'_packages','boost-0.0.1.zip'))

#~ The zip files we import from...

#~ BuildBot has a simple single package tree.
def_modules(
    [ os.path.join(root,'_packages'), 'buildbot-(\d+).(\d+).(\d+).zip' ],
    {   'buildbot' :
            '^buildbot-[^/]+/buildbot/__init__.py$' }
    )

#~ Twisted has a variety of split packages.
def_modules(
    [ os.path.join(root,'_packages'), 'Twisted-(\d+).(\d+).(\d+).zip' ],
    {   'twisted' :
            '^Twisted[^/]+/TwistedCore-[^/]+/twisted/$' ,
        'twisted.conch' :
            '^Twisted[^/]+/TwistedConch-[^/]+/twisted/conch/$' ,
        'twisted.lore' :
            '^Twisted[^/]+/TwistedLore-[^/]+/twisted/lore/$' ,
        'twisted.mail' :
            '^Twisted[^/]+/TwistedMail-[^/]+/twisted/mail/$' ,
        'twisted.names' :
            '^Twisted[^/]+/TwistedNames-[^/]+/twisted/names/$' ,
        'twisted.news' :
            '^Twisted[^/]+/TwistedNews-[^/]+/twisted/news/$' ,
        'twisted.runner' :
            '^Twisted[^/]+/TwistedRunner-[^/]+/twisted/runner/$' ,
        'twisted.web' :
            '^Twisted[^/]+/TwistedWeb-[^/]+/twisted/web/$' ,
        'twisted.words' :
            '^Twisted[^/]+/TwistedWords-[^/]+/twisted/words/$' ,
        'zope' :
            '^Twisted[^/]+/zope.interface-[^/]+/src/zope/$' }
    )

#~ Since we have our own extra slave commands they have to loaded before
#~ starting the slave deamon.
if len(sys.argv) > 1 and sys.argv[1] == 'start':
    import boost.bot.remote

#~ And run the buildbot frontend script.
from buildbot.scripts import runner
runner.run()
