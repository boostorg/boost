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
import re

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

def collect_test_logs( input_dirs, test_results_writer ):
    __log__ = 1
    log( 'Collecting test logs ...' )
    for input_dir in input_dirs:
        log( 'Walking directory "%s" ...' % input_dir )
        os.path.walk( input_dir, process_test_log_files, test_results_writer );

def collect_and_upload_logs( locate_root_dir
                             , runner
                             , tag
                             , platform
                             , comment_file
                             , timestamp
                             , user
                             ):
    test_results_file =  '%s.xml' % runner
    test_results_writer = open( test_results_file, "w" )
    g = xml.sax.saxutils.XMLGenerator( test_results_writer )
    g.startDocument()
    g.startElement( "test-run", { "tag": tag
                                  , "platform": platform
                                  , "runner": runner
                                  , "timestamp": timestamp } )

    g.startElement( "comment", {} )

    f = open( comment_file, "r" )
    try:
        comments = f.read()
    finally:
        f.close()
        
    g.characters( comments )
    g.endElement( "comment" )
    
    collect_test_logs( [ locate_root_dir ], test_results_writer )

    g.endElement( "test-run" )
    g.endDocument()
    upload_results_file( user, tag, test_results_file )

def upload_results_file( user, tag, results_file ):
    checked_system( [ "scp %(file)s %(user)s@shell.sf.net:/home/groups/b/bo/boost/htdocs/regression-logs/incoming/%(tag)s"
                    % { "file": results_file
                        , "user": user
                        , "tag": tag } ] )

def accept_args( args ):
    ( option_pairs, rest_args ) = getopt.getopt( sys.argv[1:], '', [ 'locate-root='
                                                                     , 'runner='
                                                                     , 'tag='
                                                                     , 'platform='
                                                                     , 'comment='
                                                                     , 'timestamp='
                                                                     , 'user='
                                                                     , 'help'
                                                                     ] )
    options = {}
    map( lambda x: options.__setitem__( x[0], x[1] ), option_pairs )

    if ( options.has_key( '--help' ) or len( options.keys() ) + len( rest_args ) == 0 ):
        usage()
        sys.exit( 1 )

    if len( rest_args ) > 0 and rest_args[0][0] == "@":
        f = open( rest_args[0][1:], "r" )
        config_lines  = f.read().splitlines()
        f.close()
        for l in config_lines:
            if re.search( r"^\s*#", l ):  continue
            if re.search( r"^\s*$", l ): continue
            m = re.match( r"^(?P<name>.*?)=(?P<value>.*)", l )
            if m:
                options[ "--%s" % m.group( "name" ) ] = m.group( "value" )
            else:
                raise "Invalid format of config line '%s'" % l
                
        
    return ( options[ '--locate-root' ]
             , options[ '--runner' ]
             , options[ '--tag' ]
             , options[ '--platform']
             , options[ '--comment' ]
             , options[ '--timestamp' ]
             , options[ '--user' ]
             )

def usage():
    print 'Usage: %s [options]' % os.path.basename( sys.argv[0] )
    print    '''
\t--locate-root       directory to to scan for test_log.xml files
\t--tag               the tag for the results (i.e. 'CVS main trunk')
\t--platform          the platform
\t--timestamp         timestamp of the run
\t--comment           an html comment file (will be inserted in the reports)

'''
    
def main():
    apply( collect_and_upload_logs, accept_args( sys.argv[ 1 : ] ) )
    
if __name__ == '__main__':
    main()
