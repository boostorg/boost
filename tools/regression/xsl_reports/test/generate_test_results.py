import xml.sax.saxutils

import os
import time

number_of_libs = 2
number_of_runners = 3
initial_number_of_toolsets = 3
number_of_tests = 10

tag = "1_30_0"

def make_test_name( library_idx, test_idx ):
    return "test_%02d_%02d" % ( library_idx, test_idx )

def make_library_name( library_idx ):
    return "library_%02d" % library_idx

def make_toolset_name( toolset_idx ):
    return "toolset_%02d" % toolset_idx

def make_library_target_directory( library_idx, toolset_idx ):
    return "lib/%s/%s" % ( make_library_name( library_idx )
                           , make_toolset_name( toolset_idx ) )

def make_test_target_directory( library_idx, toolset_idx, test_idx ):
    return "%s/%s/%s" % ( make_library_name( library_idx )
                          , make_toolset_name( toolset_idx )
                          , make_test_name( test_idx ) )
    

def library_build_failed( library_idx ):
    return library_idx % 2

def make_test_results():
    if not os.path.exists( tag ):
        os.makedirs( tag )
    for i_runner in range( 0, number_of_runners ):
        runner_id = "runner_%02d" % i_runner
        g = xml.sax.saxutils.XMLGenerator( open( os.path.join( tag, runner_id + ".xml" ), "w" ) )
        platform = "Win32"
        g.startElement( "test-run", { "platform": platform
                                      , "runner": runner_id
                                      , "timestamp": time.strftime( "%a, %d %b %Y %H:%M:%S +0000"
                                                                   , time.gmtime()) } )

        g.startElement( "comment", {} )
        g.characters( "<b>Runner</b> is who <i>running</i> does." )
        g.endElement( "comment" )

        for i_lib in range( 0, number_of_libs ):
            for i_toolset in range( initial_number_of_toolsets ):
                if library_build_failed( i_lib ): test_result = "fail"
                else:                             test_result = "success"
                    
                make_test_log( xml_generator = g
                               , library_idx = i_lib
                               , toolset_idx = i_toolset 
                               , test_name = ""
                               , test_type = "lib"
                               , test_result = test_result
                               , show_run_output = "false" )

    
        for i_lib in range( 0, number_of_libs ):
            library_name = "library_%02d" % i_lib
            if number_of_runners - 1 == i_runner and  i_lib % 2: 
                continue 
                
            for i_toolset in range( initial_number_of_toolsets ):
                toolset_name = "toolset_%02d" % ( i_toolset )

                if number_of_runners - 1 == i_runner and i_toolset % 2:
                    continue
                
                for i_test in range( number_of_tests ):
                    test_name = "test_%02d_%02d" % ( i_lib, i_test )
                    test_result = ""
                    test_type = "run"
                    show_run_output = "false"
                    
                    if number_of_runners - 1 == i_runner and i_test % 2:
                        continue
                    
                    if i_runner % 2: test_result = "success"
                    else:             test_result = "fail"

                    if test_result == "success" and ( 0 == i_test % 5 ):
                        show_run_output = "true"
                        
                    make_test_log( g, i_lib, i_toolset, test_name, test_type, test_result, show_run_output )
        g.endElement( "test-run" )


def make_expicit_failure_markup():
    g = xml.sax.saxutils.XMLGenerator( open( "explicit-failures-markup.xml", "w" ) )
    g.startElement( "explicit-failures-markup", {} );

    # required toolsets
    for i_toolset in range( 0, initial_number_of_toolsets ):
        if i_toolset < 2:
            g.startElement( "mark-toolset", { "name": "toolset_%02d" % i_toolset, "status":"required"} )
            g.endElement( "mark-toolset" )

    for i_library in range( 0, number_of_libs ):
        g.startElement( "library", { "name": make_library_name( i_library ) } )
        if i_library % 4 == 0:
            g.startElement( "mark-unusable", {} )
            for i_toolset in range( 0, initial_number_of_toolsets ):
                if i_toolset % 2 == 1:
                    g.startElement( "toolset", { "name": make_toolset_name( i_toolset ) } )
                    g.endElement( "toolset" )
            g.startElement( "note", { "author": "T. Test" } )
            g.characters( "Test note" )
            g.endElement( "note" )
            g.endElement( "mark-unusable" )

        for i_test in range( 0, number_of_tests ):
            if i_test > number_of_tests - 3:     corner_case_test = 1
            else:                                corner_case_test = 0
            
            if i_test % 4 == 0:      explicitly_marked_test = 1
            else:                    explicitly_marked_test = 0

            if corner_case_test or explicitly_marked_test:
                test_attrs = { "name": make_test_name( i_library, i_test ) }
                if corner_case_test:
                    test_attrs[ "corner_case" ] = "yes"
                g.startElement( "test", test_attrs )
                if explicitly_marked_test:
                    g.startElement( "mark-failure", {} )
                    
                    g.startElement( "toolset", { "name": make_toolset_name( 1 ) } )
                    g.endElement( "toolset" )
                    g.startElement( "toolset", { "name": make_toolset_name( 0 ) } )
                    g.endElement( "toolset" )

                    g.startElement( "note", {  "author": "V. Annotated" } )
                    g.characters( "Some thoughtful note" )
                    g.endElement( "note" )
                    
                    g.endElement( "mark-failure" )
                    
                g.endElement( "test" );
        g.endElement( "library" )
            
        
    g.endElement( "explicit-failures-markup" )

def make_test_log( xml_generator, library_idx, toolset_idx, test_name, test_type, test_result, show_run_output ):
    library = make_library_name( library_idx )
    toolset_name = make_toolset_name( toolset_idx )
    
    target_directory = ""

    if test_type == "run":
        target_directory = "%s/%s/%s" % ( library, toolset_name, test_name )
    if test_type == "lib":
        target_directory = make_library_target_directory( library_idx, toolset_idx )
        
    xml_generator.startElement( "test-log", { "library": library
                                  , "test-name":  test_name
                                  , "toolset": toolset_name
                                  , "test-type": test_type
                                  , "test-program": "some_program"
                                  , "target-directory": target_directory
                                  , "show-run-output": show_run_output
                                  } )
    if test_type ==  "run":
        xml_generator.startElement( "compile", { "result": "success" } );
        xml_generator.characters( "Compiling in %s" % target_directory )
        xml_generator.endElement( "compile" )

        
        xml_generator.startElement( "lib", { "result": test_result } );
        xml_generator.characters( make_library_target_directory( library_idx, toolset_idx ) )
        xml_generator.endElement( "lib" )

        xml_generator.startElement( "link", { "result": "success" } );
        xml_generator.characters( "Linking in %s" % target_directory )
        xml_generator.endElement( "link" )

        xml_generator.startElement( "run", { "result": test_result } );
        xml_generator.characters( "Running in %s" % target_directory )
        xml_generator.endElement( "run" )

    if test_type ==  "lib":
        xml_generator.startElement( "compile", { "result": test_result } );
        xml_generator.characters( "Compiling in %s" % make_library_target_directory( library_idx, toolset_idx ) )
        xml_generator.endElement( "compile" )



    xml_generator.endElement( "test-log" )



## <test-log library="algorithm" test-name="container" test-type="run" test-program="libs/algorithm/string/test/container_test.cpp" target-directory="bin/boost/libs/algorithm/string/test/container.test/borland-5.6.4/debug" toolset="borland-5.6.4" show-run-output="false">
## <compile result="fail" timestamp="2004-06-29 17:02:27 UTC">

##     "C:\Progra~1\Borland\CBuilder6\bin\bcc32"  -j5 -g255 -q -c -P -w -Ve -Vx -a8 -b-   -v -Od -vi- -tWC -tWR -tWC -WM- -DBOOST_ALL_NO_LIB=1  -w-8001  -I"C:\Users\Administrator\boost\main\results\bin\boost\libs\algorithm\string\test"   -I"C:\Users\Administrator\boost\main\boost" -I"C:\Progra~1\Borland\CBuilder6\include"  -o"C:\Users\Administrator\boost\main\results\bin\boost\libs\algorithm\string\test\container.test\borland-5.6.4\debug\container_test.obj"  "..\libs\algorithm\string\test\container_test.cpp" 

## ..\libs\algorithm\string\test\container_test.cpp:
## Warning W8091 C:\Users\Administrator\boost\main\boost\libs/test/src/unit_test_result.cpp 323: template argument _InputIter passed to 'for_each' is a output iterator: input iterator required in function unit_test_result::~unit_test_result()
## Warning W8091 C:\Users\Administrator\boost\main\boost\libs/test/src/unit_test_suite.cpp 63: template argument _InputIter passed to 'find_if' is a output iterator: input iterator required in function test_case::Impl::check_dependencies()
## Warning W8091 C:\Users\Administrator\boost\main\boost\libs/test/src/unit_test_suite.cpp 204: template argument _InputIter passed to 'for_each' is a output iterator: input iterator required in function test_suite::~test_suite()
## Error E2401 C:\Users\Administrator\boost\main\boost\boost/algorithm/string/finder.hpp 45: Invalid template argument list
## Error E2040 C:\Users\Administrator\boost\main\boost\boost/algorithm/string/finder.hpp 46: Declaration terminated incorrectly
## Error E2090 C:\Users\Administrator\boost\main\boost\boost/algorithm/string/finder.hpp 277: Qualifier 'algorithm' is not a class or namespace name
## Error E2272 C:\Users\Administrator\boost\main\boost\boost/algorithm/string/finder.hpp 277: Identifier expected
## Error E2090 C:\Users\Administrator\boost\main\boost\boost/algorithm/string/finder.hpp 278: Qualifier 'algorithm' is not a class or namespace name
## Error E2228 C:\Users\Administrator\boost\main\boost\boost/algorithm/string/finder.hpp 278: Too many error or warning messages
## *** 6 errors in Compile ***
## </compile>
## </test-log>
        
    
make_test_results()
make_expicit_failure_markup()
