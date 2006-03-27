#!/usr/bin/python

# Copyright (c) MetaCommunications, Inc. 2003-2006
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
import platform
import traceback
import string
import sys

regression_root    = os.path.abspath( os.path.dirname( sys.argv[0] ) )
regression_results = os.path.join( regression_root, 'results' )
regression_log     = os.path.join( regression_results, 'bjam.log' )
install_log        = os.path.join( regression_results, 'bjam_install.log' )

boost_root      = os.path.join( regression_root, 'boost' )
xsl_reports_dir = os.path.join( boost_root, 'tools', 'regression', 'xsl_reports' )
timestamp_path  = os.path.join( regression_root, 'timestamp' )

cvs_command_line         = 'cvs -z9 %(command)s'
cvs_ext_command_line     = 'cvs -d:ext:%(user)s@cvs.sourceforge.net:/cvsroot/boost -z9 %(command)s'
cvs_pserver_command_line = 'cvs -d:pserver:%(user)s@cvs.sourceforge.net:/cvsroot/boost -z9 %(command)s'

bjam = {}
process_jam_log = {}


if sys.platform == 'win32':
    bjam[ 'name' ] = 'bjam.exe'
    bjam[ 'build_cmd' ] = lambda toolset, v2: bjam_build_script_cmd( 'build.bat %s' % toolset )
    bjam[ 'is_supported_toolset' ] = lambda x: x in [ 'borland', 'como', 'gcc', 'gcc-nocygwin' \
                                                    , 'intel-win32', 'metrowerks', 'mingw' \
                                                    , 'msvc', 'vc7' \
                                                    ]
    process_jam_log[ 'name' ] = 'process_jam_log.exe'

    def default_toolset(v2):
        if v2:
            return 'msvc'
        else:
            return 'vc-7_1'

    process_jam_log[ 'default_toolset' ] = default_toolset
    patch_boost_name = 'patch_boost.bat'
else:
    bjam[ 'name' ] = 'bjam'
    bjam[ 'build_cmd' ] = lambda toolset, v2: bjam_build_script_cmd( './build.sh %s' % toolset )
    bjam[ 'is_supported_toolset' ] = lambda x: x in [ 'acc', 'como', 'darwin', 'gcc' \
                                                    , 'intel-linux', 'kcc', 'kylix' \
                                                    , 'mipspro', 'sunpro', 'tru64cxx' \
                                                    , 'vacpp'\
                                                    ]
    process_jam_log[ 'name' ] = 'process_jam_log'
    process_jam_log[ 'default_toolset' ] = lambda x: 'gcc'
    patch_boost_name = 'patch_boost'

bjam[ 'default_toolset' ] = lambda x: ''
bjam[ 'path' ] = os.path.join( regression_root, bjam[ 'name' ] )
bjam[ 'source_dir' ] = os.path.join( boost_root, 'tools', 'jam', 'src' )
bjam[ 'build_path_root' ] = lambda unused: bjam[ 'source_dir' ]

process_jam_log[ 'path' ] = os.path.join( regression_root, process_jam_log[ 'name' ] )
process_jam_log[ 'source_dir' ] = os.path.join( boost_root, 'tools', 'regression', 'build' )


def process_jam_build_root(v2):
    if v2:
        return os.path.join(boost_root, 'dist', 'bin')
    else:
        return os.path.join( 
            boost_root, 'bin', 'boost', 'tools', 'regression', 'build'
            , process_jam_log[ 'name' ])


process_jam_log[ 'build_path_root' ] = process_jam_build_root

process_jam_log[ 'build_cmd' ] = lambda toolset, v2: bjam_command( toolset, v2 )
process_jam_log[ 'is_supported_toolset' ] = lambda x : True

build_monitor_url = 'http://www.meta-comm.com/engineering/resources/build_monitor.zip'
pskill_url = 'http://www.sysinternals.com/files/pskill.zip'

utils = None


def log( message ):
    sys.stdout.flush()
    sys.stderr.flush()
    sys.stderr.write( '# %s\n' % message )
    sys.stderr.flush()


def platform_name():
    # See http://article.gmane.org/gmane.comp.lib.boost.testing/933
    if sys.platform == 'win32':
        return 'Windows'
    elif sys.platform == 'cygwin':
        return 'Windows/Cygwin'

    return platform.system()


def rmtree( path ):
    if os.path.exists( path ):
        if sys.platform == 'win32':
            os.system( 'del /f /s /q "%s" >nul 2>&1' % path )
            shutil.rmtree( unicode( path ) )
        else:
            os.system( 'rm -f -r "%s"' % path )


def retry( f, args, max_attempts=5, sleep_secs=10 ):
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
        boost_bin_dir = os.path.join( boost_root, 'bin' )
        log( 'Cleaning up "%s" directory ...' % boost_bin_dir )
        rmtree( boost_bin_dir )

        boost_binv2_dir = os.path.join( boost_root, 'bin.v2' )
        log( 'Cleaning up "%s" directory ...' % boost_binv2_dir )
        rmtree( boost_binv2_dir )

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


def tarball_name_for_tag( tag, timestamp = False ):
    if not timestamp: return 'boost-%s.tar.bz2' % tag
    else:             return 'boost-%s.timestamp' % tag


def download_boost_tarball( destination, tag, proxy, timestamp_only = False ):
    tarball_name = tarball_name_for_tag( tag, timestamp_only )
    tarball_path = os.path.join( destination, tarball_name )
    tarball_url = 'http://www.meta-comm.com/engineering/boost/snapshot/%s' % tarball_name

    log( 'Downloading "%s" to "%s"...'  % ( tarball_url, os.path.dirname( tarball_path ) ) )
    if os.path.exists( tarball_path ):
        os.unlink( tarball_path )

    http_get(
          tarball_url
        , tarball_path
        , proxy
        )

    return tarball_path


def find_boost_dirs( path ):
    return [ x for x in glob.glob( os.path.join( path, 'boost[-_]*' ) )
                        if os.path.isdir( x ) ]


def unpack_tarball( tarball_path, destination  ):
    log( 'Looking for old unpacked archives...' )
    old_boost_dirs = find_boost_dirs( destination )
    
    for old_boost_dir in old_boost_dirs:
        if old_boost_dir != tarball_path:
            log( 'Deleting old directory %s.' % old_boost_dir ) 
            rmtree( old_boost_dir )

    log( 'Unpacking boost tarball ("%s")...' % tarball_path )

    tarball_name = os.path.basename( tarball_path )
    extension = tarball_name[ tarball_name.find( '.' ) : ]

    if extension in ( ".tar.gz", ".tar.bz2" ):
        mode = os.path.splitext( extension )[1][1:]
        tar = tarfile.open( tarball_path, 'r:%s' % mode )
        for tarinfo in tar:
            tar.extract( tarinfo, destination )
            if sys.platform == 'win32' and not tarinfo.isdir():
                # workaround what appears to be a Win32-specific bug in 'tarfile'
                # (modification times for extracted files are not set properly)
                f = os.path.join( destination, tarinfo.name )
                os.chmod( f, stat.S_IWRITE )
                os.utime( f, ( tarinfo.mtime, tarinfo.mtime ) )
        tar.close()
    elif extension in ( ".zip" ):
        import zipfile
        
        z = zipfile.ZipFile( tarball_path, 'r', zipfile.ZIP_DEFLATED ) 
        for f in z.infolist():
            destination_file_path = os.path.join( destination, f.filename )
            if destination_file_path[-1] == "/": # directory 
                if not os.path.exists( destination_file_path  ):
                    os.makedirs( destination_file_path  )
            else: # file
                result = open( destination_file_path, 'wb' )
                result.write( z.read( f.filename ) )
                result.close()
        z.close()
    else:
        raise 'Do not know how to unpack archives with extension \"%s\"' % extension

    boost_dir = find_boost_dirs( destination )[0]
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
    cvs_command( user, command )


def cvs_update( user, tag, args ):
    if tag != 'CVS-HEAD':
        command = 'update -dPA -r %s' % tag
    else:
        command = 'update -dPA'
    
    os.chdir( os.path.join( regression_root, 'boost' ) )
    cvs_command( user, command )


def format_time( t ):
    return time.strftime( 
          '%a, %d %b %Y %H:%M:%S +0000'
        , t
        )


def refresh_timestamp():
    if os.path.exists( timestamp_path ):
       os. unlink( timestamp_path )

    open( timestamp_path, 'w' ).close()


def timestamp():
    return time.strftime(
          '%Y-%m-%dT%H:%M:%SZ'
        , time.gmtime( os.stat( timestamp_path ).st_mtime )
        )


def get_tarball( tag, proxy, args, **unused ):
    if args == []: args = [ 'download', 'unpack' ]

    tarball_path = None

    if 'download' in args:
        tarball_path = download_boost_tarball( regression_root, tag, proxy )

    if 'unpack' in args:
        if not tarball_path:
            tarball_path = os.path.join( regression_root, tarball_name_for_tag( tag ) )
        unpack_tarball( tarball_path, regression_root )


def get_source( user, tag, proxy, args, **unused ):
    refresh_timestamp()
    log( 'Getting sources (%s)...' % timestamp() )

    if user is not None:
        retry( 
              cvs_checkout
            , ( user, tag, args )
            )
    else:
        retry( 
              get_tarball
            , ( tag, proxy, args )
            )


def update_source( user, tag, proxy, args, **unused ):
    if user is not None or os.path.exists( os.path.join( boost_root, 'CVS' ) ):
        open( timestamp_path, 'w' ).close()
        log( 'Updating sources from CVS (%s)...' % timestamp() )
        retry( 
              cvs_update
            , ( user, tag, args )
            )
    else:
        get_source( user, tag, proxy, args )


def tool_path( name_or_spec, v2=None ):
    if isinstance( name_or_spec, basestring ):
        return os.path.join( regression_root, name_or_spec )

    if os.path.exists( name_or_spec[ 'path' ] ):
        return name_or_spec[ 'path' ]

    if name_or_spec.has_key( 'build_path' ):
        return name_or_spec[ 'build_path' ]

    build_path_root = name_or_spec[ 'build_path_root' ]( v2 )
    log( 'Searching for "%s" in "%s"...' % ( name_or_spec[ 'name' ], build_path_root ) )
    for root, dirs, files in os.walk( build_path_root ):
        if name_or_spec[ 'name' ] in files:
            return os.path.join( root, name_or_spec[ 'name' ] )
    
    raise Exception( 'Cannot find "%s" in any of the following locations:\n%s' % (
          name_or_spec[ 'name' ]
        , '\n'.join( [ name_or_spec[ 'path' ], build_path_root ] )
        ) )


def build_if_needed( tool, toolset, toolsets, v2 ):
    if os.path.exists( tool[ 'path' ] ):
        log( 'Found preinstalled "%s"; will use it.' % tool[ 'path' ] )
        return

    log( 'Preinstalled "%s" is not found; building one...' % tool[ 'path' ] )

    if toolset is None:
        if toolsets is not None:
            toolset = string.split( toolsets, ',' )[0]
            if not tool[ 'is_supported_toolset' ]( toolset ):
                log( 'Warning: Specified toolset (%s) cannot be used to bootstrap "%s".'\
                     % ( toolset, tool[ 'name' ] ) )

                toolset = tool[ 'default_toolset' ](v2)
                log( '         Using default toolset for the platform (%s).' % toolset )
        else:
            toolset = tool[ 'default_toolset' ](v2)
            log( 'Warning: No bootstrap toolset for "%s" was specified.' % tool[ 'name' ] )
            log( '         Using default toolset for the platform (%s).' % toolset )

    if os.path.exists( tool[ 'source_dir' ] ):
        log( 'Found "%s" source directory "%s"' % ( tool[ 'name' ], tool[ 'source_dir' ] ) )
        build_cmd = tool[ 'build_cmd' ]( toolset, v2 )
        log( 'Building "%s" (%s)...' % ( tool[ 'name'], build_cmd ) )
        utils.system( [ 
              'cd "%s"' % tool[ 'source_dir' ]
            , build_cmd
            ] )
    else:
        raise 'Could not find "%s" source directory "%s"' % ( tool[ 'name' ], tool[ 'source_dir' ] )

    if not tool.has_key( 'build_path' ):
        tool[ 'build_path' ] = tool_path( tool, v2 )

    if not os.path.exists( tool[ 'build_path' ] ):
        raise 'Failed to find "%s" after build.' % tool[ 'build_path' ]

    log( '%s succesfully built in "%s" location' % ( tool[ 'name' ], tool[ 'build_path' ] ) )


def import_utils():
    global utils
    if utils is None:
        sys.path.append( xsl_reports_dir )
        import utils as utils_module
        utils = utils_module


def download_if_needed( tool_name, tool_url, proxy ):
    path = tool_path( tool_name )
    if not os.path.exists( path ):
        log( 'Preinstalled "%s" is not found.' % path )
        log( '  Downloading from %s...' % tool_url )
        
        zip_path = '%s.zip' % os.path.splitext( path )[0]
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
        , v2
        , args
        , **unused
        ):
    import_utils()

    patch_boost_path = os.path.join( regression_root, patch_boost_name )
    if os.path.exists( patch_boost_path ):
        log( 'Found patch file "%s". Executing it.' % patch_boost_path )
        os.chdir( regression_root )
        utils.system( [ patch_boost_path ] )

    build_if_needed( bjam, bjam_toolset, toolsets, v2 )
    build_if_needed( process_jam_log, pjl_toolset, toolsets, v2 )
    
    if monitored:
        if sys.platform == 'win32':
            download_if_needed( 'build_monitor.exe', build_monitor_url, proxy )
            download_if_needed( 'pskill.exe', pskill_url, proxy )
        else:
            log( 'Warning: Test monitoring is not supported on this platform (yet).' )
            log( '         Please consider contributing this piece!' )


def bjam_build_script_cmd( cmd ):
    env_setup_key = 'BJAM_ENVIRONMENT_SETUP'
    if os.environ.has_key( env_setup_key ):
        return '%s & %s' % ( os.environ[env_setup_key], cmd )

    return cmd


def bjam_command( toolsets, v2 ):
    build_path = regression_root
    if build_path[-1] == '\\': build_path += '\\'
    v2_option = ""
    if v2:
        v2_option = "--v2"
    result = '"%s" %s "-sBOOST_BUILD_PATH=%s" "-sBOOST_ROOT=%s"'\
        % (
            tool_path( bjam, v2 )
          , v2_option
          , build_path
          , boost_root
          )
    
    if not toolsets is None:
        if v2:
            result += ' ' + string.join(string.split( toolsets, ',' ), ' ' )
        else:
            result += ' "-sTOOLS=%s"' % string.join( string.split( toolsets, ',' ), ' ' )

    return result


def install( toolsets, v2, **unused ):
    import_utils()
    os.chdir( os.path.join( boost_root ) )

    log( 'Making "%s" directory...' % regression_results )
    utils.makedirs( regression_results )
    
    install_cmd = '%s -d2 install >>%s 2>&1' % ( bjam_command( toolsets, v2 ), install_log )
    log( 'Installing libraries (%s)...' % install_cmd )
    utils.system( [ install_cmd ] )


def start_build_monitor( timeout ):
    if sys.platform == 'win32':
        build_monitor_path = tool_path( 'build_monitor.exe' )
        if os.path.exists( build_monitor_path ):
            utils.system( [ 'start /belownormal "" "%s" bjam.exe %d' % ( build_monitor_path, timeout*60 ) ] )
        else:
            log( 'Warning: Build monitor is not found at "%s"' % build_monitor_path )


def stop_build_monitor():
    if sys.platform == 'win32':
        build_monitor_path = tool_path( 'build_monitor.exe' )
        if os.path.exists( build_monitor_path ):
            utils.system( [ '"%s" build_monitor' %  tool_path( 'pskill.exe' ) ] )


def run_process_jam_log(v2):
    log( 'Getting test case results out of "%s"...' % regression_log )

    if v2:
        v2 = "--v2"
    else:
        v2 = ""

    utils.checked_system( [ 
        '"%s" %s "%s" <"%s"' % (
              tool_path( process_jam_log, v2 )
            , v2  
            , regression_results
            , regression_log
            )
        ] )


def test( 
          toolsets
        , bjam_options
        , monitored
        , timeout
        , v2  
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

        build_dir_option = "-sALL_LOCATE_TARGET"
        if v2:
            build_dir_option = "--build-dir"

        if "test" in args:
            test_cmd = '%s -d2 --dump-tests %s "%s=%s" >>"%s" 2>&1' % (
                  bjam_command( toolsets, v2 )
                , bjam_options
                , build_dir_option  
                , regression_results
                , regression_log
                )

            log( 'Starting tests (%s)...' % test_cmd )
            utils.system( [ test_cmd ] )

        if "process" in args:
            run_process_jam_log(v2)

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
    
    if comment is None:
        comment = 'comment.html'

    comment_path = os.path.join( regression_root, comment )
    if not os.path.exists( comment_path ):
        log( 'Comment file "%s" not found; creating default comment.' % comment_path )
        f = open( comment_path, 'w' )
        f.write( '<p>Tests are run on %s platform.</p>' % platform_name() )
        f.close()
    
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
        , ftp_proxy
        , debug_level
        , **unused
        ):
    import_utils()
    from runner import upload_logs
    retry(
          upload_logs
        , ( regression_results, runner, tag, user, ftp_proxy, debug_level )
        )


def update_itself( tag, **unused ):
    source = os.path.join( xsl_reports_dir, 'runner', os.path.basename( sys.argv[0] ) )
    self = os.path.join( regression_root, os.path.basename( sys.argv[0] ) )

    log( 'Updating %s from %s...' % ( self, source )  )
    log( '    Checking modification dates...' )
    if os.stat( self ).st_mtime > os.stat( source ).st_mtime:
        log( 'Warning: The current version of script appears to be newer than the source.' )
        log( '         Update skipped.' )
    else:
        log( '    Saving a backup copy of the current script...' )
        os.chmod( self, stat.S_IWRITE ) # Win32 workaround
        shutil.move( self, '%s~' % self )
        log( '    Replacing %s with a newer version...' % self )
        shutil.copy2( source, self )


def send_mail( smtp_login, mail, subject, msg = '', debug_level = 0 ):
    import smtplib
    if not smtp_login:
        server_name = 'mail.%s' % mail.split( '@' )[-1]
        user_name = None
        password = None
    else:
        server_name = smtp_login.split( '@' )[-1]
        ( user_name, password ) = string.split( smtp_login.split( '@' )[0], ':' )
        
    log( '    Sending mail through "%s"...' % server_name )
    smtp_server = smtplib.SMTP( server_name )
    smtp_server.set_debuglevel( debug_level )
    if user_name:
        smtp_server.login( user_name, password )
    
    smtp_server.sendmail( 
          mail
        , [ mail ]
        , 'Subject: %s\nTo: %s\n\n%s' % ( subject, mail, msg )
        )


def regression( 
          tag
        , local
        , runner
        , platform
        , user
        , comment
        , toolsets
        , bjam_options
        , bjam_toolset
        , pjl_toolset
        , incremental
        , force_update
        , monitored
        , timeout
        , mail = None
        , smtp_login = None
        , proxy = None
        , ftp_proxy = None
        , debug_level = 0
        , v2 = 0
        , args = []
        ):

    try:
        mail_subject = 'Boost regression for %s on %s' % ( tag, string.split(socket.gethostname(), '.')[0] )
        start_time = time.localtime()
        if mail:
            log( 'Sending start notification to "%s"' % mail )
            send_mail(
                  smtp_login
                , mail
                , '%s started at %s.' % ( mail_subject, format_time( start_time ) )
                , debug_level = debug_level
                )

        if local is not None:
            log( 'Using local file "%s"' % local )
            
            b = os.path.basename( local )
            tag = b[ 0: b.find( '.' ) ]
            log( 'Tag: "%s"' % tag  )
            
            unpack_tarball( local, regression_root )
        else:
            if incremental or force_update:
                if not incremental: cleanup( [ 'bin' ] )
                update_source( user, tag, proxy, [] )
            else:
                cleanup( [] )
                get_source( user, tag, proxy, [] )

        setup( comment, toolsets, bjam_toolset, pjl_toolset, monitored, proxy,
               v2, [] )
        test( toolsets, bjam_options, monitored, timeout, v2, [] )
        collect_logs( tag, runner, platform, user, comment, incremental, [] )
        upload_logs( tag, runner, user, ftp_proxy, debug_level )
        update_itself( tag )
        
        if mail:
            log( 'Sending report to "%s"' % mail )
            end_time = time.localtime()
            send_mail( 
                  smtp_login
                , mail
                , '%s completed successfully at %s.' % ( mail_subject, format_time( end_time ) )
                , debug_level = debug_level
                )
    except:
        if mail:
            log( 'Sending report to "%s"' % mail )
            traceback_ = '\n'.join( apply( traceback.format_exception, sys.exc_info() ) )
            end_time = time.localtime()
            send_mail(
                  smtp_login
                , mail
                , '%s failed at %s.' % ( mail_subject, format_time( end_time ) )
                , traceback_
                , debug_level
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
        , 'local='
        , 'runner='
        , 'platform='
        , 'user='
        , 'comment='
        , 'toolsets='
        , 'bjam-options='
        , 'bjam-toolset='
        , 'pjl-toolset='
        , 'timeout='
        , 'mail='
        , 'smtp-login='
        , 'proxy='
        , 'ftp-proxy='
        , 'debug-level='
        , 'incremental'
        , 'force-update'
        , 'monitored'
        , 'help'
        , 'v2'  
        ]
    
    options = {
          '--tag'           : 'CVS-HEAD'
        , '--local'         : None
        , '--platform'      : platform_name()
        , '--user'          : None
        , '--comment'       : None
        , '--toolsets'      : None
        , '--bjam-options'  : ''
        , '--bjam-toolset'  : None
        , '--pjl-toolset'   : None
        , '--timeout'       : 5
        , '--mail'          : None
        , '--smtp-login'    : None
        , '--proxy'         : None
        , '--debug-level'   : 0
        , '--ftp-proxy'     : None
        }
    
    ( option_pairs, other_args ) = getopt.getopt( args, '', args_spec )
    map( lambda x: options.__setitem__( x[0], x[1] ), option_pairs )

    if not options.has_key( '--runner' ) or options.has_key( '--help' ):
        usage()
        sys.exit( 1 )

    return {
          'tag'             : options[ '--tag' ]
        , 'local'           : options[ '--local' ]
        , 'runner'          : options[ '--runner' ]
        , 'platform'        : options[ '--platform']
        , 'user'            : options[ '--user' ]
        , 'comment'         : options[ '--comment' ]
        , 'toolsets'        : options[ '--toolsets' ]
        , 'bjam_options'    : options[ '--bjam-options' ]
        , 'bjam_toolset'    : options[ '--bjam-toolset' ]
        , 'pjl_toolset'     : options[ '--pjl-toolset' ]
        , 'incremental'     : options.has_key( '--incremental' )
        , 'force_update'    : options.has_key( '--force-update' )
        , 'monitored'       : options.has_key( '--monitored' )
        , 'timeout'         : options[ '--timeout' ]
        , 'mail'            : options[ '--mail' ]
        , 'smtp_login'      : options[ '--smtp-login' ]
        , 'proxy'           : options[ '--proxy' ]
        , 'ftp_proxy'       : options[ '--ftp-proxy' ]
        , 'debug_level'     : int(options[ '--debug-level' ])
        , 'v2'              : options.has_key( '--v2' )  
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
\t--local         the name of the boost tarball
\t--comment       an HTML comment file to be inserted in the reports
\t                ('comment.html' by default)
\t--incremental   do incremental run (do not remove previous binaries)
\t--force-update  do a CVS update (if applicable) instead of a clean
\t                checkout, even when performing a full run
\t--monitored     do a monitored run
\t--timeout       specifies the timeout, in minutes, for a single test
\t                run/compilation (enforced only in monitored runs, 5 by 
\t                default)
\t--user          SourceForge user name for a shell/CVS account (optional)
\t--toolsets      comma-separated list of toolsets to test with (optional)
\t--bjam-options  options to pass to the regression test (optional)
\t--bjam-toolset  bootstrap toolset for 'bjam' executable (optional)
\t--pjl-toolset   bootstrap toolset for 'process_jam_log' executable
\t                (optional)
\t--mail          email address to send run notification to (optional)
\t--smtp-login    STMP server address/login information, in the following
\t                form: <user>:<password>@<host>[:<port>] (optional).
\t--proxy         HTTP proxy server address and port (e.g. 
\t                'http://www.someproxy.com:3128', optional)
\t--ftp-proxy     FTP proxy server (e.g. 'ftpproxy', optional)
\t--debug-level   debugging level; controls the amount of debugging 
\t                output printed; 0 by default (no debug output)
\t--v2            Use Boost.Build V2
''' % '\n\t'.join( commands.keys() )

    print 'Example:\n\t%s --runner=Metacomm\n' % os.path.basename( sys.argv[0] )
    print 'For more documentation, see http://tinyurl.com/4f2zp\n'


if __name__ == '__main__':
    if len(sys.argv) > 1 and sys.argv[1] in commands:
        command = sys.argv[1]
        args = sys.argv[ 2: ]
        if command not in [ 'collect-logs', 'upload-logs' ]:
            args.insert( 0, '--runner=' )
    else:
        command = 'regression'
        args = sys.argv[ 1: ]

    commands[ command ]( **accept_args( args ) )
