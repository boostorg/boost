
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
    bjam[ 'toolset/compiler' ] = 'vc7'
    bjam[ 'build_cmd' ] = 'build.bat %s' % bjam[ 'toolset/compiler' ]
    bjam[ 'location' ] = 'bin.ntx86'
    process_jam_log[ 'name' ] = 'process_jam_log.exe'
    process_jam_log[ 'toolset/compiler' ] = 'vc7.1'
    patch_boost_name = 'patch_boost.bat'
else:
    bjam[ 'name' ] = 'bjam'
    bjam[ 'toolset/compiler' ] = 'gcc'
    bjam[ 'build_cmd' ] = './build.sh %s' % bjam[ 'toolset/compiler' ]
    bjam[ 'location' ] = ''
    process_jam_log[ 'name' ] = "process_jam_log"
    process_jam_log[ 'toolset/compiler' ] = 'gcc'
    patch_boost_name = './patch_boost'

bjam[ 'path' ] = os.path.join( regression_root, bjam[ 'name' ] )
bjam[ 'source_dir' ] = os.path.join( boost_root, 'tools', 'build', 'jam_src' )
bjam[ 'build_path' ] = os.path.join( bjam[ 'source_dir' ], bjam[ 'location' ], bjam[ 'name' ] )

process_jam_log[ 'path' ] = os.path.join( regression_root, process_jam_log[ 'name' ] )
process_jam_log[ 'source_dir' ] = os.path.join( boost_root, 'tools', 'regression', 'build' )
process_jam_log[ 'build_path' ] = os.path.join( 
          boost_root, 'bin', 'boost', 'tools', 'regression', 'build'
        , process_jam_log[ 'name' ], process_jam_log[ 'toolset/compiler' ]
        , 'release', process_jam_log[ 'name' ]
        )


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
    return tarball_path
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
        tar.extract( tarinfo, destination )        
        if not tarinfo.isdir():
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


def build_if_needed( tool ):
    if os.path.exists( tool[ 'path' ] ):
        log( 'Found preinstalled "%s"; will use it.' % tool[ 'path' ] )
        return
    
    log( 'Preinstalled "%s" is not found; building one...' % tool[ 'path' ] )
    if os.path.exists( tool[ 'source_dir' ] ):
        log( 'Found %s source directory "%s"' % ( tool[ 'name' ], tool[ 'source_dir' ] ) )
        log( 'Building %s using \"%s\"...' % ( tool[ 'name'], tool[ 'toolset/compiler' ] ) )            
        log( '%s' % tool[ 'build_cmd' ] )
        utils.system( [ 
              'cd %s' % tool[ 'source_dir' ]
            , tool[ 'build_cmd' ]
            ] )
    else:
        raise 'Could not find %s source directory \"%s\"' % ( tool[ 'name' ], tool[ 'source_dir' ] )

    if not os.path.exists( tool[ 'build_path' ] ):
        raise 'Failed to find (\"%s\") after build.' % tool[ 'build_path' ]

    log( '%s succesfully built in "%s" directory' % ( tool[ 'name' ], tool[ 'build_path' ] ) )


def import_utils():
    global utils    
    if utils is None:
        sys.path.append( xsl_reports_dir )
        import utils as utils_module
        utils = utils_module


def tool_path( name_or_spec ):
    if isinstance( name_or_spec, basestring ):
        return os.path.join( regression_results, name_or_spec )

    if os.path.exists( name_or_spec[ 'path' ] ):
        return name_or_spec[ 'path' ]
    else:
        return name_or_spec[ 'build_path' ]


def setup(
          comment
        , args
        , **unused
        ):
    import_utils()
    
    if os.path.exists( patch_boost_name ):
        log( 'Found patch file "%s". Executing it.' % patch_boost_name )
        utils.system( [ patch_boost_name ] )

    build_if_needed( bjam )

    process_jam_log[ 'build_cmd' ] = '%s -sTOOLS=%s'% (
          tool_path( bjam )
        , process_jam_log[ 'toolset/compiler' ]
        )

    build_if_needed( process_jam_log )


def bjam_set_commands( toolsets ):
    result = []
    if not toolsets is None:
        result.append( 'set TOOLS=%s' % string.join( string.split( toolsets, ',' ), ' ' ) )
    result.append( 'set BOOST_ROOT=%s' % boost_root )
    return result


def install( toolsets, **unused ):
    import_utils()
    os.chdir( os.path.join( boost_root ) )

    log( 'Making "%s" directory...' % regression_results )
    utils.makedirs( regression_results )
    
    install_cmd = bjam_set_commands( toolsets )
    install_cmd.append( '"%s" -d2 install >>%s 2>&1' % ( tool_path( bjam ), install_log ) )
    log( 'Installing libraries (%s)...' % install_cmd )
    utils.system( install_cmd )


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
            test_cmd = bjam_set_commands( toolsets )
            test_cmd.append( '"%s" -d2 --dump-tests "-sALL_LOCATE_TARGET=%s" >>%s 2>&1'
                                      % (     tool_path( bjam )
                                            , regression_results
                                            , regression_log
                                            )
                                      )

            log( 'Starting tests (%s)...' % test_cmd )
            utils.system( test_cmd )

        if "process" in args:
            run_process_jam_log()

        os.chdir( cd )
    finally:
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
        mail_subject = 'Boost regression for %s on %s ' % ( tag, string.split(socket.gethostname(), '.')[0] )
        start_time = time.localtime()
        if mail:
            log( 'Sending start notification to "%s"' % mail )
            utils.send_mail( mail, mail_subject + ' started at %s.' % format_time( start_time ) )

        if incremental:
            update_source( user, tag, proxy, [] )
            setup( comment, [] )
        else:
            cleanup( args )
            get_source( user, tag, proxy, [] )
            setup( comment, [] )

        test( toolsets, [] )
        collect_logs( tag, runner, platform, user, comment, incremental, args )
        upload_logs( tag, runner, user )

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
    , 'install'         : install
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
\t--comment       an HTML comment file to be inserted in the reports
\t                ('comment.html' by default)
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
    if command not in [ 'collect-logs', 'upload-logs' ]:
        args.append( '--runner=' )
else:
    command = 'regression'
    args = sys.argv[ 1: ]
    
commands[ command ]( **accept_args( args ) )
