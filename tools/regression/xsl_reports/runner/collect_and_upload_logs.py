
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


def upload_to_ftp( tag, results_file ):
    ftp_site = 'fx.meta-comm.com'
    site_path = '/boost-regression'
    utils.log( "Uploading log archive \"%s\" to ftp://%s%s/%s" % ( results_file, ftp_site, site_path, tag ) )
    
    ftp = ftplib.FTP( ftp_site )
    ftp.login()
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
          locate_root_dir
        , runner_id
        , tag
        , platform
        , comment_file
        , timestamp
        , user
        , source
        , run_type
        ):
    
    results_file = os.path.join( locate_root_dir, '%s.xml' % runner_id )
    results_writer = open( results_file, 'w' )
    utils.log( 'Collecting test logs into "%s"...' % results_file )
    
    if not os.path.exists( timestamp ):
        t = time.gmtime()
        utils.log( 'Warning: timestamp file "%s" does not exist' )
        utils.log( 'Using current UTC time (%s)' % ( timestamp, t ) )
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
            , 'timestamp':  time.strftime( '%a, %d %b %Y %H:%M:%S +0000', t )     
            , 'source':     source
            , 'run-type':   run_type
            }
        )
    
    copy_comments( results_xml, comment_file )
    collect_test_logs( [ locate_root_dir ], results_writer )

    results_xml.endElement( "test-run" )
    results_xml.endDocument()
    results_writer.close()
    utils.log( 'Done writing "%s".' % results_file )

    utils.log( 'Compressing "%s"...' % results_file )
    archive_path = os.path.join( locate_root_dir,'%s.zip' % runner_id )
    z = zipfile.ZipFile( archive_path, 'w', zipfile.ZIP_DEFLATED )
    z.write( results_file, os.path.basename( results_file ) )
    z.close()
    utils.log( 'Done writing "%s".'% archive_path )


def upload_logs( results_dir, runner_id, tag, user ):
    logs_archive = os.path.join( results_dir, '%s.zip' % runner_id )
    upload_to_ftp( tag, logs_archive )


def collect_and_upload_logs( 
          locate_root_dir
        , runner_id
        , tag
        , platform
        , comment_file
        , timestamp
        , user
        , source
        , run_type
        ):
    
    collect_logs( 
          locate_root_dir
        , runner_id
        , tag
        , platform
        , comment_file
        , timestamp
        , user
        , source
        , run_type
        )
    
    upload_logs( locate_root_dir, runner_id, tag, user )


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
        , 'help'
        ]
    
    options = {
          '--tag' :         'CVS-HEAD'
        , '--platform' :    sys.platform
        , '--comment' :     'comment.html'
        , '--timestamp' :   'timestamp'
        , '--user' :        None
        , '--source' :      'CVS'
        , '--run-type' :    'full'
        }
    
    utils.accept_args( args_spec, args, options, usage )
        
    return ( 
          options[ '--locate-root' ]
        , options[ '--runner' ]
        , options[ '--tag' ]
        , options[ '--platform']
        , options[ '--comment' ]
        , options[ '--timestamp' ]
        , options[ '--user' ]
        , options[ '--source' ]
        , options[ '--run-type' ]
        )


def usage():
    print 'Usage: %s [options]' % os.path.basename( sys.argv[0] )
    print    '''
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
'''
    
def main():
    collect_and_upload_logs( *accept_args( sys.argv[ 1 : ] ) )


if __name__ != '__main__':  import utils
else:
    # in absense of relative import...
    xsl_path = os.path.abspath( os.path.dirname( sys.argv[ 0 ] ) )
    while os.path.basename( xsl_path ) != 'xsl_reports': xsl_path = os.path.dirname( xsl_path )
    sys.path.append( xsl_path )

    import utils
    main()
