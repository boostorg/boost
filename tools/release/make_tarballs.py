import sys
import os
import shutil

import optparse


dont_export = 0

def accept_args( args ):
    return ( args[0], args[1], args[2], args[3] )


def clean_directory( directory ):
    if os.path.exists( directory ):
        print "    Removing  directory %s" % directory
        os.system( 'rd /s /q "%s"' % directory )
        
    print "    Creating directory %s" % directory 
    os.makedirs( directory )

def make_temp_platform( temp, platform ):
    temp_platform = os.path.join( temp, platform )
    if not dont_export:
        clean_directory( temp_platform )
    return temp_platform
    
def cvs_export( sf_user, cvs_tag, release_version, shell = "%s" ):
    if not dont_export:
        print "    Exporting..."
        cvs_export_template = 'cvs -d:ext:%(user)s@cvs.sourceforge.net:/cvsroot/boost -z9 export -r %(branch)s boost'

        cmd = cvs_export_template % { "user": sf_user
                                      , "branch" : cvs_tag }

        os.system( shell % cmd )
        os.system( "del /S/F/Q .cvsignore >nul" )
        os.rename( "boost", "boost_%s" % release_version )
        
    
def build_win( release_version, cvs_tag, sf_user, temp_dir ):
    print "Preparing windows .zip"
    temp_win = make_temp_platform( temp_dir, "win" )
    os.chdir( temp_win )

    cvs_export( sf_user, cvs_tag, release_version )
    
    print "    Zipping"
    zip_name = "boost_%s.zip" % release_version 
    if os.path.exists( zip_name ): os.unlink( zip_name )

    os.system( "7z a -r -tzip %s %s\* > %s" % ( os.path.splitext( zip_name )[0], "boost_%s" % release_version, zip_name + ".log" ) )
    return [ os.path.abspath( zip_name ) ]

def correct_permissions( path ):
    print "    Correcting permissions"
    for i in os.walk( path ):
        for f in i[2]:
            if os.path.splitext( f )[1] in ( ".css", ".hpp", ".cpp",\
                                             ".html", ".htm", ".rst", \
                                             ".pdf", ".xml", ".png",\
                                             ".jpg", ".vcproj", ".pattern2", \
                                             ".jam", ".bat", ".sty", ".diff" ) \
               or os.path.basename( f ).lower() in ( "jamfile", "todo", "makefile", "jamrules", "gnumakefile" ):
                print os.path.join( i[0], f )
                os.system( "chmod a=r %s" % os.path.join( i[0], f ) )
    
def build_unix( release_version, cvs_tag, sf_user, temp_dir ):
    print "Preparing unix .gz and .bz2"
    temp_unix = make_temp_platform( temp_dir, "unix" )
    os.chdir( temp_unix )

    cvs_export( sf_user, cvs_tag, release_version, "bash -c \"%s\"" )
    correct_permissions( "." )

    
    
    print "    Making .gz"
    gz_archive_name = "boost_%s" % release_version + ".tar.gz"
    
    if os.path.exists( gz_archive_name  ):  os.unlink( gz_archive_name  )

    os.system( "tar cfz %s %s" % ( gz_archive_name, "boost_%s" % release_version ) )

    bz2_archive_name = "boost_%s" % release_version + ".tar.bz2"
    if os.path.exists( bz2_archive_name  ):  os.unlink( bz2_archive_name )
    
    print "    Making .bz2"
    os.system( 'bash -c "gunzip -c %s | bzip2 > %s"' % ( gz_archive_name, bz2_archive_name ) )
    return [ os.path.abspath( x ) for x in ( gz_archive_name, bz2_archive_name ) ]

def main():
    start_dir = os.getcwd()
    ( release_version, cvs_tag, sf_user, temp_dir ) = accept_args( sys.argv[ 1: ] )

    archives = build_win( release_version, cvs_tag, sf_user, temp_dir )\
               + build_unix( release_version, cvs_tag, sf_user, temp_dir )

    os.chdir( start_dir )
    for archive in archives:
        shutil.copy( archive, start_dir )
    
if __name__ == "__main__":
    main()
