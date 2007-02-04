import ftplib
import optparse
import os
import time
import urlparse
import utils
import sys
import zipfile

def get_date( words ):
    date = words[ 5: -1 ]
    t = time.localtime()

    month_names = [ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" ]

    year = time.localtime()[0] # If year is not secified is it the current year
    month = month_names.index( date[0] ) + 1
    day = int( date[1] )
    hours = 0 
    minutes = 0

    if  date[2].find( ":" ) != -1:
        ( hours, minutes ) = [ int(x) for x in date[2].split( ":" ) ]
    else:
        # there is no way to get seconds for not current year dates
        year = int( date[2] )

    return ( year, month, day, hours, minutes, 0, 0, 0, 0 )

#def check_for_new_upload( target_dir, boostbook_info ):

def accept_args( args ):        
    parser = optparse.OptionParser()
    parser.add_option( '-t', '--tag', dest='tag', help="the tag for the results (i.e. 'RC_1_34_0')" )
    parser.add_option( '-d', '--destination', dest='destination', help='destination directory' )

    if len(args) == 0:
        parser.print_help()
        sys.exit( 1 )

    (options, args) = parser.parse_args()
    if not options.destination:
        print '-d is required'
        parser.print_help()
        sys.exit( 1 )
    return options

def unzip( archive_path, result_dir ):
    utils.log( 'Unpacking %s into %s' % ( archive_path, result_dir ) )
    z = zipfile.ZipFile( archive_path, 'r', zipfile.ZIP_DEFLATED ) 
    for f in z.infolist():
        dir = os.path.join( result_dir, os.path.dirname( f.filename ) )
        if not os.path.exists( dir ):
            os.makedirs( dir )
        result = open( os.path.join( result_dir, f.filename ), 'wb' )
        result.write( z.read( f.filename ) )
        result.close()

    z.close()

def boostbook_report( options ):
    site = 'fx.meta-comm.com'
    site_path = '/boost-regression/%s' % options.tag
    
    utils.log( 'Opening %s ...' % site )
    f = ftplib.FTP( site )
    f.login()
    utils.log( '   cd %s ...' % site_path )
    f.cwd( site_path )
    
    utils.log( '   dir' )
    lines = []
    f.dir( lambda x: lines.append( x ) )
    word_lines = [ x.split( None, 8 ) for x in lines ]
    boostbook_info = [ ( l[-1], get_date( l ) ) for l in word_lines if l[-1] == "BoostBook.zip" ]
    if len( boostbook_info ) > 0:
        boostbook_info = boostbook_info[0]
        utils.log( 'BoostBook found! (%s)' % ( boostbook_info, ) )
        local_copy = os.path.join( options.destination,'BoostBook.zip' )
        
        if 0: 
            if os.path.exists( local_copy ):
                utils.log( 'Local copy exists. Checking if it is older than uploaded one...' )
                uploaded_mtime = time.mktime( boostbook_info[1] )
                local_mtime    = os.path.getmtime( local_copy )
                utils.log( '    uploaded: %s %s, local: %s %s' % 
                           ( uploaded_mtime
                             , boostbook_info[1]
                             , local_mtime 
                             , time.localtime( local_mtime )) ) 
                modtime = time.localtime( os.path.getmtime( local_copy ) )
                if uploaded_mtime <= local_mtime:
                    utils.log( 'Local copy is newer: exiting' )
                    sys.exit()
                
        if 0:
            temp = os.path.join( options.destination,'BoostBook.zip' )
            result = open( temp, 'wb' )
            f.retrbinary( 'RETR %s' % boostbook_info[0], result.write )
            result.close()
            
            os.rename( temp, local_copy )
            m = time.mktime( boostbook_info[1] )
            os.utime( local_copy, ( m, m ) )

        unzip( local_copy, options.destination )
    
def main():
    options = accept_args( sys.argv[1:])
    boostbook_report( options )

if __name__ == '__main__':
    main()