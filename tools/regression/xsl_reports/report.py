# (C) Copyright MetaCommunications, Inc. 2003.
#
# Permission to use, copy, modify, distribute and sell this software
# and its documentation for any purpose is hereby granted without fee, 
# provided that the above copyright notice appears in all copies and 
# that both the copyright notice and this permission notice appear in 
# supporting documentation. No representations are made about the 
# suitability of this software for any purpose. It is provided "as is" 
# without express or implied warranty.

import shutil
import sys
import os
import os.path
import string
import time
import inspect
import getopt

if __name__ == "__main__":
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
       if i[0].f_locals.has_key( "__log__" ):
           level = level + i[0].f_locals[ "__log__" ]
   return level
 

def stdlog( message ):
    sys.stderr.write( "# " + "    " * log_level() +  message + "\n" );
    sys.stderr.flush()

log = stdlog
# log = lambda x: x

def system( commands ):
    f = open( "tmp.cmd", "w" )
    f.write( string.join( commands, "\n" ) )
    f.close()
    rc = os.system( "tmp.cmd" )
    return rc


def checked_system( commands ):
    rc = system( commands ) 
    if 0 != rc : raise failure_exception( rc )
    return rc


def set_char( str, index, char ):
    return str[ 0 : index ] + char + str[ index + 1: ]


def process_xml_file( input_file, output_file ):
    log( "Processing test log \"%s\"" % input_file )
    
    f = open( input_file, "r" )
    xml = f.readlines();
    f.close()
    
    ascii_chars = ''.join(map(chr, range(0,128)))
    nonascii_chars = ''.join(map(chr, range(128,256)))
    translated_ascii_chars = ''.join( map( lambda x: "?", range(128,256)))
    for i in string.printable:
        if ( ord( i ) < 128 and ord(i) not in ( 12, ) ):
            translated_ascii_chars = set_char( translated_ascii_chars, ord(i), i )
        
    translated_nonascii_chars = ''.join( map( lambda x: "?", range(128,256) ) )
    
    mask_nonascii_translation_table = string.maketrans( ascii_chars + nonascii_chars
                                                        , translated_ascii_chars + translated_nonascii_chars
                                                        )
    for i in range( 0, len(xml)):
        xml[i] = string.translate( xml[i], mask_nonascii_translation_table )
    output_file.writelines( xml )


def process_test_log_files( output_file, dir, names ):
    for file in names:
        if os.path.basename( file ) == "test_log.xml":
            process_xml_file( os.path.join( dir, file ), output_file )


def collect_test_logs( input_dirs, output_file ):
    __log__ = 1
    log( "Collecting test logs ..." )
    f = open( output_file, "w+" )
    f.write( "<tests>\n" );
    for input_dir in input_dirs:
        os.path.walk( input_dir, process_test_log_files, f );
    f.write( "</tests>\n" );
    f.close()


def xalan( xml_file, xsl_file,output_file, parameters = None ):
    transform_command = "xalan"
    transform_command = transform_command  + ' -o "%s" ' %  output_file
    if parameters is not None:
         for i in parameters: 
              transform_command = transform_command + ' -p %s "\'%s\'" ' % ( i, parameters[ i ] )
    transform_command = transform_command  + ' "%s"' %  xml_file
    transform_command = transform_command  + ' "%s"' %  xsl_file
    log( transform_command )
    os.system( transform_command )    
                  

def msxsl( xml_file, xsl_file, output_file, parameters = None ):
    transform_command = "msxsl"
    transform_command = transform_command  + ' "%s"' %  xml_file
    transform_command = transform_command  + ' "%s"' %  xsl_file
    transform_command = transform_command  + ' -o  "%s" ' %  output_file

    if parameters is not None:
         for i in parameters: 
              transform_command = transform_command + ' %s="%s" ' % ( i, parameters[ i ] )

    log( transform_command )
    os.system( transform_command )    

def libxslt( xml_file, xsl_file, output_file, parameters = None ):
    transform_command = "xsltproc"
    transform_command = transform_command + ' -o ' + "%s" % output_file

    if parameters is not None:
         for i in parameters: 
             parameters[i] = parameters[i].replace( "\\", "/" )
             transform_command = transform_command + ' --param %s "\'%s\'" ' % ( i, parameters[ i ] )

    transform_command = transform_command + ' "%s" ' % xsl_file
    transform_command = transform_command + ' "%s" ' % xml_file
    log( transform_command )
    os.system( transform_command )    


registered_xsltprocs = {   "msxsl": msxsl
                         , "xalan": xalan
                         , "libxslt": libxslt
                         }

def map_path( path ):
    return os.path.join( run_dir, path ) 

def xsl_path( xsl_file_name ):
    return map_path( os.path.join( "xsl", xsl_file_name ) )

def make_result_pages( test_results_file
                       , expected_results_file
                       , failures_markup_file
                       , source
                       , run_date
                       , comment_file
                       , results_dir
                       , result_prefix
                       , xslt_proc_name
                       , reports
                       ):
    log( "Producing the reports..." )
    __log__ = 1
    
    output_dir = os.path.join( results_dir, result_prefix )
    if not os.path.exists( output_dir ):
        os.makedirs( output_dir )
        
    xslt_proc = registered_xsltprocs[ xslt_proc_name ]
    
    if comment_file != "":
        comment_file = os.path.abspath( comment_file )
        
    if expected_results_file != "":
        expected_results_file = os.path.abspath( expected_results_file )
    else:
        expected_results_file = os.path.abspath( map_path( "empty_expected_results.xml" ) )
        

    extended_test_results = os.path.join( output_dir, "extended_test_results.xml" )
    if "x" in reports:    
        log( "    Merging with expected results..." )
        xslt_proc( test_results_file
                   , xsl_path( "add_expected_results.xsl" )
                   , extended_test_results
                   , { "expected_results_file": expected_results_file, "failures_markup_file" : failures_markup_file }
                 )

    links = os.path.join( output_dir, "links.html"  )
    if "l" in reports:        
        log( "    Making -links file..." )
        xslt_proc( extended_test_results
                   , xsl_path( "links_page.xsl" )
                   , links
                   , {
                     "source": source
                     , "run_date": run_date 
                     , "comment_file": comment_file
                     }
                   )


    for mode in ( "developer", "user" ):
        if mode[0] + "d" in reports:
            log( "    Making detailed %s  report..." % mode )
            xslt_proc(  extended_test_results
                        , xsl_path( "result_page.xsl" )
                        , os.path.join( output_dir, "%s_%s" % ( mode, "result_page.html" ) )
                        , { "links_file": "links.html"
                            , "mode": mode
                            , "source": source
                            , "run_date": run_date 
                            , "comment_file": comment_file
                            , "expected_results_file": expected_results_file
                            , "explicit_markup_file" : failures_markup_file
                            }
                        );


    for mode in ( "developer", "user" ):
        if mode[0] + "s" in reports:
            log( "    Making summary %s  report..." % mode )
            xslt_proc(  extended_test_results
                        , xsl_path( "summary_page.xsl" )
                        , os.path.join( output_dir, "%s_%s" % ( mode, "summary_page.html" ) )
                        , { "mode" : mode 
                            , "source": source
                            , "run_date": run_date 
                            , "comment_file": comment_file
                            , "explicit_markup_file" : failures_markup_file
                            }
                        );

    if "e" in reports:
        log( "    Generating expected_results ..." )
        xslt_proc( extended_test_results
                   , xsl_path( "produce_expected_results.xsl" )
                   , os.path.join( output_dir, "expected_results.xml" )
                   )
    
    shutil.copyfile( xsl_path( "master.css" ),  os.path.join( output_dir, "master.css" ) )


def build_experimental_reports( locate_root_dir
                                , source
                                , expected_results_file
                                , failures_markup_file
                                , comment_file
                                , results_dir
                                , result_file_prefix
                                , xslt_proc_name
                                , dont_collect_logs
                                , reports
                                ):
    ( run_date ) = time.strftime("%a, %d %b %Y %H:%M:%S +0000", time.gmtime() )
    test_results_file = os.path.join( results_dir, "test_results.xml" )
    print "dont_collect_logs: %s" % dont_collect_logs
    if not dont_collect_logs:
        collect_test_logs( [ os.path.join( locate_root_dir, "libs" ), os.path.join( locate_root_dir, "status" ) ]
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
    ( option_pairs, rest_args ) = getopt.getopt( sys.argv[1:], "", [ "locate-root="
                                                                     , "tag="
                                                                     , "expected-results="
                                                                     , "failures-markup="
                                                                     , "comment="
                                                                     , "results-dir="
                                                                     , "results-prefix="
                                                                     , "xsltproc="
                                                                     , "dont-collect-logs"
                                                                     , "reports="
                                                                     , "help"
                                                                     ] )
    options = { "--comment": ""
                , "--expected-results": ""
                , "--failures-markup": ""
                , "--reports" : "dd,ud,us,ds,l,p,x" }
    
    map( lambda x: options.__setitem__( x[0], x[1] ), option_pairs )

    if ( options.has_key( "--help" ) or len( options.keys() ) == 4 ):
        usage()
        sys.exit( 1 )

    if not options.has_key( "--results-dir" ):
         options[ "--results-dir" ] = options[ "--locate-root" ]
         
    return ( options[ "--locate-root" ]
             , options[ "--tag" ]
             , options[ "--expected-results" ]
             , options[ "--failures-markup" ]
             , options[ "--comment" ]
             , options[ "--results-dir" ]
             , options[ "--results-prefix" ]
             , options[ "--xsltproc" ]
             , options.has_key( '--dont-collect-logs' )
             , options[ "--reports" ].split( "," )
             )

def usage():
    print "Usage: %s [options]" % os.path.basename( sys.argv[0] )
    print    """
\t--locate-root       the same as --locate-root in compiler_status
\t--tag               the tag for the results (i.e. "CVS main trunk")
\t--expected-results  the file with the results to be compared with
\t                    the current run
\t--failures-markup   the file with the failures markup
\t--comment           an html comment file (will be inserted in the reports)
\t--results-dir       the directory containing -links.html, -fail.html
\t                    files produced by compiler_status (by default the
\t                    same as specified in --locate-root)
\t--results-prefix    the prefix of -links.html, -fail.html
\t                    files produced by compiler_status
\t--xsltproc          the name of xslt processor (msxsl, xalan, libxslt)

                      The XSLT used in report generation uses exsl 
                      (http:///www.exsl.org). Make sure that specified 
                      processor supports it.

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
    """

def main():
    apply( build_experimental_reports, accept_args( sys.argv[ 1 : ] ) )
    
if __name__ == '__main__':
    main()
