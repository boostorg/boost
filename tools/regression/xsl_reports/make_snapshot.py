
# Copyright (c) MetaCommunications, Inc. 2003-2004
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import tarfile
import shutil
import time
import os.path
import string
import sys
import traceback


def retry( f, args, max_attempts=2, sleep_secs=10 ):
    for attempts in range( max_attempts, -1, -1 ):
        try:
            return f( *args )
        except Exception, msg:
            utils.log( '%s failed with message "%s"' % ( f.__name__, msg ) )
            if attempts == 0: 
                utils.log( 'Giving up.' )
                raise

            utils.log( 'Retrying (%d more attempts).' % attempts )
            time.sleep( sleep_secs )


def rmtree( path ):
    if os.path.exists( path ):
        if sys.platform == 'win32':
            os.system( 'del /f /s /q "%s" >nul 2>&1' % path )
            shutil.rmtree( path )
        else:
            os.system( 'rm -f -r "%s"' % path )


def cvs_command( user, command ):
    cmd = 'cvs -d:ext:%(user)s@cvs.sourceforge.net:/cvsroot/boost -z9 %(command)s' \
            % { 'user': user, 'command': command }
    
    utils.log( 'Executing CVS command "%s"' % cmd )
    rc = os.system( cmd )
    if rc != 0:
        raise Exception( 'CVS command "%s" failed with code %d' % ( cmd, rc ) )



def cvs_export( working_dir, user, tag ):
    if tag != 'CVS-HEAD':
        command = 'export -r %s boost' % tag
    else:
        command = 'export -r HEAD boost'

    os.chdir( working_dir )
    retry( 
         cvs_command
       , ( user, command )
       , max_attempts=5
       )


def make_tarball(
          working_dir
        , tag
        , user
        , site_dir
        ):

    sources_dir = os.path.join( working_dir, 'boost' )
    if os.path.exists( sources_dir ):
        utils.log( 'Directory "%s" already exists, cleaning it up...' % sources_dir )
        rmtree( sources_dir )

    try:
        os.mkdir( sources_dir )
        utils.log( 'Exporting files from CVS...' )
        cvs_export( working_dir, user, tag )
    except:
        utils.log( 'Cleaning up...' )
        rmtree( sources_dir )
        raise

    timestamped_dir_name = 'boost-%s-%s' % ( tag, time.strftime( '%y-%m-%d-%H%M', time.gmtime() ) )
    timestamped_dir = os.path.join( working_dir, timestamped_dir_name )

    utils.log( 'Renaming "%s" to "%s"...' % ( sources_dir, timestamped_dir ) )
    os.rename( sources_dir, timestamped_dir )

    tarball_name = 'boost-%s.tar.bz2' % tag
    tarball_path = os.path.join( working_dir, tarball_name )

    utils.log( 'Archiving "%s" to "%s"...' % ( timestamped_dir, tarball_path ) )
    tar = tarfile.open( tarball_path, 'w|bz2' )
    tar.posix = False # see http://tinyurl.com/4ebd8

    tar.add( timestamped_dir, timestamped_dir_name )
    tar.close()

    if site_dir is not None:
        utils.log( 'Moving "%s" to the site location "%s"...' % ( tarball_name, site_dir ) )
        shutil.move( tarball_path, site_dir )
        utils.log( 'Removing "%s"...' % timestamped_dir )
        rmtree( timestamped_dir )


def accept_args( args ):
    args_spec = [ 
          'working-dir='
        , 'tag='
        , 'user='
        , 'site-dir='
        , 'mail='
        , 'help'
        ]
        
    options = { 
          '--tag': 'CVS-HEAD'
        , '--site-dir': None
        }
    
    utils.accept_args( args_spec, args, options, usage )
        
    return ( 
          options[ '--working-dir' ]
        , options[ '--tag' ]
        , options[ '--user' ]
        , options[ '--site-dir' ]
        )


def usage():
    print 'Usage: %s [options]' % os.path.basename( sys.argv[0] )
    print    '''
\t--working-dir   working directory
\t--tag           snapshot tag (i.e. 'CVS-HEAD')
\t--user          SourceForge user name for a CVS account
\t--site-dir      site directory to copy the snapshot to (optional)
'''

def main():
    make_tarball( *accept_args( sys.argv[ 1: ] ) )

if __name__ != '__main__':  import utils
else:
    # in absense of relative import...
    xsl_path = os.path.abspath( os.path.dirname( sys.argv[ 0 ] ) )
    while os.path.basename( xsl_path ) != 'xsl_reports': xsl_path = os.path.dirname( xsl_path )
    sys.path.append( xsl_path )

    import utils
    main()
