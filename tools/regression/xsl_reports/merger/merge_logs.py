# Copyright (c) MetaCommunications, Inc. 2003-2004
#
# Use, modification and distribution are subject to the Boost Software 
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy 
# at http://www.boost.org/LICENSE_1_0.txt)

import xml.sax.saxutils

import shutil
import sys
import os
import os.path
import string
import time
import inspect
import getopt

report_types = [ 'us', 'ds', 'ud', 'dd', 'l', 'p', 'x', 'i' ]

if __name__ == '__main__':
    run_dir = os.path.abspath( os.path.dirname( sys.argv[ 0 ] ) )
else:
    run_dir = os.path.abspath( os.path.dirname( sys.modules[ __name__ ].__file__ ) )

class failure_exception:
    def __init__( self, rc ):
        self.rc_ = rc

def log_level():
   frames = inspect.stack()
   level = 0
   for i in frames[ 3: ]:
       if i[0].f_locals.has_key( '__log__' ):
           level = level + i[0].f_locals[ '__log__' ]
   return level
 

def stdlog( message ):
    sys.stderr.write( '# ' + '    ' * log_level() +  message + '\n' );
    sys.stderr.flush()

log = stdlog
# log = lambda x: x

def system( commands ):
    f = open( 'tmp.cmd', 'w' )
    f.write( string.join( commands, '\n' ) )
    f.close()
    rc = os.system( 'tmp.cmd' )
    return rc


def checked_system( commands ):
    rc = system( commands ) 
    if 0 != rc : raise failure_exception( rc )
    return rc


def set_char( str, index, char ):
    return str[ 0 : index ] + char + str[ index + 1: ]


def process_xml_file( input_file, output_file ):
    log( 'Processing test log "%s"' % input_file )
    
    f = open( input_file, 'r' )
    xml = f.readlines();
    f.close()
    
    ascii_chars = ''.join(map(chr, range(0,128)))
    nonascii_chars = ''.join(map(chr, range(128,256)))
    translated_ascii_chars = ''.join( map( lambda x: '?', range(128,256)))
    for i in string.printable:
        if ( ord( i ) < 128 and ord(i) not in ( 12, ) ):
            translated_ascii_chars = set_char( translated_ascii_chars, ord(i), i )
        
    translated_nonascii_chars = ''.join( map( lambda x: '?', range(128,256) ) )
    
    mask_nonascii_translation_table = string.maketrans( ascii_chars + nonascii_chars
                                                        , translated_ascii_chars + translated_nonascii_chars
                                                        )
    for i in range( 0, len(xml)):
        xml[i] = string.translate( xml[i], mask_nonascii_translation_table )
    output_file.writelines( xml )


def process_test_log_files( output_file, dir, names ):
    for file in names:
        if os.path.basename( file ) == 'test_log.xml':
            process_xml_file( os.path.join( dir, file ), output_file )


def map_path( path ):
    return os.path.join( run_dir, path ) 

def xsl_path( xsl_file_name ):
    return map_path( os.path.join( 'xsl', xsl_file_name ) )

def makedirs( path ):
    if not os.path.exists( path ):
        os.makedirs( path )

def xalan( xml_file, xsl_file,output_file, parameters = None ):
    transform_command = 'xalan'
    transform_command = transform_command  + ' -o "%s" ' %  output_file
    if parameters is not None:
         for i in parameters: 
              transform_command = transform_command + ' -p %s "\'%s\'" ' % ( i, parameters[ i ] )
    transform_command = transform_command  + ' "%s"' %  xml_file
    transform_command = transform_command  + ' "%s"' %  xsl_file
    log( transform_command )
    os.system( transform_command )    
                  

def msxsl( xml_file, xsl_file, output_file, parameters = None ):
    transform_command = 'msxsl'
    transform_command = transform_command  + ' "%s"' %  xml_file
    transform_command = transform_command  + ' "%s"' %  xsl_file
    transform_command = transform_command  + ' -o  "%s" ' %  output_file

    if parameters is not None:
         for i in parameters: 
              transform_command = transform_command + ' %s="%s" ' % ( i, parameters[ i ] )

    log( transform_command )
    os.system( transform_command )    

def libxslt( xml_file, xsl_file, output_file, parameters = None ):

    if sys.platform == 'win32':
        os.chdir( os.path.dirname( xsl_file ) )
    transform_command = 'xsltproc'
    transform_command = transform_command + ' -o ' + '%s' % output_file

    if parameters is not None:
         for i in parameters: 
             parameters[i] = parameters[i].replace( '\\', '/' )
             transform_command = transform_command + ' --param %s "\'%s\'" ' % ( i, parameters[ i ] )

    transform_command = transform_command + ' "%s" ' % xsl_file
    transform_command = transform_command + ' "%s" ' % xml_file
    log( transform_command )
    os.system( transform_command )    

registered_xsltprocs = {   'msxsl': msxsl
                         , 'xalan': xalan
                         , 'libxslt': libxslt
                         }


def make_result_pages( test_results_file
                       , expected_results_file
                       , failures_markup_file
                       , source
                       , run_date
                       , comment_file
                       , results_dir
                       , xslt_proc_name
                       , reports
                       ):
    log( 'Producing the reports...' )
    __log__ = 1
    
    if not os.path.exists( results_dir ):
        os.makedirs( results_dir )
        
    xslt_proc = registered_xsltprocs[ xslt_proc_name ]
    
    if comment_file != '':
        comment_file = os.path.abspath( comment_file )
        
    if expected_results_file != '':
        expected_results_file = os.path.abspath( expected_results_file )
    else:
        expected_results_file = os.path.abspath( map_path( 'empty_expected_results.xml' ) )
        

    extended_test_results = os.path.join( results_dir, 'extended_test_results.xml' )
    if 'x' in reports:    
        log( '    Merging with expected results...' )
        xslt_proc( test_results_file
                   , xsl_path( 'add_expected_results.xsl' )
                   , extended_test_results
                   , { 'expected_results_file': expected_results_file, 'failures_markup_file' : failures_markup_file }
                 )

    links = os.path.join( results_dir, 'links.html' )
    
    makedirs( os.path.join( results_dir, 'output' ) )
    for mode in ( 'developer', 'user' ):
        makedirs( os.path.join( results_dir, mode , 'output' ) )

    runners = os.path.join( results_dir, 'runners.html' )
    
    if "n" in reports:
        log( '    Making runner comment files...' )
        xslt_proc( extended_test_results
                   , xsl_path( 'runners.xsl' )
                   , runners
                   )
        
    if 'l' in reports:        
        log( '    Making test output files...' )
        xslt_proc( extended_test_results
                   , xsl_path( 'links_page.xsl' )
                   , links
                   , {
                     'source': source
                     , 'run_date': run_date 
                     , 'comment_file': comment_file
                     , 'explicit_markup_file' : failures_markup_file
                     }
                   )

    issues = os.path.join( results_dir, 'developer', 'issues.html'  )
    if 'i' in reports:
        log( '    Making issues list...' )
        xslt_proc( extended_test_results
                   , xsl_path( 'issues_page.xsl' )
                   , issues
                   , {
                     'source': source
                     , 'run_date': run_date 
                     , 'comment_file': comment_file
                     , 'explicit_markup_file' : failures_markup_file
                     }
                   )
                   
    for mode in ( 'developer', 'user' ):
        if mode[0] + 'd' in reports:
            log( '    Making detailed %s  report...' % mode )
            xslt_proc(  extended_test_results
                        , xsl_path( 'result_page.xsl' )
                        , os.path.join( results_dir, mode, 'index.html' )
                        , { 'links_file': 'links.html'
                            , 'mode': mode
                            , 'source': source
                            , 'run_date': run_date 
                            , 'comment_file': comment_file
                            , 'expected_results_file': expected_results_file
                            , 'explicit_markup_file' : failures_markup_file
                            }
                        );


    for mode in ( 'developer', 'user' ):
        if mode[0] + 's' in reports:
            log( '    Making summary %s  report...' % mode )
            xslt_proc(  extended_test_results
                        , xsl_path( 'summary_page.xsl' )
                        , os.path.join( results_dir, mode, 'summary.html' )
                        , { 'mode' : mode 
                            , 'source': source
                            , 'run_date': run_date 
                            , 'comment_file': comment_file
                            , 'explicit_markup_file' : failures_markup_file
                            }
                        );

    if 'e' in reports:
        log( '    Generating expected_results ...' )
        xslt_proc( extended_test_results
                   , xsl_path( 'produce_expected_results.xsl' )
                   , os.path.join( results_dir, 'expected_results.xml' )
                   )
    
    shutil.copyfile( 
          xsl_path( 'html/master.css' )
        , os.path.join( results_dir, 'master.css' )
        )

def download_test_runs( destination, tag, user ):
    log( "Downloading test runs for tag \"%s\" connecting as %s" % ( tag, user ) )

    checked_system( [ "rsync --verbose --progress --recursive %s@shell.sourceforge.net:/home/groups/b/bo/boost/htdocs/regression-logs/incoming/%s/* %s" % ( user, tag, destination ) ] )
    
def collect_test_runs( locate_root, tag, writer ):
    log( "Collecting test runs" )
    test_run_directory = os.path.join( locate_root, tag )
    log( "   Looking for test runs in the directory \"%s\"" % test_run_directory )
    g = xml.sax.saxutils.XMLGenerator( writer )
    g.startElement( "all-test-runs", {} )
    test_run_files  = os.listdir( test_run_directory )
    
    g.endElement( "all-test-runs" )
    
def merge_logs( locate_root
                , tag
                , expected_results_file
                , failures_markup_file
                , comment_file
                , results_dir
                , user
                , dont_collect_logs = 0
                , reports = report_types
                                ):
    log( "Building reports ..." )
    download_test_runs( "a", tag, user )
    writer = open( "all-runs.xml", "w" )
    collect_test_runs( locate_root, tag, writer )
    sys.exit( 1 )
    
    ( run_date ) = time.strftime('%a, %d %b %Y %H:%M:%S +0000', time.gmtime() )
    test_results_file = os.path.join( results_dir, 'test_results.xml' )
    bin_boost_dir = os.path.join( locate_root_dir, 'bin', 'boost' )
    print 'dont_collect_logs: %s' % dont_collect_logs
    if not dont_collect_logs:
        collect_test_logs( [ os.path.join( bin_boost_dir, 'libs' ), os.path.join( bin_boost_dir, 'status' ) ]
                           , test_results_file
                           )

    make_result_pages( test_results_file
                       , expected_results_file
                       , failures_markup_file
                       , source
                       , run_date
                       , comment_file
                       , results_dir
                       , result_file_prefix
                       , xslt_proc_name
                       , reports
                       )


def accept_args( args ):
    ( option_pairs, rest_args ) = getopt.getopt( sys.argv[1:], '', [ 'locate-root='
                                                                     , 'tag='
                                                                     , 'user='
                                                                     , 'failures-markup='
                                                                     , 'comment='
                                                                     , 'expected-results='
                                                                     , 'results-dir='
                                                                     , 'xsltproc='
                                                                     , 'reports='
                                                                     , 'help'
                                                                     ] )
    options = { '--expected-results': ''
                , '--failures-markup': ''
                , '--reports' : string.join( report_types, ',' )
                , '--comment': ""
                }
    
    map( lambda x: options.__setitem__( x[0], x[1] ), option_pairs )

    if ( options.has_key( '--help' ) or len( options.keys() ) == 5 ):
        usage()
        sys.exit( 1 )

    if not options.has_key( '--results-dir' ):
         options[ '--results-dir' ] = options[ '--locate-root' ]
         
    return { "locate_root": options[ '--locate-root' ]
             , "tag": options[ '--tag' ]
             , "expected_results_file": options[ '--expected-results' ]
             , "failures_markup_file":  options[ '--failures-markup' ]
             , "results_dir": options[ '--results-dir' ]
             , "user": options[ '--user' ]
             , "dont_collect_logs": options.has_key( '--dont-collect-logs' )
             , "reports": options[ '--reports' ].split( ',' )
             , "comment_file": options[ '--comment' ]
             }

def usage():
    print 'Usage: %s [options]' % os.path.basename( sys.argv[0] )
    print    '''
\t--locate-root       the same as --locate-root in compiler_status
\t--tag               the tag for the results (i.e. 'CVS main trunk')
\t--timestamp         ...
\t--comment           an html comment file (will be inserted in the reports)
\t--results-dir       the directory containing -links.html, -fail.html
\t                    files produced by compiler_status (by default the
\t                    same as specified in --locate-root)

'''
    
def main():
    print accept_args( sys.argv[ 1 : ] )
    merge_logs( **accept_args( sys.argv[ 1 : ] ) )
    
if __name__ == '__main__':
    main()
