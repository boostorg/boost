
# Copyright (c) MetaCommunications, Inc. 2003-2004
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import xml.sax.saxutils
import xml.dom.minidom
import zipfile
import glob
import os.path
import sys


def download_test_runs( destination, tag, user ):
    utils.log( 'Downloading test runs for tag "%s" [connecting as %s]...' % ( tag, user ) )

    destination_dir = os.path.join( destination, tag )
    utils.makedirs( destination_dir )

    utils.sourceforge.download( 
          'regression-logs/incoming/%s/' % tag
        , destination_dir
        , user
        )
    
def unzip( archive_path, result_dir ):
    z = zipfile.ZipFile( archive_path, 'r', zipfile.ZIP_DEFLATED ) 
    for f in z.infolist():
        result = open( os.path.join( result_dir, f.filename ), 'w' )
        result.write( z.read( f.filename ) )
        result.close()

    z.close()
    

def unzip_test_runs( dir ):
    files = glob.glob( os.path.join( dir, '*.zip' ) )
    for test_run in files:
        try:
            utils.log( '  Unzipping "%s" ...' % test_run )
            zip_path = os.path.join( dir, test_run )
            unzip( zip_path, dir )
            utils.log( '  Removing "%s" ...' % test_run )
            os.unlink( zip_path )
        except Exception, msg:
            utils.log( '  Skipping "%s" due to errors (%s)' % ( test_run, msg ) )
    
def merge_test_runs( incoming_dir, tag, writer ):
    test_runs_dir = os.path.join( incoming_dir, tag )
    
    utils.log( 'Removing stale XMLs in "%s"...' % test_runs_dir )
    files = glob.glob( os.path.join( test_runs_dir, '*.xml' ) )
    for f in files:  
       	utils.log( '  Removing "%s" ...' % f )
        os.unlink( f )

    utils.log( 'Unzipping new test runs...' )
    unzip_test_runs( test_runs_dir )
    
    all_runs_xml = xml.sax.saxutils.XMLGenerator( writer )
    all_runs_xml.startDocument()
    all_runs_xml.startElement( 'all-test-runs', {} )
    
    utils.log( 'Processing test runs...' )
    files = glob.glob( os.path.join( test_runs_dir, '*.xml' ) )
    for test_run in files:
        try:
            utils.log( '  Loading "%s" in memory...' % test_run )
            run = xml.dom.minidom.parse( test_run  )
            utils.log( '  Writing "%s" into the resulting XML...' % test_run )
            run.documentElement.writexml( writer )
        except Exception, msg:
            utils.log( '  Skipping "%s" due to errors (%s)' % ( test_run, msg ) )

    all_runs_xml.endElement( 'all-test-runs' )
    all_runs_xml.endDocument()
    writer.close()


def merge_logs(
          tag
        , user
        , results_dir
        , results_xml
        , dont_collect_logs
        ):
    
    utils.log( 'Merging test runs into "%s"...' % results_xml )
    
    incoming_dir = os.path.join( results_dir, 'incoming/' )

    utils.log( '  dont_collect_logs: %s' % dont_collect_logs )
    if not dont_collect_logs:
        download_test_runs( incoming_dir, tag, user )
    
    writer = open( results_xml, 'w' )
    merge_test_runs( incoming_dir, tag, writer )

    utils.log( 'Done writing "%s"' % results_xml )


def accept_args( args ):
    args_spec = [ 
          'tag='
        , 'user='
        , 'results-dir='
        , 'results-xml='
        , 'dont-collect-logs'
        , 'help'
        ]

    options = { '--results-xml' : 'all-runs.xml' }
    utils.accept_args( args_spec, args, options, usage )

    return (
          options[ '--tag' ]
        , options[ '--user' ]
        , options[ '--results-dir' ]
        , options[ '--results-xml' ]
        , options.has_key( '--dont-collect-logs' )
        )


def usage():
    print 'Usage: %s [options]' % os.path.basename( sys.argv[0] )
    print    '''
\t--tag                 the tag for the results (e.g. 'CVS-HEAD')
\t--user                SourceForge user name for a shell account
\t--results-dir         directory for the resulting XML
\t--results-xml         name of the resulting XML document (default 'all-runs.xml')
\t--dont-collect-logs   don't collect logs from SourceForge
'''
    
def main():
    merge_logs( *accept_args( sys.argv[ 1 : ] ) )
    

if __name__ != '__main__':  import utils
else:
    # in absense of relative import...
    xsl_path = os.path.abspath( os.path.dirname( sys.argv[ 0 ] ) )
    while os.path.basename( xsl_path ) != 'xsl_reports': xsl_path = os.path.dirname( xsl_path )
    sys.path.append( xsl_path )

    import utils
    main()
