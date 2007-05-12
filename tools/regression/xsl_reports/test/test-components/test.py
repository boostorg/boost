import new 
import os
import unittest
import sys

sys.path.append( os.path.normpath( os.path.join( os.path.abspath( os.path.dirname( __file__ ) ), '../..' ) ) )

import utils.libxslt

#1. collect test_cases
#2. Add test cases to test suite
#3. Run test cases

class test_case_definition:
    def __init__(self, root, name ):
        self._root = root
        self._name = name

    def root(self):
        return self._root
    
    def name(self):
        return self._name
    
    def path(self):
        return os.path.join( self._root, self._name )
    
    def xsl_driver( self ):
        return os.path.join( self.path(), 'test-driver.xsl' )
    
    def xml_data_files( self ):
        data_files = [ x for x in os.listdir( self.path() ) if os.path.splitext( x )[1] == '.xml' ]
        return data_files
    
def collect_test_cases():
    test_cases_dir = os.path.join( os.path.dirname( __file__ ) )
    test_cases = os.listdir( test_cases_dir )
    def is_disabled( test_case ):
        return os.path.exists( os.path.join( test_case, '.disabled' ) )
    return [ test_case_definition( test_cases_dir, x ) for x in test_cases if os.path.isdir( os.path.join( test_cases_dir, x ) ) and not is_disabled(x) ]
    
def log( msg ):
    print msg 

def read_file_lines( path ):
    f = open( path )
    try:
        return f.readlines()
    finally:
        f.close()

def write_file_lines( path, content ):
    f = open( path, 'w' )
    try:
        return f.writelines( content )
    finally:
        f.close()
        
def make_test_case( test_case ):
    def test_case_method( self ):
        for test_case_xml in test_case.xml_data_files():
            print test_case_xml
            result_file_name = os.path.splitext( os.path.basename( test_case_xml ) )[0] + '.xml'
            ( result_file, expected_file ) = [ os.path.join( test_case.path(), x, result_file_name ) for x in ( 'actual', 'expected' ) ]
            print ( result_file, expected_file )
            saved_cwd = os.getcwd()
            try:
                os.chdir( test_case.path() )
                utils.libxslt( log, test_case_xml, test_case.xsl_driver(), result_file )
            finally:
                os.chdir( saved_cwd )
            self.failUnlessEqual( read_file_lines( expected_file ), read_file_lines( result_file ) )
    test_case_method.__name__ = test_case.name()
    return test_case_method

class test_components(unittest.TestCase):    
    pass
    
if __name__ == '__main__':
    for test_case in collect_test_cases():
        print test_case.name() 
        setattr( test_components, test_case.name(), make_test_case(test_case) )
    unittest.main()    
    
    
    
    