
# Copyright (c) MetaCommunications, Inc. 2003-2004
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import urllib
import tarfile
import zipfile
import socket
import time
import getopt
import glob
import shutil
import stat
import os.path
import os
import traceback
import string
import sys

regression_root = os.path.abspath( os.path.dirname( sys.argv[0] ) )
regression_results = os.path.join( regression_root, 'results' )
regression_log = os.path.join( regression_results, 'bjam.log' )
install_log = os.path.join( regression_results, 'bjam_install.log' )

boost_root = os.path.join( regression_root, 'boost' )
xsl_reports_dir = os.path.join( boost_root, 'tools', 'regression', 'xsl_reports' )
comment_path = os.path.join( regression_root, 'comment.html' )
timestamp_path = os.path.join( regression_root, 'timestamp' )

cvs_command_line = 'cvs -z9 %(command)s'
cvs_ext_command_line = 'cvs -d:ext:%(user)s@cvs.sourceforge.net:/cvsroot/boost -z9 %(command)s'
cvs_pserver_command_line = 'cvs -d:pserver:%(user)s@cvs.sourceforge.net:/cvsroot/boost -z9 %(command)s'

bjam = {}
process_jam_log = {}

if sys.platform == 'win32':
    bjam[ 'name' ] = 'bjam.exe'
    bjam[ 'build_cmd' ] = lambda toolset: 'build.bat %s' % toolset
    bjam[ 'location' ] = 'bin.ntx86'
    process_jam_log[ 'name' ] = 'process_jam_log.exe'
    process_jam_log[ 'default_toolset' ] = 'vc7.1'
    patch_boost_name = 'patch_boost.bat'
else:
    bjam[ 'name' ] = 'bjam'
    bjam[ 'build_cmd' ] = lambda toolset:'./build.sh %s' % toolset
    bjam[ 'location' ] = ''
    process_jam_log[ 'name' ] = 'process_jam_log'
    process_jam_log[ 'default_toolset' ] = 'gcc'
    patch_boost_name = './patch_boost'

bjam[ 'default_toolset' ] = ''
bjam[ 'path' ] = os.path.join( regression_root, bjam[ 'name' ] )
bjam[ 'source_dir' ] = os.path.join( boost_root, 'tools', 'build', 'jam_src' )
bjam[ 'build_path' ] = os.path.join( bjam[ 'source_dir' ], bjam[ 'location' ], bjam[ 'name' ] )

process_jam_log[ 'path' ] = os.path.join( regression_root, process_jam_log[ 'name' ] )
process_jam_log[ 'source_dir' ] = os.path.join( boost_root, 'tools', 'regression', 'build' )
process_jam_log[ 'build_path_root' ] = os.path.join( 
      boost_root, 'bin', 'boost', 'tools', 'regression', 'build'
    , process_jam_log[ 'name' ]
    )

process_jam_log[ 'build_cmd' ] = lambda toolset:'%s -sTOOLS=%s'% ( tool_path( bjam ), toolset )

build_monitor_url = 'http://www.meta-comm.com/engineering/boost-regression/build_monitor.zip'
pskill_url = 'http://www.sysinternals.com/files/pskill.zip'

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


def cleanup( args, **unused ):
    if args == []: args = [ 'source', 'bin' ]

    if 'source' in args:
        log( 'Cleaning up "%s" directory ...' % boost_root )
        rmtree( boost_root )
    
    if 'bin' in args:
        log( 'Cleaning up "%s" directory ...' % regression_results )
        rmtree( regression_results )


def http_get( source_url, destination, proxy ):
    if proxy is None: proxies = None
    else:             proxies = { 'http' : proxy }

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

    http_get(
          'http://%s/%s' % ( site, tarball_name ) # ignore tag for now
        , tarball_path
        , proxy
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
        tar.extract( tarinfo, destination )        
        if sys.platform == 'win32' and not tarinfo.isdir():
            # workaround what appears to be a Win32-specific bug in 'tarfile'
            # (modification times for extracted files are not set properly)
            f = os.path.join( destination, tarinfo.name )
            os.chmod( f, stat.S_IWRITE )
            os.utime( f, ( tarinfo.mtime, tarinfo.mtime ) )
    
    tar.close()

    boost_dir = glob.glob( os.path.join( destination, 'boost-*' ) )[0]
    log( '    Unpacked into directory "%s"' % boost_dir )
    
    if os.path.exists( boost_root ):
        log( 'Deleting "%s" directory...' % boost_root )
        rmtree( boost_root )

    log( 'Renaming "%s" into "%s"' % ( boost_dir, boost_root ) )
    os.rename( boost_dir, boost_root )


def cvs_command( user, command ):
    if user is None:
        cmd = cvs_command_line % { 'command': command }
    elif user == 'anonymous':
        cmd = cvs_pserver_command_line % { 'user': user, 'command': command }
    else:
        cmd = cvs_ext_command_line % { 'user': user, 'command': command }
    
    log( 'Executing CVS command "%s"' % cmd )
    rc = os.system( cmd )
    if rc != 0:
        raise Exception( 'CVS command "%s" failed with code %d' % ( cmd, rc ) )


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


def cvs_update( user, tag, args ):
    if tag != 'CVS-HEAD':
        command = 'update -dPA -r %s' % tag
    else:
        command = 'update -dPA'
    
    os.chdir( os.path.join( regression_root, 'boost' ) )
    retry( 
         cvs_command
       , ( user, command )
       , max_attempts=5
       )


def format_time( t ):
    return time.strftime( 
          '%a, %d %b %Y %H:%M:%S +0000'
        , t
        )

def timestamp():
    return format_time( 
          time.gmtime( os.stat( timestamp_path ).st_mtime )
        )


def get_source( user, tag, proxy, args, **unused ):
    open( timestamp_path, 'w' ).close()
    log( 'Getting sources (%s)...' % timestamp() )

    if user is not None:
        cvs_checkout( user, tag, args )
    else:
        tarball_path = download_boost_tarball( regression_root, tag, proxy )
        unpack_tarball( tarball_path, regression_root )


def update_source( user, tag, proxy, args, **unused ):
    if user is not None or os.path.exists( os.path.join( boost_root, 'CVS' ) ):
        open( timestamp_path, 'w' ).close()
        log( 'Updating sources from CVS (%s)...' % timestamp() )
        cvs_update( user, tag, args )
    else:
        get_source( user, tag, proxy, args )


def build_if_needed( tool, toolset, toolsets ):
    if os.path.exists( tool[ 'path' ] ):
        log( 'Found preinstalled "%s"; will use it.' % tool[ 'path' ] )
        return

    log( 'Preinstalled "%s" is not found; building one...' % tool[ 'path' ] )

    if toolset is None:
        if toolsets is not None:
            toolset = string.split( toolsets, ',' )[0]
        else:
            toolset = tool[ 'default_toolset' ]
            log( 'Warning: No bootstrap toolset for "%s" was specified.' % tool[ 'name' ] )
            log( '         Using default toolset for the platform (%s).' % toolset )

    if os.path.exists( tool[ 'source_dir' ] ):
        log( 'Found "%s" source directory "%s"' % ( tool[ 'name' ], tool[ 'source_dir' ] ) )
        build_cmd = tool[ 'build_cmd' ]( toolset )
        log( 'Building "%s" (%s)...' % ( tool[ 'name'], build_cmd ) )
        utils.system( [ 
              'cd %s' % tool[ 'source_dir' ]
            , build_cmd
            ] )
    else:
        raise 'Could not find "%s" source directory "%s"' % ( tool[ 'name' ], tool[ 'source_dir' ] )

    if not tool.has_key( 'build_path' ):
        tool[ 'build_path' ] = os.path.join(
              tool[ 'build_path_root' ]
            , toolset, 'release', tool[ 'name' ]
            )

    if not os.path.exists( tool[ 'build_path' ] ):
        raise 'Failed to find "%s" after build.' % tool[ 'build_path' ]

    log( '%s succesfully built in "%s" directory' % ( tool[ 'name' ], tool[ 'build_path' ] ) )


def import_utils():
    global utils
    if utils is None:
        sys.path.append( xsl_reports_dir )
        import utils as utils_module
        utils = utils_module


def tool_path( name_or_spec ):
    if isinstance( name_or_spec, basestring ):
        return os.path.join( regression_root, name_or_spec )

    if os.path.exists( name_or_spec[ 'path' ] ):
        return name_or_spec[ 'path' ]

    if name_or_spec.has_key( 'build_path' ):
        return name_or_spec[ 'build_path' ]

    build_path_root = name_or_spec[ 'build_path_root' ]
    log( 'Searching for "%s" in "%s"...' % ( name_or_spec[ 'name' ], build_path_root ) )
    for root, dirs, files in os.walk( build_path_root ):
        if name_or_spec[ 'name' ] in files:
            return os.path.join( root, name_or_spec[ 'name' ] )
    
    raise Exception( 'Cannot find "%" in any of the following locations:\n%s' % (
          '\n'.join( [ name_or_spec[ 'path' ], build_path_root ] )
        ) )


def download_if_needed( tool_name, tool_url, proxy ):
    path = tool_path( tool_name )
    if not os.path.exists( path ):
        log( 'Preinstalled "%s" is not found.' % path )
        log( '  Downloading from %s...' % tool_url )
        
        zip_path = '%s.zip' % path
        http_get( tool_url, zip_path, proxy )

        log( '  Unzipping %s...' % path )
        utils.unzip( zip_path, os.path.dirname( path ) )

        log( '  Removing %s...' % zip_path )
        os.unlink( zip_path )
        log( 'Done.' )


def setup(
          comment
        , toolsets
        , bjam_toolset
        , pjl_toolset
        , monitored
        , proxy
        , args
        , **unused
        ):
    import_utils()
    
    if os.path.exists( patch_boost_name ):
        log( 'Found patch file "%s". Executing it.' % patch_boost_name )
        utils.system( [ patch_boost_name ] )

    build_if_needed( bjam, bjam_toolset, toolsets )
    build_if_needed( process_jam_log, pjl_toolset, toolsets )
    
    if monitored:
        if sys.platform == 'win32':
            download_if_needed( 'build_monitor.exe', build_monitor_url, proxy )
            download_if_needed( 'pskill.exe', pskill_url, proxy )
        else:
            log( 'Warning: Test monitoring is not supported on this platform (yet).' )
            log( '         Please consider contributing this piece!' )


def bjam_command( toolsets ):
    result = '"%s"' % tool_path( bjam )
    if not toolsets is None:
        result += ' "-sTOOLS=%s"' % string.join( string.split( toolsets, ',' ), ' ' )
    result += ' "-sBOOST_ROOT=%s"' % boost_root
    return result


def install( toolsets, **unused ):
    import_utils()
    os.chdir( os.path.join( boost_root ) )

    log( 'Making "%s" directory...' % regression_results )
    utils.makedirs( regression_results )
    
    install_cmd = '%s -d2 install >>%s 2>&1' % ( bjam_command( toolsets ), install_log )
    log( 'Installing libraries (%s)...' % install_cmd )
    utils.system( [ install_cmd ] )


def start_build_monitor( timeout ):
    if sys.platform == 'win32':
        build_monitor_path = tool_path( 'build_monitor.exe' )
        if os.path.exists( build_monitor_path ):
            utils.system( [ 'start /belownormal %s bjam.exe %d' % ( build_monitor_path, timeout*60 ) ] )
        else:
            log( 'Warning: Build monitor is not found at "%s"' % build_monitor_path )


def stop_build_monitor():
    if sys.platform == 'win32':
        build_monitor_path = tool_path( 'build_monitor.exe' )
        if os.path.exists( build_monitor_path ):
            utils.system( [ '%s build_monitor' %  tool_path( 'pskill.exe' ) ] )


def run_process_jam_log():
    log( 'Getting test case results out of "%s"...' % regression_log )

    utils.checked_system( [ 
        '%s %s <%s' % (
              tool_path( process_jam_log )
            , regression_results
            , regression_log
            )
        ] )
    

def test( 
          toolsets
        , monitored
        , timeout
        , args
        , **unused
        ):
    if args == []:
        args = [ "test", "process" ]

    import_utils()

    try:
        if monitored:
            start_build_monitor( timeout )

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
            test_cmd = '%s -d2 --dump-tests "-sALL_LOCATE_TARGET=%s" >>%s 2>&1' % (
                  bjam_command( toolsets )
                , regression_results
                , regression_log
                )

            log( 'Starting tests (%s)...' % test_cmd )
            utils.system( [ test_cmd ] )

        if "process" in args:
            run_process_jam_log()

        os.chdir( cd )
    finally:
        if monitored:
            stop_build_monitor()


def collect_logs( 
          tag
        , runner
        , platform
        , user
        , comment
        , incremental
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
    
    run_type = ''
    if incremental: run_type = 'incremental'
    else:           run_type = 'full'

    source = 'tarball'
    cvs_root_file = os.path.join( boost_root, 'CVS', 'root' )
    if os.path.exists( cvs_root_file ):
        if string.split( open( cvs_root_file ).readline(), '@' )[0] == ':pserver:anonymous':
            source = 'anonymous CVS'
        else:
            source = 'CVS'
   
    from runner import collect_logs
    collect_logs( 
          regression_results
        , runner
        , tag
        , platform
        , comment_path
        , timestamp_path
        , user
        , source
        , run_type
        )
        

def upload_logs( 
          tag
        , runner
        , user
        , **unused
        ):
    import_utils()
    from runner import upload_logs
    upload_logs( regression_results, runner, tag, user )


def update_itself( **unused ):
    source = os.path.join( xsl_reports_dir, 'runner', os.path.basename( sys.argv[0] ) )
    log( 'Updating %s from %s...' % ( sys.argv[0], source )  )
    log( '    Checking modification dates...' )
    if os.stat( sys.argv[0] ).st_mtime > os.stat( source ).st_mtime:
        log( 'Warning: The current version of script appears to be newer than the source.' )
        log( '         Update skipped.' )
    else:
        log( '    Saving a backup copy of the current script...' )
        shutil.move( sys.argv[0], '%s~' % sys.argv[0] )
        log( '    Replacing %s with a newer version...' % sys.argv[0] )
        shutil.copy2( source, sys.argv[0] )


def regression( 
          tag
        , runner
        , platform
        , user
        , comment
        , toolsets
        , bjam_toolset
        , pjl_toolset
        , incremental
        , monitored
        , timeout
        , mail = None
        , proxy = None
        , args = []
        ):

    try:
        mail_subject = 'Boost regression for %s on %s ' % ( tag, string.split(socket.gethostname(), '.')[0] )
        start_time = time.localtime()
        if mail:
            log( 'Sending start notification to "%s"' % mail )
            utils.send_mail( mail, mail_subject + ' started at %s.' % format_time( start_time ) )

        if incremental:
            update_source( user, tag, proxy, [] )
        else:
            cleanup( [] )
            get_source( user, tag, proxy, [] )

        setup( comment, toolsets, bjam_toolset, pjl_toolset, monitored, proxy, [] )
        test( toolsets, monitored, timeout, [] )
        collect_logs( tag, runner, platform, user, comment, incremental, [] )
        upload_logs( tag, runner, user )
        update_itself()
        
        if mail:
            log( 'Sending report to "%s"' % mail )
            end_time = time.localtime()
            utils.send_mail( 
                  mail
                , mail_subject + ' completed successfully at %s.' % format_time( end_time )
                )
    except:
        if mail:
            log( 'Sending report to "%s"' % mail )
            msg = regression_log + [ '' ] + apply( traceback.format_exception, sys.exc_info() ) 
            end_time = time.localtime()
            utils.send_mail( 
                  mail
                , mail_subject + ' failed at %s.' % format_time( end_time )
                , '\n'.join( msg )
                )
        raise


def show_revision( **unused ):
    modified = '$Date$'
    revision = '$Revision$'

    import re
    re_keyword_value = re.compile( r'^\$\w+:\s+(.*)\s+\$$' )
    print '\n\tResivion: %s' % re_keyword_value.match( revision ).group( 1 )
    print '\tLast modified on: %s\n' % re_keyword_value.match( modified ).group( 1 )


def accept_args( args ):
    args_spec = [
          'tag='
        , 'runner='
        , 'platform='
        , 'user='
        , 'comment='
        , 'toolsets='
        , 'bjam-toolset='
        , 'pjl-toolset='
        , 'timeout='
        , 'mail='
        , 'proxy='
        , 'incremental'
        , 'monitored'
        , 'help'
        ]
    
    options = {
          '--tag'           : 'CVS-HEAD'
        , '--platform'      : sys.platform
        , '--user'          : None
        , '--comment'       : None
        , '--toolsets'      : None
        , '--bjam-toolset'  : None
        , '--pjl-toolset'   : None
        , '--timeout'       : 5
        , '--mail'          : None
        , '--proxy'         : None
        }
    
    defaults_num = len( options )

    ( option_pairs, other_args ) = getopt.getopt( args, '', args_spec )
    map( lambda x: options.__setitem__( x[0], x[1] ), option_pairs )

    if ( options.has_key( '--help' ) or len( options.keys() ) == defaults_num ):
        usage()
        sys.exit( 1 )

    return {
          'tag'             : options[ '--tag' ]
        , 'runner'          : options[ '--runner' ]
        , 'platform'        : options[ '--platform']
        , 'user'            : options[ '--user' ]
        , 'comment'         : options[ '--comment' ]
        , 'toolsets'        : options[ '--toolsets' ]
        , 'bjam_toolset'    : options[ '--bjam-toolset' ]
        , 'pjl_toolset'     : options[ '--pjl-toolset' ]
        , 'incremental'     : options.has_key( '--incremental' )
        , 'monitored'       : options.has_key( '--monitored' )
        , 'timeout'         : options[ '--timeout' ]
        , 'mail'            : options[ '--mail' ]
        , 'proxy'           : options[ '--proxy' ]
        , 'args'            : other_args
        }


commands = {
      'cleanup'         : cleanup
    , 'get-source'      : get_source
    , 'update-source'   : update_source
    , 'setup'           : setup
    , 'install'         : install
    , 'test'            : test
    , 'collect-logs'    : collect_logs
    , 'upload-logs'     : upload_logs
    , 'update-itself'   : update_itself
    , 'regression'      : regression
    , 'show-revision'   : show_revision
    }

def usage():
    print 'Usage:\n\t%s [command] options' % os.path.basename( sys.argv[0] )
    print    '''
Commands:
\t%s

Options:
\t--runner        runner ID (e.g. 'Metacomm')
\t--tag           the tag for the results ('CVS-HEAD' by default)
\t--comment       an HTML comment file to be inserted in the reports
\t                ('comment.html' by default)
\t--incremental   do incremental run (do not remove previous binaries)
\t--monitored     do a monitored run
\t--timeout       specifies the timeout, in minutes, for a single test
\t                run/compilation (enforced only in monitored runs, 5 by 
\t                default)
\t--user          SourceForge user name for a shell/CVS account (optional)
\t--toolsets      comma-separated list of toolsets to test with (optional)
\t--bjam-toolset  bootstrap toolset for 'bjam' executable (optional)
\t--pjl-toolset   bootstrap toolset for 'process_jam_log' executable
\t                (optional)
\t--mail          email address to send run notification to (optional)
\t--proxy         HTTP proxy server address and port (e.g. 
\t                'http://www.someproxy.com:3128', optional)
''' % '\n\t'.join( commands.keys() )

    print 'Example:\n\t%s --runner=Metacomm\n' % os.path.basename( sys.argv[0] )
    print 'For more documentation, see http://tinyurl.com/4f2zp\n'


if len(sys.argv) > 1 and sys.argv[1] in commands:
    command = sys.argv[1]
    args = sys.argv[ 2: ]
    if command not in [ 'collect-logs', 'upload-logs' ]:
        args.insert( 0, '--runner=' )
else:
    command = 'regression'
    args = sys.argv[ 1: ]
    
commands[ command ]( **accept_args( args ) )
