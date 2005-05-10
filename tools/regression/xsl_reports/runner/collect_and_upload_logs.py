
# Copyright (c) MetaCommunications, Inc. 2003-2004
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import xml.sax.saxutils
import zipfile
import ftplib
import time
import stat

import os.path
import string
import sys


def process_xml_file( input_file, output_file ):
    utils.log( 'Processing test log "%s"' % input_file )
    
    f = open( input_file, 'r' )
    xml = f.readlines()
    f.close()
    
    for i in range( 0, len(xml)):
        xml[i] = string.translate( xml[i], utils.char_translation_table )

    output_file.writelines( xml )


def process_test_log_files( output_file, dir, names ):
    for file in names:
        if os.path.basename( file ) == 'test_log.xml':
            process_xml_file( os.path.join( dir, file ), output_file )


def collect_test_logs( input_dirs, test_results_writer ):
    __log__ = 1
    utils.log( 'Collecting test logs ...' )
    for input_dir in input_dirs:
        utils.log( 'Walking directory "%s" ...' % input_dir )
        os.path.walk( input_dir, process_test_log_files, test_results_writer )


def upload_to_ftp( tag, results_file, ftp_proxy, debug_level ):
    ftp_site = 'fx.meta-comm.com'
    site_path = '/boost-regression'
    utils.log( 'Uploading log archive "%s" to ftp://%s%s/%s' % ( results_file, ftp_site, site_path, tag ) )
    
    if not ftp_proxy:
        ftp = ftplib.FTP( ftp_site )
        ftp.set_debuglevel( debug_level )
        ftp.login()
    else:
        utils.log( '    Connecting through FTP proxy server "%s"' % ftp_proxy )
        ftp = ftplib.FTP( ftp_proxy )
        ftp.set_debuglevel( debug_level )
        ftp.set_pasv (0) # turn off PASV mode
        ftp.login( 'anonymous@%s' % ftp_site, 'anonymous@' )

    ftp.cwd( site_path )
    try:
        ftp.cwd( tag )
    except ftplib.error_perm:
        ftp.mkd( tag )
        ftp.cwd( tag )

    f = open( results_file, 'rb' )
    ftp.storbinary( 'STOR %s' % os.path.basename( results_file ), f )
    ftp.quit()


def copy_comments( results_xml, comment_file ):
    results_xml.startElement( 'comment', {} )

    if os.path.exists( comment_file ):
        utils.log( 'Reading comments file "%s"...' % comment_file )
        f = open( comment_file, 'r' )
        try:
            results_xml.characters( f.read() )
        finally:
            f.close()    
    else:
        utils.log( 'Warning: comment file "%s" is not found.' % comment_file )
 
    results_xml.endElement( 'comment' )


def collect_logs( 
          results_dir
        , runner_id
        , tag
        , platform
        , comment_file
        , timestamp
        , user
        , source
        , run_type
        , **unused
        ):
    
    results_file = os.path.join( results_dir, '%s.xml' % runner_id )
    results_writer = open( results_file, 'w' )
    utils.log( 'Collecting test logs into "%s"...' % results_file )
    
    if not os.path.exists( timestamp ):
        t = time.gmtime()
        utils.log( 'Warning: timestamp file "%s" does not exist'% timestamp )
        utils.log( 'Using current UTC time (%s)' % t )
    else:
        t = time.gmtime( os.stat( timestamp ).st_mtime )
    
    results_xml = xml.sax.saxutils.XMLGenerator( results_writer )
    results_xml.startDocument()
    results_xml.startElement( 
          'test-run'
        , { 
              'tag':        tag
            , 'platform':   platform
            , 'runner':     runner_id
            , 'timestamp':  time.strftime( '%Y-%m-%dT%H:%M:%SZ', t )
            , 'source':     source
            , 'run-type':   run_type
            }
        )
    
    copy_comments( results_xml, comment_file )
    collect_test_logs( [ results_dir ], results_writer )

    results_xml.endElement( "test-run" )
    results_xml.endDocument()
    results_writer.close()
    utils.log( 'Done writing "%s".' % results_file )

    utils.log( 'Compressing "%s"...' % results_file )
    archive_path = os.path.join( results_dir,'%s.zip' % runner_id )

    try:
        z = zipfile.ZipFile( archive_path, 'w', zipfile.ZIP_DEFLATED )
        z.write( results_file, os.path.basename( results_file ) )
        z.close()
        utils.log( 'Done writing "%s".'% archive_path )
    except Exception, msg:
        utils.log( 'Warning: Compressing falied (%s)' % msg )
        utils.log( '         Trying to compress using a platform-specific tool...' )
        try: import zip_cmd
        except ImportError:
            script_dir = os.path.dirname( os.path.abspath( sys.argv[0] ) )
            utils.log( 'Could not find \'zip_cmd\' module in the script directory (%s).' % script_dir )
            raise Exception( 'Compressing failed!' )
        else:
            if os.path.exists( archive_path ):
                os.unlink( archive_path )
                utils.log( 'Removing stale "%s".' % archive_path )
                
            zip_cmd.main( results_file, archive_path )
            utils.log( 'Done compressing "%s".' % archive_path )


def upload_logs(
          results_dir
        , runner_id
        , tag
        , user
        , ftp_proxy
        , debug_level
        , **unused
        ):

    logs_archive = os.path.join( results_dir, '%s.zip' % runner_id )
    upload_to_ftp( tag, logs_archive, ftp_proxy, debug_level )


def collect_and_upload_logs( 
          results_dir
        , runner_id
        , tag
        , platform
        , comment_file
        , timestamp
        , user
        , source
        , run_type
        , ftp_proxy = None
        , debug_level = 0
        , **unused
        ):
    
    collect_logs( 
          results_dir
        , runner_id
        , tag
        , platform
        , comment_file
        , timestamp
        , user
        , source
        , run_type
        )
    
    upload_logs( results_dir, runner_id, tag, user, ftp_proxy, debug_level )


def accept_args( args ):
    args_spec = [ 
          'locate-root='
        , 'runner='
        , 'tag='
        , 'platform='
        , 'comment='
        , 'timestamp='
        , 'source='
        , 'run-type='
        , 'user='
        , 'ftp-proxy='
        , 'debug-level='
        , 'help'
        ]
    
    options = {
          '--tag'           : 'CVS-HEAD'
        , '--platform'      : sys.platform
        , '--comment'       : 'comment.html'
        , '--timestamp'     : 'timestamp'
        , '--user'          : None
        , '--source'        : 'CVS'
        , '--run-type'      : 'full'
        , '--debug-level'   : 0
        , '--ftp-proxy'     : None
        }
    
    utils.accept_args( args_spec, args, options, usage )
        
    return {
          'results_dir'     : options[ '--locate-root' ]
        , 'runner_id'       : options[ '--runner' ]
        , 'tag'             : options[ '--tag' ]
        , 'platform'        : options[ '--platform']
        , 'comment_file'    : options[ '--comment' ]
        , 'timestamp'       : options[ '--timestamp' ]
        , 'user'            : options[ '--user' ]
        , 'source'          : options[ '--source' ]
        , 'run_type'        : options[ '--run-type' ]
        , 'ftp_proxy'       : options[ '--ftp-proxy' ]
        , 'debug_level'     : int(options[ '--debug-level' ])
        }


commands = {
      'collect-and-upload'  : collect_and_upload_logs
    , 'collect-logs'        : collect_logs
    , 'upload-logs'         : upload_logs
    }

def usage():
    print 'Usage: %s [command] [options]' % os.path.basename( sys.argv[0] )
    print    '''
Commands:
\t%s

Options:
\t--locate-root   directory to to scan for "test_log.xml" files
\t--runner        runner ID (e.g. "Metacomm")
\t--timestamp     path to a file which modification time will be used 
\t                as a timestamp of the run ("timestamp" by default)
\t--comment       an HTML comment file to be inserted in the reports
\t                ("comment.html" by default)
\t--tag           the tag for the results ("CVS-HEAD" by default)
\t--user          SourceForge user name for a shell account (optional)
\t--source        where Boost sources came from (e.g. "CVS", "tarball",
\t                "anonymous CVS"; "CVS" by default)
\t--run-type      "incremental" or "full" ("full" by default)
\t--ftp-proxy     FTP proxy server (e.g. 'ftpproxy', optional)
\t--debug-level   debugging level; controls the amount of debugging 
\t                output printed; 0 by default (no debug output)
''' % '\n\t'.join( commands.keys() )

    
def main():
    if len(sys.argv) > 1 and sys.argv[1] in commands:
        command = sys.argv[1]
        args = sys.argv[ 2: ]
    else:
        command = 'collect-and-upload'
        args = sys.argv[ 1: ]
    
    commands[ command ]( **accept_args( args ) )


if __name__ != '__main__':  import utils
else:
    # in absense of relative import...
    xsl_path = os.path.abspath( os.path.dirname( sys.argv[ 0 ] ) )
    while os.path.basename( xsl_path ) != 'xsl_reports': xsl_path = os.path.dirname( xsl_path )
    sys.path.append( xsl_path )

    import utils
    main()
