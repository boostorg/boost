
import os.path
import os
import sys

def libxslt( log, xml_file, xsl_file, output_file, parameters = None ):

    if sys.platform == 'win32':
        os.chdir( os.path.dirname( xsl_file ) )
    transform_command = 'xsltproc'
    transform_command = transform_command + ' -o ' + '%s' % output_file

    if parameters is not None:
         for i in parameters:
             if parameters[i]:
                 parameters[i] = parameters[i].replace( '\\', '/' )
                 transform_command = transform_command + ' --param %s "\'%s\'" ' % ( i, parameters[ i ] )

    transform_command = transform_command + ' "%s" ' % xsl_file
    transform_command = transform_command + ' "%s" ' % xml_file
    log( transform_command )
    os.system( transform_command )    
