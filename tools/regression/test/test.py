# Copyright (c) MetaCommunications, Inc. 2003-2005
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import difflib
import os
import re
import shutil
import string
import sys



def scan_for_test_cases():
    return [ os.path.join( "test-cases", x ) for x in os.listdir( "test-cases" ) if x != "CVS" ]

def clean_dir( dir ):
    if os.path.exists( dir ):
        shutil.rmtree( dir )
    os.makedirs( dir )

def system( commands ):
    if sys.platform == 'win32':
        f = open( 'tmp.cmd', 'w' )
        f.write( string.join( commands, '\n' ) )
        f.close()
        rc = os.system( 'tmp.cmd' )
        os.unlink( 'tmp.cmd' )
        return rc
    else:
        rc = os.system( '&&'.join( commands ) )
        return rc

def checked_system( commands, valid_return_codes = [ 0 ] ):
    rc = system( commands ) 
    if rc not in [ 0 ] + valid_return_codes:
        raise Exception( 'Command sequence "%s" failed with return code %d' % ( commands, rc ) )
    return rc

def list_recursively( dir ):
    r = []
    for root, dirs, files in os.walk( dir, topdown=False ):
        root = root[ len( dir ) + 1 : ]
        r.extend( [ os.path.join( root, x ) for x in dirs ] )
        r.extend( [ os.path.join( root, x ) for x in files ] )

    return r

def find_process_jam_log():
    root = "../../../"
    
    for root, dirs, files in os.walk( os.path.join( root, "bin" ), topdown=False ):
        if "process_jam_log.exe" in files:
            return os.path.abspath( os.path.normpath( os.path.join( root, "process_jam_log.exe" ) ) )
        if "process_jam_log" in files:
            return os.path.abspath( os.path.normpath( os.path.join( root, "process_jam_log" ) ) )
    return None

def process_jam_log( executable, file, locate_root, results_dir ):
    args = []
    args.append( executable )
    args.append( "--results-dir" )
    args.append( results_dir )
    args.append( locate_root )
    args.append( file )

    cmd = " ".join( args )
    print "Running process_jam_log (%s)" % cmd
    checked_system( [ cmd ] )
    

def read_file( file_path ):
    f = open( file_path )
    try:
        return f.read()
    finally:
        f.close()

def remove_timestamps( log_lines ):
    return [ re.sub( "timestamp=\"[^\"]+\"", "timestamp=\"\"", x ) for x in log_lines ]    
    
def run_test_cases( test_cases ):
    process_jam_log_executable = find_process_jam_log()
    for test_case in test_cases:
        print 'Running test case "%s"' % test_case
        os.chdir( test_case )
        if os.path.exists( "expected" ):
            actual_results_dir = os.path.join( test_case, "actual" )
            f = open( "locate_root.txt" ) 
            try:
                locate_root = f.read().splitlines()[0]
            finally:
                f.close()

            clean_dir( "actual" )
            os.chdir( "actual" )
            process_jam_log( executable = process_jam_log_executable
                             , results_dir = "."
                             , locate_root = locate_root
                             , file="..\\bjam.log" )
            os.chdir( ".." )

            actual_content = list_recursively( "actual" )
            actual_content.sort()

            expected_content = list_recursively( "expected" )
            expected_content.sort()

            structure_diffs = list( difflib.unified_diff( actual_content, expected_content ) )
            if ( len( structure_diffs ) > 0 ):
                raise "Actual results are different from expected \n %s" % "\n".join( structure_diffs )
            else:
                for i in range( 0, len( actual_content ) ):
                    expected_file = os.path.join( "expected", expected_content[ i ] )
                    actual_file = os.path.join( "actual", actual_content[ i ] )
                    
                    print "Comparing %s to %s" % ( expected_file, actual_file )
                    if ( not os.path.isdir( expected_file ) and not os.path.isdir( actual_file ) ):
                        expected = remove_timestamps( read_file( expected_file ).splitlines() )
                        actual = remove_timestamps( read_file( actual_file ).splitlines() )
                        content_diff = list( difflib.unified_diff( expected, actual ) )
                        if ( len( content_diff ) > 0 ):
                            raise "difference \n%s" % "\n".join( content_diff )
                                         
        else:
            raise '   Test case "%s" doesn\'t contain the expected results directory ("expected" )' % ( test_case )
        
run_test_cases( scan_for_test_cases() )
# print find_process_jam_log()
