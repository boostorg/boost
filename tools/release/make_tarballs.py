# Copyright (c) MetaCommunications, Inc. 2003-2004
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import sys
import os
import shutil
import optparse


import utils

my_location  = os.path.abspath( os.path.dirname( sys.argv[0] ) )

def accept_args( args ):
    #( release_version, cvs_tag, sf_user, temp_dir, start_step ) = accept_args( sys.argv[ 1: ] )
    parser = optparse.OptionParser()
    parser.add_option( "-v", "--release-version", dest="release_version", metavar="release-version", help="release version (e.g. 1.32.0)")
    parser.add_option( "", "--tag",  dest="tag", help="CVS tag" )
    parser.add_option( "-r", "--cvs-branch", dest="cvs_branch", metavar="cvs-branch"
                       , help = "cvs branch to get the sources from (e.g RC_1_32_0). Important: it is case sensitive" )
    parser.add_option( "-u", "--sf-user", dest="sf_user", metavar="sf-user"
                       , help = "SourceForge user name (for CVS)" )
    parser.add_option( "-t", "--toolset", dest="toolset", help="toolset to use to build needed tools" )
    parser.add_option( "-s", "--start-step", dest="start_step" )
    parser.usage = "make_tarballs [options] target_directory \n\n" + \
                   "Requirements:\n" + \
                   "  CVS:\n"+ \
                   "    cvs             - (windows) to export sources with windows newlines \n" + \
                   "    /usr/bin/cvs    - (cygwin) to export sources with posix newlines\n" + \
                   "  Utilities:\n" + \
                   "    mv              - (cygwin) posix move\n" + \
                   "    /usr/bin/find   - (cygwin) to export sources with posix newlines\n" + \
                   "    7z              - to create zipball\n" + \
                   "  BoostBook generation:\n" + \
                   "    bjam\n" + \
                   "    user-config.jam - in user directory ($HOME/%HOME%)  for BoostBook generation\n" + \
                   "    java\n" + \
                   "    doxygen\n"
    
    

    ( options, args ) = parser.parse_args( args )

    temp_dir = None
    start_step = None
    if ( len( args ) > 0 ): temp_dir = args[0]

    
    ( version, tag, user, toolset, start_step ) =  ( options.release_version
                                         , options.cvs_branch
                                         , options.sf_user
                                         , options.toolset
                                         , options.start_step )

    if ( start_step is None ): start_step = ""

    def required( value, name ):
       if ( value is None ):
            print "%s should be specified." % name
            parser.print_help()
            sys.exit( 1 )
       
    required( version, "version" )
    required( tag, "tag" )
    required( user, "user" )
    required( temp_dir, "temp_dir" )
    required( toolset, "toolset" )
                       
    return ( version, tag, user, toolset, temp_dir, start_step )


def remove_directory( directory ):
    if os.path.exists( directory ):
        print "    Removing  directory %s" % directory
        os.system( 'rd /s /q "%s"' % directory )
    
def clean_directory( directory ):
    remove_directory( directory )
    print "    Creating directory %s" % directory 
    os.makedirs( directory )

def listdir_recursively( root, path="" ):
    # recursive listdir
    files = []
    try:
        for file in os.listdir(os.path.join(root, path)):
            pathname = os.path.join(path, file)
            if os.path.isdir(os.path.join(root, pathname)):
                files.extend(listdir_recursively(root, pathname))
            else:
                files.append(pathname)
    except OSError:
        pass
    return files

def find_file( root, name ):
    print root
    files = listdir_recursively( root )
    for file in files:
        # print file
        if os.path.basename( file ) == name:
            return os.path.join( root, file )
    return None

start_dir = os.getcwd()

class make_tarballs( utils.step_controller ):
    def __init__( self, release_version, cvs_tag, sf_user, toolset, temp_dir, start_step ):
        utils.step_controller.__init__( self, start_step )
        self.release_version_ = release_version
        self.cvs_tag_         = cvs_tag
        self.sf_user_         = sf_user
        self.toolset_         = toolset
        self.temp_dir_        = temp_dir
    
    def run( self ):
        archives = []

        win_build_results = self.build_win( self.release_version_
                                         , self.cvs_tag_
                                         , self.sf_user_
                                         , self.temp_dir_ )
        archives.extend( win_build_results[1] )
        
        archives.extend( self.build_unix( self.release_version_
                                         , self.cvs_tag_
                                         , self.sf_user_
                                         , self.temp_dir_
                                         , win_build_results[0] ) )

        
        #    os.chdir( start_dir )
        #    for archive in archives:
        #        shutil.copy( archive, start_dir )


    def make_temp_platform( self, temp, platform ):
        temp_platform = os.path.join( temp, platform )
        if not self.is_skipping():
            clean_directory( temp_platform )
        return temp_platform

    def cvs_export( self, sf_user, cvs_tag, release_version, shell = "%s" ):
        if not self.is_skipping():
            print "    Exporting..."
            cvs_export_template = 'cvs -d:ext:%(user)s@cvs.sourceforge.net:/cvsroot/boost -z9 export -r %(branch)s boost'

            cmd = cvs_export_template % { "user": sf_user
                                          , "branch" : cvs_tag }

            print cmd
            os.system( shell % cmd )
            os.system( "del /S/F/Q .cvsignore >nul" )
            # have to use mv instead of os.rename - cygwin cvs sets strange directory permssions
            # which Windows rename or Python's os.rename cannot deal with
            os.system( "mv boost boost_%s" % release_version )
        return "boost_%s" % release_version

    def build_win( self, release_version, cvs_tag, sf_user, temp_dir ):

        if "win.export":
            self.start_step( "win.export", "Exporting windows copy" )

            temp_win = self.make_temp_platform( temp_dir, "win" )
            os.chdir( temp_win )

            exported_dir = self.cvs_export( sf_user, cvs_tag, release_version )
            self.finish_step( "win.export" )
        
        self.make_docs( os.path.abspath( exported_dir ), temp_dir )

        if self.start_step( "win.make_readonly", "Making all files writable" ):
            os.chdir( temp_win )
            utils.checked_system( [ "attrib /S -R *.*" ] )
            self.finish_step( "win.make_readonly" )
        
        zip_name = "boost_%s.zip" % release_version
        os.chdir( temp_win )

        
        if self.start_step( "win.zip", "    Zipping" ):
             print "    Zipping"
             if os.path.exists( zip_name ): os.unlink( zip_name )

             utils.checked_system( ["7z a -r -tzip %s %s\* > %s" % ( zip_name, "boost_%s" % release_version, zip_name + ".log" ) ] )
             self.finish_step( "win.zip" )


        return ( os.path.abspath( exported_dir ), [ os.path.abspath( zip_name ) ] )

    def make_docs( self, boost_directory, temp_dir ):
        boostbook_temp = os.path.join( boost_directory, "bin.v2" )
        tools_directory = os.path.join( temp_dir, "tools" )
        if not os.path.exists( tools_directory ):
            os.makedirs( tools_directory )

        if self.start_step( "win.make_docs.setup_tools", "Setting up BoostBook tools" ):
            sys.path.append( sys.path[0] + "/../boostbook" )
            print sys.path
            import setup_boostbook
            os.environ[ "BOOST_ROOT" ] = boost_directory
            setup_boostbook.setup_boostbook( os.path.join( temp_dir, "tools" ) )

        if self.start_step( "win.make_docs.clean", "Clearing \"bin.v2" ):
            if os.path.exists( boostbook_temp ):
                shutil.rmtree( boostbook_temp )
            self.finish_step( "win.make_docs.clean" )

            
        cd = os.getcwd()
        os.chdir( os.path.join( boost_directory, "doc" )  )

        if self.start_step( "win.make_docs.correct_permissions", "Making html's writable" ):
            utils.checked_system( 
                [ 
                "cd html"
                , "attrib -R *"
                , "cd .."
                ] )
            self.finish_step( "win.make_docs.correct_permissions" )

        def generate( output_format ):
            if self.start_step( "win.make_docs.%s" % output_format, '    Generating %s' % output_format ):
                utils.checked_system( [ 
                    # "set HOME=%s" % my_location
                     "%s -d2 --v2 %s " % ( bjam_path(), output_format )
                    ] )
                self.finish_step( "win.make_docs.%s" % output_format )
        
        generate( "html" )
        generate( "docbook" )
        generate( "fo" )

        if self.start_step( "win.make_docs.copy_docs", "Copying docs into doc directory" ):
            shutil.copy( os.path.join( boostbook_temp, "doc", self.toolset_, "debug", "boost.docbook" ), "boost.docbook" )
            shutil.copy( os.path.join( boostbook_temp, "doc", self.toolset_, "debug", "boost.fo" ), "boost.fo" )
            self.finish_step( "win.make_docs.copy_docs" )


        if self.start_step( "win.make_docs.clean2", "Copying docs into doc directory" ):            
            shutil.rmtree( boostbook_temp )
            shutil.rmtree( "xml" )
            self.finish_step( "win.make_docs.clean2" )

        if self.start_step( "win.make_docs.bb_userman", "Creating Boost.Build user manual" ):
            os.chdir( os.path.join( boost_directory, "tools", "build", "v2", "doc" ) )

            utils.checked_system( [ 
#                    "set HOME=%s" % my_location
                    "%s -d2 --v2 pdf" % bjam_path()
                    ] )

            for f in [ "userman.pdf" ]:
                shutil.copy( find_file( os.path.join( boostbook_temp ), f ), f  )

            shutil.rmtree( boostbook_temp )
            self.finish_step( "win.make_docs.bb_userman" )

        if self.start_step( "win.make_docs.clean3", boost_directory ):
            for i in os.walk( boost_directory ):
                for f in i[2]:
                    full_path = os.path.join( i[0], f )
                    if os.path.splitext( f )[1] in [ ".boostbook" ] \
                        and os.access( full_path, os.W_OK ):
                        os.unlink( full_path )
            self.finish_step( "win.make_docs.clean3" )
                        

    def correct_executable_permissions( self, path ):
        if not self.is_skipping():
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
                        os.system( "chmod a-x %s" % os.path.join( i[0], f ) )

        
    def build_unix( self, release_version, cvs_tag, sf_user, temp_dir, win_build_dir ):

        self.start_step( "unix.export", "Exporting unix copy" )

        temp_unix = self.make_temp_platform( temp_dir, "unix" )
        os.chdir( temp_unix )

        exported_dir = self.cvs_export( sf_user, cvs_tag, release_version, "bash -c \"/usr/bin/%s\"" )
        self.correct_executable_permissions( "." )
        self.finish_step( "unix.export" )

        self.copy_docs_to_unix( os.path.abspath( exported_dir )
                                , win_build_dir )

        if self.start_step( "unix.make_readonly", "Making all files readonly" ):
            utils.checked_system( [ "chmod -R a-w+r,u+w %s" % temp_unix ] )
            utils.checked_system( [ "bash -c /usr/bin/find %s -type d -exec chmod u+w {} ;" % temp_unix ] )
            self.finish_step( "unix.make_readonly" )

        gz_archive_name = "boost_%s" % release_version + ".tar.gz"
        if self.start_step( "unix.gz", "    Making .gz" ):
            if os.path.exists( gz_archive_name  ):  os.unlink( gz_archive_name  )
            os.system( "tar cfz %s %s" % ( gz_archive_name, "boost_%s" % release_version ) )
            self.finish_step( "unix.gz" )

        bz2_archive_name = "boost_%s" % release_version + ".tar.bz2"
        if self.start_step( "unix.bz2", "    Making .bz2" ):
            if os.path.exists( bz2_archive_name  ):  os.unlink( bz2_archive_name )
            os.system( 'bash -c "gunzip -c %s | bzip2 > %s"' % ( gz_archive_name, bz2_archive_name ) )
            self.finish_step( "unix.bz2" )
            
        return [ os.path.abspath( x ) for x in ( gz_archive_name, bz2_archive_name ) ]

    def remove_x_permission( self, directory ):
        for i in os.walk( directory ):
            for f in i[1]:
                os.system( "chmod a=xr,u=rwx %s" % os.path.join( i[0], f ) )
            for f in i[2]:
                os.system( "chmod a=r,u=rw %s" % os.path.join( i[0], f ) )
        
    def copy_docs_to_unix( self, unix_boost_directory, win_boost_directory ):
        if self.start_step( "unix.copy_docs", "Copying docs to unix copy" ):
            doc_directory = os.path.join( unix_boost_directory, "doc" )
            doc_html_directory = os.path.join( doc_directory, "html" )
            remove_directory( doc_html_directory )
            utils.checked_system( [
                "cp -R %s %s " % ( os.path.join( win_boost_directory, "doc", "html" )
                                , doc_html_directory )
                ] )
            for f in [ "boost.docbook", "boost.fo" ]:
                utils.checked_system( [
                    "cp %s %s" % ( os.path.join( win_boost_directory, "doc", f )
                                    , os.path.join( doc_directory, f ) )
                    ] )

            self.remove_x_permission( doc_directory )

            boost_build_doc_directory = os.path.join( unix_boost_directory, "tools", "build", "v2", "doc" )
            boost_build_doc_html_directory = os.path.join( boost_build_doc_directory, "html" )
            
            remove_directory( boost_build_doc_html_directory )
            utils.checked_system( [
                "cp -R %s %s " % ( os.path.join( win_boost_directory, "tools", "build", "v2", "doc", "html" )
                                , boost_build_doc_html_directory ) ] )

            for f in [ "userman.pdf" ]:
                utils.checked_system( [
                    "cp %s %s " % ( os.path.join( win_boost_directory, "tools", "build", "v2", "doc", f )
                                    , os.path.join( boost_build_doc_directory, f ) ) ] )

            self.remove_x_permission( boost_build_doc_directory )
            self.finish_step( "unix.copy_docs" )


def bjam_path():
    if os.path.exists( os.path.join( my_location, "bjam.exe" ) ):
        return os.path.join( my_location, "bjam.exe" )
    else:
        return "bjam.exe"
        
def main():
    ( release_version, cvs_tag, sf_user, toolset, temp_dir, start_step ) = accept_args( sys.argv[ 1: ] )

    make_tarballs( release_version, cvs_tag, sf_user, toolset, temp_dir, start_step  ).run()
    
if __name__ == "__main__":
    main()
