import os
import sys
import win32com.client

objBL = win32com.client.Dispatch("SQLXMLBulkLoad.SQLXMLBulkload.3.0")
objBL.ConnectionString = "provider=SQLOLEDB;data source=localhost;database=boost;integrated security=SSPI"
objBL.ErrorLogFile = "c:\\error.log"
#objBL.Execute( os.path.join( os.path.dirname( __file__ ), "test-runs.xsd" ), "Huang-WinXP-x64.xml" )
#objBL.Execute( os.path.join( os.path.dirname( __file__ ), "test-runs.xsd" ), "extended_test_results.xml" )
objBL.Execute( os.path.join( os.path.dirname( __file__ ), "test-runs.xsd" ), sys.argv[1] )