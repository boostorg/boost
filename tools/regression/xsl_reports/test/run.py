import sys

sys.path.append( '..' )

import os

import report
import merger
import utils


tag = "1_30_0"

utils.makedirs( "results" )
    
all_xml_file = "results/all.xml"
all_xml_writer = open( all_xml_file, "w" )
merger.merge_test_runs( ".", tag, all_xml_writer, 1 )
all_xml_writer.close()

report.make_result_pages( 
      test_results_file = os.path.abspath( all_xml_file )
    , expected_results_file = ""
    , failures_markup_file = os.path.abspath( "explicit-failures-markup.xml" )
    , tag = tag
    , run_date = "Today date"
    , comment_file = os.path.abspath( "comment.html" )
    , results_dir = "results"
    , result_prefix = ""
    , reports = [ "dd" ]
    , v2 = 1
    )



