
# Copyright (c) MetaCommunications, Inc. 2003-2004
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import urllib
import tarfile
import socket
import time
import getopt
import glob
import shutil
import os.path
import os
import traceback
import string
import sys

regression_root = os.path.abspath( os.path.dirname( sys.argv[0] ) )
regression_results = os.path.join( regression_root, 'results' )
regression_log = os.path.join( regression_results, 'bjam.log' )

boost_root = os.path.join( regression_root, 'boost' )
xsl_reports_dir = os.path.join( boost_root, 'tools', 'regression', 'xsl_reports' )
comment_path = os.path.join( regression_root, 'comment.html' )
timestamp_path = os.path.join( boost_root, 'timestamp' )

cvs_ext_command_line = 'cvs -d:ext:%(user)s@cvs.sourceforge.net:/cvsroot/boost -z9 %(command)s'
cvs_pserver_command_line = 'cvs -d:pserver:%(user)s@cvs.sourceforge.net:/cvsroot/boost -z9 %(command)s'

if sys.platform == 'win32': 
    bjam_name = 'bjam.exe'
    bjam_build_compiler = 'vc7'
    bjam_make_cmd = 'build.bat %s' % bjam_build_compiler
    bjam_location = 'bin.ntx86'
    process_jam_log_name = 'process_jam_log.exe'
    process_jam_log_toolset = 'vc7.1'
else:
    bjam_name = 'bjam'
    bjam_build_compiler = 'gcc'
    bjam_make_cmd = './build.sh %s' % bjam_build_compiler
    bjam_location = ''
    process_jam_log_name = "process_jam_log"
    process_jam_log_toolset = 'gcc'

bjam_path = os.path.join( regression_root, bjam_name )
process_jam_log_path = os.path.join( regression_root, process_jam_log_name )

utils = None


def log( message ):
    sys.stderr.write( '# %s\n' % message )
    sys.stderr.flush()


def rmtree( path ):
    if os.path.exists( path ):
        if sys.platform == 'win32':
            os.system( 'del /f /s /q "%s" >nul 2>&1' % path )
            shutil.rmtree( path )
        else:
            os.system( 'rm -f -r "%s"' % path )


def retry( f, args, max_attempts=2, sleep_secs=10 ):
    for attempts in range( max_attempts, -1, -1 ):
        try:
            return f( *args )
        except Exception, msg:
            log( '%s failed with message "%s"' % ( f.__name__, msg ) )
            if attempts == 0: 
                log( 'Giving up.' )
                raise

            log( 'Retrying (%d more attempts).' % attempts )
            time.sleep( sleep_secs )


def cleanup( args ):
    log( 'Cleaning up "%s" directory ...' % boost_root )
    rmtree( boost_root )
    
    log( 'Cleaning up "%s" directory ...' % regression_results )
    rmtree( regression_results )


def http_get( source_url, destination, proxies ):
    src = urllib.urlopen( source_url, proxies = proxies )
    f = open( destination, 'wb' )
    while True:
        data = src.read( 16*1024 )
        if len( data ) == 0: break
        f.write( data )

    f.close()
    src.close()


def download_boost_tarball( destination, tag, proxy ):
    site = 'www.boost-consulting.com'
    tarball_name = 'boost.tar.bz2'
    tarball_path = os.path.join( destination, tarball_name )

    log( "Downloading '%s' for tag %s from %s..."  % ( tarball_path, tag, site ) )
    if os.path.exists( tarball_path ):
        os.unlink( tarball_path )

    if proxy is None: proxies = None
    else:             proxies = { 'http' : proxy }
    http_get(
          'http://%s/%s' % ( site, tarball_name ) # ignore tag for now
        , tarball_path
        , proxies
        )

    return tarball_path


def unpack_tarball( tarball_path, destination ):
    log( 'Looking for old unpacked archives...' )
    old_boost_dirs =  glob.glob( os.path.join( destination, 'boost-*' ) )

    for old_boost_dir in old_boost_dirs:
        log( 'Deleting old directory %s.' % old_boost_dir ) 
        rmtree( old_boost_dir )

    log( 'Unpacking boost tarball ("%s")...' % tarball_path )
    tar = tarfile.open( tarball_path, 'r|bz2' )
    for tarinfo in tar:
        tar.extract(tarinfo)
    tar.close()

    boost_dir = glob.glob( os.path.join( destination, 'boost-*' ) )[0]
    log( '    Unpacked into directory "%s"' % boost_dir )
    
    if os.path.exists( boost_root ):
        log( 'Deleting "%s" directory...' % boost_root )
        rmtree( boost_root )

    log( 'Renaming "%s" into "%s"' % ( boost_dir, boost_root ) )
    os.rename( boost_dir, boost_root )


def cvs_command( user, command ):
    if user == 'anonymous':
        cmd = cvs_pserver_command_line % { 'user': user, 'command': command }
    else:
        cmd = cvs_ext_command_line % { 'user': user, 'command': command }
    
    log( 'Executing CVS command "%s"' % cmd )
    rc = os.system( cmd )
    if rc != 0:
        raise Exception( 'Cvs command "%s" failed with code %d' % ( cmd, rc ) )


def cvs_checkout( user, tag, args ):
    if tag != 'CVS-HEAD':
        command = 'checkout -r %s boost' % tag
    else:
        command = 'checkout boost'
    
    os.chdir( regression_root )
    retry( 
         cvs_command
       , ( user, command )
       , max_attempts=5
       )


def get_source( user, tag, proxy, args, **unused ):
    log( "Getting sources ..." )

    if user is not None:
        cvs_checkout( user, tag, args )
    else:
        tarball_path = download_boost_tarball( regression_root, tag, proxy )
        unpack_tarball( tarball_path, regression_root )

    open( timestamp_path, 'w' ).close()


def update_source( user, tag, proxy, args, **unused ):
    if user is not None:
        log( 'Updating sources...' )
        cvs_update( user, tag, args )
    else:
        get_source( user, tag, proxy, args )


def build_bjam_if_needed():
    global bjam_path
    if os.path.exists( bjam_path ):
        log( 'Found preinstalled "%s"; will use it.' % bjam_path )
        return
    
    log( 'Preinstalled "%s" is not found; building one...' % bjam_path )

    log( 'Locating bjam source directory...' )
    bjam_source_dir = os.path.join( boost_root, 'tools', 'build', 'jam_src' )
    
    if os.path.exists( bjam_source_dir ):
        log( 'Found bjam source directory "%s"' % bjam_source_dir )
        log( 'Building bjam using \"%s\"...' % bjam_build_compiler )
            
        log( "Building bjam (%s)" % bjam_make_cmd )
        utils.system( [ 
              'cd %s' % bjam_source_dir
            , bjam_make_cmd 
            ] )
    else:
        raise 'Could not find bjam source directory \"%s\"' % bjam_source_dir

    bjam_path = os.path.join( bjam_source_dir, bjam_location, bjam_name )
    if not os.path.exists( bjam_path ):
        raise 'Failed to find bjam (\"%s\") after build.' % bjam_path

    log( 'Bjam succesfully built in "%s" directory' % bjam_path )


def build_process_jam_log_if_needed():
    global process_jam_log_path
    if os.path.exists( process_jam_log_path ):
        log( 'Found preinstalled "%s"; will use it.' % process_jam_log_path )
        return
    
    log( 'Preinstalled "%s" is not found; building one.' % process_jam_log_path )
    
    process_jam_log_source_dir = os.path.join( boost_root, 'tools', 'regression', 'build' )

    log( 'Locating proces_jam_log source directory...' )
    if os.path.exists( process_jam_log_source_dir ):
        log( 'Found proces_jam_log source directory "%s"' % process_jam_log_source_dir )

        log( 'Building process_jam_log using toolset "%s"' % process_jam_log_toolset )
        utils.system( [ 
              'cd %s' % process_jam_log_source_dir
            , '%s -sTOOLS=%s' % ( bjam_path, process_jam_log_toolset )
            ] )

    else:
        raise 'Could not find process_jam_log source directory "%s"' % process_jam_log_source_dir

    process_jam_log_path = os.path.join( 
          boost_root, 'bin', 'boost', 'tools', 'regression', 'build'
        , process_jam_log_name, process_jam_log_toolset, 'release', process_jam_log_name
        )
    
    if not os.path.exists( process_jam_log_path ):
        raise 'Failed to find process_jam_log ("%s") after build.' % process_jam_log_path
    
    log( 'Process_jam_log succesfully built in "%s" directory' % process_jam_log_path )


def import_utils():
    global utils    
    if utils is None:
        sys.path.append( xsl_reports_dir )
        import utils as utils_module
        utils = utils_module


def setup(
          comment
        , args
        , **unused
        ):
    import_utils()
    
    if not 'no-bjam' in args:
        build_bjam_if_needed()
    
    if not 'no-process_jam_log' in args:
        build_process_jam_log_if_needed()


def tool_path( name ):
    return os.path.join( regression_results, name )

def start_build_monitor():
    if sys.platform == 'win32':
        build_monitor = tool_path( 'build_monitor.exe' )
        if os.path.exists( build_monitor ):
            utils.system( [ 'start "" %s bjam.exe %d' % ( build_monitor, 3*60 ) ] )


def stop_build_monitor():
    if sys.platform == 'win32':
        build_monitor = tool_path( 'build_monitor.exe' )
        if os.path.exists( build_monitor ):
            utils.system( [ '%s build_monitor' %  tool_path( 'pskill.exe' ) ] )


def process_bjam_log():
    log( 'Getting test case results out of "%s"...' % regression_log )
    utils.checked_system( [ 
        "%s %s <%s" % (  
              process_jam_log_path
            , regression_results
            , regression_log
            )
        ] )
    

def test( 
          toolsets
        , args
        , **unused
        ):
    if args == []:
        args = [ "test", "process" ]

    import_utils()

    try:
        start_build_monitor()
        cd = os.getcwd()
        os.chdir( os.path.join( boost_root, 'status' ) )

        log( 'Making "%s" directory...' % regression_results )
        utils.makedirs( regression_results )

        results_libs = os.path.join( regression_results, 'libs' )
        results_status = os.path.join( regression_results, 'status' )
        
        if "clean" in args:
            rmtree( results_libs )
            rmtree( results_status )

        if "test" in args:
            test_cmd = []
            if not toolsets is None:
                test_cmd.append( 'set TOOLS=%s' % string.join( string.split( toolsets, ',' ), ' ' ) )
            test_cmd.append( 'set BOOST_ROOT=%s' % boost_root )
            test_cmd.append( '"%s" -d2 --dump-tests "-sALL_LOCATE_TARGET=%s" >>%s 2>&1'
                                      % (     bjam_path
                                            , regression_results
                                            , regression_log
                                            )
                                      )

            utils.checked_system( test_cmd, [ 1 ] )

        if "process" in args:
            process_bjam_log()

        os.chdir( cd )
    finally:
        stop_build_monitor()


def collect_logs( 
          tag
        , runner
        , platform
        , user
        , comment
        , args
        , **unused
        ):
    import_utils()

    global comment_path
    if comment is None:
        log( 'Comment file "%s" not found; creating default comment.' % comment_path )
        f = open( comment_path, 'w' )
        f.write( '<p>Tests are run on %s platform.</p>' % string.capitalize( sys.platform ) )
        f.close()
    else: 
        comment_path = os.path.join( regression_root, comment )
    
    from runner import collect_logs
    collect_logs( 
          regression_results
        , runner
        , tag
        , platform
        , comment_path
        , time.strftime( 
              "%a, %d %b %Y %H:%M:%S +0000"
            , time.gmtime( os.stat( timestamp_path ).st_mtime )
            )
        , user
        )
        

def upload_logs( 
          tag
        , runner
        , user
        , **unused
        ):
    import_utils()
    from runner import upload_logs
    upload_logs( runner, tag, user )


def regression( 
          tag
        , runner
        , platform
        , user
        , comment
        , toolsets
        , incremental
        , mail = None
        , proxy = None
        , args = []
        ):

    try:
        mail_subject = "Boost regression for %s on %s \n" % ( tag, string.split(socket.gethostname(), '.')[0] )
        if incremental:
            update_source( user, tag, proxy, [] )
            setup( comment, [] )
        else:
            cleanup( args )
            get_source( user, tag, proxy, [] )
            setup( comment, [] )

        test( toolsets, [] )
        collect_logs( tag, runner, platform, user, comment, args )
        upload_logs( tag, runner, user )

        if mail:
            log( 'Sending report to "%s"' % mail )
            utils.send_mail( mail, mail_subject + ' completed successfully.' )
    except:
        if mail:
            log( 'Sending report to "%s"' % mail )
            msg = regression_log + [ "" ] + apply( traceback.format_exception, sys.exc_info() ) 
            utils.send_mail( mail, mail_subject + ' failed.', '\n'.join( msg ) )
        raise


def accept_args( args ):
    args_spec = [
          'tag='
        , 'runner='
        , 'platform='
        , 'user='
        , 'comment='
        , 'toolsets='
        , 'mail='
        , 'proxy='
        , 'incremental'
        , 'help'
        ]
    
    options = {
          '--tag' :         'CVS-HEAD'
        , '--platform' :    sys.platform
        , '--user' :        None
        , '--comment' :     None
        , '--toolsets' :    None
        , '--mail' :        None
        , '--proxy' :       None
        }
    
    defaults_num = len( options )

    ( option_pairs, other_args ) = getopt.getopt( args, '', args_spec )
    map( lambda x: options.__setitem__( x[0], x[1] ), option_pairs )

    if ( options.has_key( '--help' ) or len( options.keys() ) == defaults_num ):
        usage()
        sys.exit( 1 )

    return {
          'tag':            options[ '--tag' ]
        , 'runner':         options[ '--runner' ]
        , 'platform':       options[ '--platform']
        , 'user':           options[ '--user' ]
        , 'comment':        options[ '--comment' ]
        , 'toolsets':       options[ '--toolsets' ]
        , 'incremental':    options.has_key( '--incremental' )
        , 'mail':           options[ '--mail' ]
        , 'proxy':          options[ '--proxy' ]
        , 'args':           other_args
        }


commands = {
      'cleanup'         : cleanup
    , 'get-source'      : get_source
    , 'update-source'   : update_source
    , 'setup'           : setup
    , 'test'            : test
    , 'collect-logs'    : collect_logs
    , 'upload-logs'     : upload_logs
    , 'regression'      : regression
    }

def usage():
    print 'Usage:\n\t%s [command] options' % os.path.basename( sys.argv[0] )
    print    '''
Commands:
\t%s

Options:
\t--runner        runner ID (e.g. 'Metacomm')
\t--tag           the tag for the results ('CVS-HEAD' by default)
\t--comment       an html comment file (will be inserted in the reports, 
\t                'comment.html' by default)
\t--incremental   do incremental run (do not remove previous binaries)
\t--user          SourceForge user name for a shell/CVS account (optional)
\t--toolsets      comma-separated list of toolsets to test with (optional)
\t--mail          email address to send run notification to (optional)
\t--proxy         HTTP proxy server address and port (e.g. 
\t                'http://www.someproxy.com:3128', optional)
''' % '\n\t'.join( commands.keys() )

    print 'Example:\n\t%s --runner=Metacomm\n' % os.path.basename( sys.argv[0] )
    print 'For more documentation, see http://tinyurl.com/4f2zp\n'


if len(sys.argv) > 1 and sys.argv[1] in commands:
    command = sys.argv[1]
    args = sys.argv[ 2: ]
else:
    command = 'regression'
    args = sys.argv[ 1: ]

commands[ command ]( **accept_args( args ) )
