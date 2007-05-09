
import utils.makedirs
import os.path
import os
import sys


def xslt_param( path ):
    path = path.replace( '\\', '/' )
    if sys.platform == 'win32':
        path = path.replace( ' ', '%20' )
    return path


def libxslt( log, xml_file, xsl_file, output_file, parameters = None ):

    utils.makedirs( os.path.dirname( output_file ) )
    
    if sys.platform == 'win32':
        os.chdir( os.path.dirname( xsl_file ) )

    transform_command = 'xsltproc'
    transform_command = transform_command + ' -o ' + '"%s"' % xslt_param( output_file )

    if parameters is not None:
         for i in parameters:
             if parameters[i]:
                 parameters[i] = xslt_param( parameters[i] )
                 transform_command = transform_command + ' --param %s "\'%s\'" ' % ( i, parameters[ i ] )

    transform_command = transform_command + ' "%s" ' % xslt_param( xsl_file )
    transform_command = transform_command + ' "%s" ' % xslt_param( xml_file )
    log( transform_command )
    rc = os.system( transform_command )
    if rc != 0:
        raise Exception( '"%s" failed with return code %d' % ( transform_command, rc ) )

