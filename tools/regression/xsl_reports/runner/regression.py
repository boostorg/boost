
# Copyright (c) MetaCommunications, Inc. 2003-2004
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import time
import glob
import shutil
import os.path
import os
import traceback
import string
import sys

boost_root = os.path.abspath( os.path.dirname( sys.argv[0] ) )
while os.path.basename( boost_root ) != 'boost': 
    boost_root = os.path.dirname( boost_root )

regression_root = os.path.dirname( boost_root )
regression_results = os.path.join( regression_root, 'results' )
regression_log = os.path.join( regression_results, 'bjam.log' )

timestamp = None

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
    bjam_make_cmd = 'build.sh' % bjam_build_compiler
    bjam_location = ''
    process_jam_log_name = "process_jam_log"
    process_jam_log_toolset = 'gcc'

bjam_path = os.path.join( regression_root, bjam_name )
process_jam_log_path = os.path.join( regression_root, process_jam_log_name )


def tool_path( name ):
    return os.path.join( regression_results, name )


def check_environment():
    try:
        utils.check_existance( 'gzip' )
        utils.check_existance( 'tar' )
    except Exception, msg:
        raise '\nPrerequisites check failed: %s' % msg


def cleanup( args ):
    utils.log( 'Cleaning up "%s" directory ...' % boost_root )
    utils.rmtree( boost_root )
    
    utils.log( 'Cleaning up "%s" directory ...' % regression_results )
    utils.rmtree( regression_results )


def download_boost_tarball( destination, tag ):
    site = 'boost-consulting.com'
    tarball_name = 'boost.tar.bz2'
    tarball_path = os.path.join( destination, tarball_name )

    utils.log( "Downloading '%s' for tag %s from %s "  % ( tarball_path, tag, site ) )
    return tarball_path
    if os.path.exists( tarball_path ):
        os.unlink( tarball_path )

    utils.http_get(
          site
        , '/%s' % tarball_name # ignore tag for now
        , tarball_path
        )
        
    return tarball_path


def unpack_tarball( tarball_path, destination ):
    utils.log( 'Looking for old unpacked archives...' )
    old_boost_dirs =  glob.glob( os.path.join( destination, 'boost-*' ) )

    for old_boost_dir in old_boost_dirs:
        utils.log( 'Deleting old directory %s.' % old_boost_dir ) 
        utils.rmtree( old_boost_dir )

    utils.log( 'Unpacking boost tarball ("%s")' % tarball_path )
    utils.untar( tarball_path )
    boost_dir = glob.glob( os.path.join( destination, 'boost-*' ) )[0]
    utils.log( '    Unpacked into directory "%s"' % boost_dir )
    
    if os.path.exists( boost_root ):
        utils.log( 'Deleting "%s" directory...' % boost_root )
        shutil.rmtree( boost_root )

    utils.log( 'Renaming "%s" into "%s"' % ( boost_dir, boost_root ) )
    os.rename( boost_dir, boost_root )


def get_source( user, tag, args ):
    __log__ = 1; utils.log( "Getting sources ..." )

    global timestamp
    timestamp = time.gmtime()
    if user is not None:
        boost_cvs_checkout( user, tag )
    else:
        tarball_path = download_boost_tarball( regression_root, tag )
        unpack_tarball( tarball_path, regression_root )


def update_source( args ):
    __log__ = 1; log( "Getting sources ..." ) 
    get_source_method = { "cvs": boost_cvs_update
                          , "directory": get_source_directory
                          }
    log( "    Source type: %s" % config.source.source_type )
    get_source_method[ config.source.source_type ]( args )



def build_bjam_if_needed():    
    global bjam_path
    if os.path.exists( bjam_path ):
        utils.log( 'Found preinstalled "%s"; will use it.' % bjam_path )
        return
    
    utils.log( 'Preinstalled "%s" is not found; building one...' % bjam_path )

    utils.log( 'Locating bjam source directory...' )
    bjam_source_dir = os.path.join( boost_root, 'tools', 'build', 'jam_src' )
    
    if os.path.exists( bjam_source_dir ):
        utils.log( 'Found bjam source directory "%s"' % bjam_source_dir )
        utils.log( 'Building bjam using \"%s\"...' % bjam_build_compiler )
            
        utils.log( "Building bjam (%s)" % bjam_make_cmd )
        utils.system( [ 
              'cd %s' % bjam_source_dir
            , bjam_make_cmd 
            ] )
    else:
        raise 'Could not find bjam source directory \"%s\"' % bjam_source_dir

    bjam_path = os.path.join( bjam_source_dir, bjam_location, bjam_name )
    if not os.path.exists( bjam_path ):
        raise 'Failed to find bjam (\"%s\") after build.' % bjam_path

    utils.log( 'Bjam succesfully built in "%s" directory' % bjam_path )


def build_process_jam_log_if_needed():
    global process_jam_log_path
    if os.path.exists( process_jam_log_path ):
        utils.log( 'Found preinstalled "%s"; will use it.' % process_jam_log_path )
        return
    
    utils.log( 'Preinstalled "%s" is not found; building one.' % process_jam_log_path )
    
    process_jam_log_source_dir = os.path.join( boost_root, 'tools', 'regression', 'build' )

    utils.log( 'Locating proces_jam_log source directory...' )
    if os.path.exists( process_jam_log_source_dir ):
        utils.log( 'Found proces_jam_log source directory "%s"' % process_jam_log_source_dir )

        utils.log( 'Building process_jam_log using toolset "%s"' % process_jam_log_toolset )
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
    
    utils.log( 'Process_jam_log succesfully built in "%s" directory' % process_jam_log_path )


def setup( args ):
    if not 'no-bjam' in args:
        build_bjam_if_needed()
    
    if not 'no-process_jam_log' in args:
        build_process_jam_log_if_needed()


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
    utils.log( 'Getting test case results out of "%s"...' % regression_log )
    utils.checked_system( [ 
        "%s %s <%s" % (  
              process_jam_log_path
            , regression_results
            , regression_log
            )
        ] )
    
    os.rename( regression_log, '%s.processed' % regression_log )


def test( 
          toolsets
        , args
        ):
    if args == []:
        args = [ "test", "process" ]

    try:
        start_build_monitor()
        cd = os.getcwd()
        os.chdir( os.path.join( boost_root, 'status' ) )

        utils.log( 'Making "%s" directory...' % regression_results )
        utils.makedirs( regression_results )

        results_libs = os.path.join( regression_results, 'libs' )
        results_status = os.path.join( regression_results, 'status' )
        
        if "clean" in args:
            utils.rmtree( results_libs )
            utils.rmtree( results_status )
                
        if "test" in args:
            test_cmd = []
            if not toolsets is None:
                test_cmd.append( 'set TOOLS=%s' % string.join( toolsets, ' ' ) )
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


def upload( 
          tag
        , runner_id
        , platform
        , user
        , comment_file
        , args
        ):
    global timestamp
    import runner

    runner.collect_and_upload_logs( 
          regression_results
        , runner_id
        , tag
        , platform
        , comment_file
        , time.strftime("%a, %d %b %Y %H:%M:%S +0000", timestamp )
        , user
        )


def regression( 
          tag
        , runner
        , platform
        , user
        , comment_file
        , toolsets
        , incremental
        , mail = None
        , args = []
        ):

    check_environment()

    try:
        mail_subject = "Boost regression for %s on %s \n" % ( tag, os.environ[ "COMPUTERNAME" ] )
        if incremental:
            update_source( user )
            setup( [ "bjam", "build" ] )
        else:
            cleanup( args )
            get_source( user, tag, args )
            setup( args )

        test( toolsets, args )
        upload( tag, runner, platform, user, comment_file, args )

        if mail:
            utils.log( 'Sending report to "%s"' % mail )
            utils.send_mail( mail, mail_subject + ' completed successfully.' )
    except:
        if mail:
            utils.log( 'Sending report to "%s"' % mail )
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
        , 'incremental'
        , 'help'
        ]
    
    options = {
          '--platform' :    sys.platform
        , '--user' :        None
        , '--comment' :     'comment.html'
        , '--toolsets' :    None
        , '--mail' :        None
        }
    
    other_args = utils.accept_args( args_spec, args, options, usage )

    return ( 
          options[ '--tag' ]
        , options[ '--runner' ]
        , options[ '--platform']
        , options[ '--user' ]
        , options[ '--comment' ]
        , options[ '--toolsets' ]
        , options.has_key( '--incremental' )
        , options[ '--mail' ]
        , other_args
        )


commands = {
      'cleanup'         : cleanup
    , 'get-source'      : get_source
    , 'update-source'   : update_source
    , 'setup'           : setup
    , 'test'            : test
    , 'upload'          : upload
    , 'regression'      : regression
    }

def usage():
    print 'Usage: %s command [options|@response-file]' % os.path.basename( sys.argv[0] )
    print    '''
Commands:
\t%s

Options:

\t--tag                 the tag for the results (e.g. 'CVS-HEAD')
\t--runner              runner ID (e.g. 'Metacomm')
\t--comment             an html comment file (will be inserted in the reports, 
\t                      'comment.html' by default)
\t--incremental         do incremental run (do not remove previous binaries)
\t--user                SourceForge user name for a shell/CVS account (optional)
\t--toolsets            comma-separated list of toolsets to test with (optional)
\t--mail                email address to send run notification to (optional)
''' % '\n\t'.join( commands.keys() )

    print 'Example:\n\t%s regression --tag=CVS-HEAD --runner=Metacomm' \
        % os.path.basename( sys.argv[0] )


def main():
    if len(sys.argv) < 2:
        usage()
        sys.exit( 1 )

    command = sys.argv[1];
    commands[ command ]( *accept_args( sys.argv[ 2: ] ) );


if __name__ != '__main__':  import utils
else:
    # in absense of relative import...
    xsl_path = os.path.abspath( os.path.dirname( sys.argv[ 0 ] ) )
    while os.path.basename( xsl_path ) != 'xsl_reports': xsl_path = os.path.dirname( xsl_path )
    sys.path.append( xsl_path )

    import utils
    main()
