
# Copyright (c) MetaCommunications, Inc. 2003-2004
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import shutil
import os.path
import os
import string
import time
import sys

import utils
import runner


report_types = [ 'us', 'ds', 'ud', 'dd', 'l', 'p', 'x', 'i' ]

if __name__ == '__main__':
    run_dir = os.path.abspath( os.path.dirname( sys.argv[ 0 ] ) )
else:
    run_dir = os.path.abspath( os.path.dirname( sys.modules[ __name__ ].__file__ ) )


def map_path( path ):
    return os.path.join( run_dir, path ) 

def xsl_path( xsl_file_name ):
    return map_path( os.path.join( 'xsl', xsl_file_name ) )


def make_result_pages( 
          test_results_file
        , expected_results_file
        , failures_markup_file
        , source
        , run_date
        , comment_file
        , results_dir
        , result_prefix
        , reports
        ):

    utils.log( 'Producing the reports...' )
    __log__ = 1
    
    output_dir = os.path.join( results_dir, result_prefix )
    utils.makedirs( output_dir )
    
    if comment_file != '':
        comment_file = os.path.abspath( comment_file )
        
    if expected_results_file != '':
        expected_results_file = os.path.abspath( expected_results_file )
    else:
        expected_results_file = os.path.abspath( map_path( 'empty_expected_results.xml' ) )
        

    extended_test_results = os.path.join( output_dir, 'extended_test_results.xml' )
    if 'x' in reports:    
        utils.log( '    Merging with expected results...' )
        utils.libxslt( 
              utils.log
            , test_results_file
            , xsl_path( 'add_expected_results.xsl' )
            , extended_test_results
            , { 'expected_results_file': expected_results_file, 'failures_markup_file' : failures_markup_file }
            )

    links = os.path.join( output_dir, 'links.html' )
    
    utils.makedirs( os.path.join( output_dir, 'output' ) )
    for mode in ( 'developer', 'user' ):
        utils.makedirs( os.path.join( output_dir, mode , 'output' ) )
        
    if 'l' in reports:        
        utils.log( '    Making test output files...' )
        utils.libxslt( 
              utils.log
            , extended_test_results
            , xsl_path( 'links_page.xsl' )
            , links
            , {
                  'source':                 source
                , 'run_date':               run_date 
                , 'comment_file':           comment_file
                , 'explicit_markup_file':   failures_markup_file
                }
            )


    issues = os.path.join( output_dir, 'developer', 'issues.html'  )
    if 'i' in reports:
        utils.log( '    Making issues list...' )
        utils.libxslt( 
              utils.log
            , extended_test_results
            , xsl_path( 'issues_page.xsl' )
            , issues
            , {
                  'source':                 source
                , 'run_date':               run_date
                , 'comment_file':           comment_file
                , 'explicit_markup_file':   failures_markup_file
                }
            )

    for mode in ( 'developer', 'user' ):
        if mode[0] + 'd' in reports:
            utils.log( '    Making detailed %s  report...' % mode )
            utils.libxslt( 
                  utils.log
                , extended_test_results
                , xsl_path( 'result_page.xsl' )
                , os.path.join( output_dir, mode, 'index.html' )
                , { 
                      'links_file':             'links.html'
                    , 'mode':                   mode
                    , 'source':                 source
                    , 'run_date':               run_date 
                    , 'comment_file':           comment_file
                    , 'expected_results_file':  expected_results_file
                    , 'explicit_markup_file' :  failures_markup_file
                    }
                )

    for mode in ( 'developer', 'user' ):
        if mode[0] + 's' in reports:
            utils.log( '    Making summary %s  report...' % mode )
            utils.libxslt(
                  utils.log
                , extended_test_results
                , xsl_path( 'summary_page.xsl' )
                , os.path.join( output_dir, mode, 'summary.html' )
                , { 
                      'mode' :                  mode 
                    , 'source':                 source
                    , 'run_date':               run_date 
                    , 'comment_file':           comment_file
                    , 'explicit_markup_file' :  failures_markup_file
                    }
                )

    if 'e' in reports:
        utils.log( '    Generating expected_results ...' )
        utils.libxslt(
              utils.log
            , extended_test_results
            , xsl_path( 'produce_expected_results.xsl' )
            , os.path.join( output_dir, 'expected_results.xml' )
            )
    
    shutil.copyfile(
          xsl_path( 'html/master.css' )
        , os.path.join( output_dir, 'master.css' )
        )


def build_xsl_reports( 
          locate_root_dir
        , source
        , expected_results_file
        , failures_markup_file
        , comment_file
        , results_dir
        , result_file_prefix
        , dont_collect_logs = 0
        , reports = report_types
        ):

    ( run_date ) = time.strftime('%a, %d %b %Y %H:%M:%S +0000', time.gmtime() )
    
    test_results_file = os.path.join( results_dir, 'test_results.xml' )
    bin_boost_dir = os.path.join( locate_root_dir, 'bin', 'boost' )

    utils.log( '  dont_collect_logs: %s' % dont_collect_logs )
    if not dont_collect_logs:
        f = open( test_results_file, 'w+' )
        f.write( '<tests>\n' )
        runner.collect_test_logs( [ bin_boost_dir ], f )
        f.write( '</tests>\n' )
        f.close()

    make_result_pages( 
          test_results_file
        , expected_results_file
        , failures_markup_file
        , source
        , run_date
        , comment_file
        , results_dir
        , result_file_prefix
        , reports
        )


def accept_args( args ):
    args_spec = [ 
          'locate-root='
        , 'tag='
        , 'expected-results='
        , 'failures-markup='
        , 'comment='
        , 'results-dir='
        , 'results-prefix='
        , 'dont-collect-logs'
        , 'reports='
        , 'help'
        ]
        
    options = { 
          '--comment': ''
        , '--expected-results': ''
        , '--failures-markup': ''
        , '--reports' : string.join( report_types, ',' )
        }
    
    utils.accept_args( args_spec, args, options, usage )
    if not options.has_key( '--results-dir' ):
         options[ '--results-dir' ] = options[ '--locate-root' ]

    return ( 
          options[ '--locate-root' ]
        , options[ '--tag' ]
        , options[ '--expected-results' ]
        , options[ '--failures-markup' ]
        , options[ '--comment' ]
        , options[ '--results-dir' ]
        , options[ '--results-prefix' ]
        , options.has_key( '--dont-collect-logs' )
        , options[ '--reports' ].split( ',' )
        )


def usage():
    print 'Usage: %s [options]' % os.path.basename( sys.argv[0] )
    print    '''
\t--locate-root       the same as --locate-root in compiler_status
\t--tag               the tag for the results (i.e. 'CVS main trunk')
\t--expected-results  the file with the results to be compared with
\t                    the current run
\t--failures-markup   the file with the failures markup
\t--comment           an html comment file (will be inserted in the reports)
\t--results-dir       the directory containing -links.html, -fail.html
\t                    files produced by compiler_status (by default the
\t                    same as specified in --locate-root)
\t--results-prefix    the prefix of -links.html, -fail.html
\t                    files produced by compiler_status

The following options are useful in debugging:

\t--dont-collect-logs dont collect the test logs
\t--reports           produce only the specified reports
\t                        us - user summary
\t                        ds - developer summary
\t                        ud - user detailed
\t                        dd - developer detailed
\t                        l  - links
\t                        p  - patches
\t                        x  - extended results file
\t                        i  - issues
'''

def main():
    build_xsl_reports( *accept_args( sys.argv[ 1 : ] ) )

if __name__ == '__main__':
    main()
