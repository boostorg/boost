#!/usr/bin/python

# Copyright MetaCommunications, Inc. 2003-2007
# Copyright Redshift Software, Inc. 2007
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import glob
import optparse
import os
import os.path
import platform
import sys
import time

#~ Place holder for xsl_reports/util module
utils = None

repo_root = {
    'anon'          : 'http://svn.boost.org/svn/boost/',
    'user'          : 'https://svn.boost.org/svn/boost/'
    }
repo_path = {
    'trunk'         : 'trunk',
    'release'       : 'branches/release',
    'build'         : 'branches/release-tools/build/v2',
    'jam'           : 'branches/release-tools/jam/src',
    'regression'    : 'branches/release-tools/regression'
    }

class runner:
    
    def __init__(self,root):
        commands = "commands: %s" % ', '.join(
            map(
                lambda m: m[8:].replace('_','-'),
                filter(
                    lambda m: m.startswith('command_'),
                    runner.__dict__.keys())
                )
            )
        
        opt = optparse.OptionParser(
            usage="%prog [options] [commands]",
            description=commands)
            
        #~ Base Options:
        opt.add_option( '--runner',
            help="runner ID (e.g. 'Metacomm')" )
        opt.add_option( '--comment',
            help="an HTML comment file to be inserted in the reports" )
        opt.add_option( '--tag',
            help="the tag for the results" )
        opt.add_option( '--toolsets',
            help="comma-separated list of toolsets to test with" )
        opt.add_option( '--incremental',
            help="do incremental run (do not remove previous binaries)",
            action='store_true' )
        opt.add_option( '--timeout',
            help="specifies the timeout, in minutes, for a single test run/compilation",
            type='int' )
        opt.add_option( '--bjam-options',
            help="options to pass to the regression test" )
        opt.add_option( '--bjam-toolset',
            help="bootstrap toolset for 'bjam' executable" )
        opt.add_option( '--pjl-toolset',
            help="bootstrap toolset for 'process_jam_log' executable" )
        opt.add_option( '--platform' )

        #~ Source Options:
        opt.add_option( '--user',
            help="Boost SVN user ID" )
        opt.add_option( '--local',
            help="the name of the boost tarball" )
        opt.add_option( '--force-update',
            help="do an SVN update (if applicable) instead of a clean checkout, even when performing a full run" )
        opt.add_option( '--have-source',
            help="do neither a tarball download nor an SVN update; used primarily for testing script changes" )

        #~ Connection Options:
        opt.add_option( '--proxy',
            help="HTTP proxy server address and port (e.g.'http://www.someproxy.com:3128')" )
        opt.add_option( '--ftp-proxy',
            help="FTP proxy server (e.g. 'ftpproxy')" )
        opt.add_option( '--dart-server',
            help="the dart server to send results to" )

        #~ Debug Options:
        opt.add_option( '--debug-level',
            help="debugging level; controls the amount of debugging output printed",
            type='int' )
        opt.add_option( '--send-bjam-log',
            help="send full bjam log of the regression run",
            action='store_true' )
        opt.add_option( '--mail',
            help="email address to send run notification to" )
        opt.add_option( '--smtp-login',
            help="STMP server address/login information, in the following form: <user>:<password>@<host>[:<port>]" )
        opt.add_option( '--skip-tests',
            help="do not run bjam; used for testing script changes" )
        
        self.comment='comment.html'
        self.tag='trunk'
        self.incremental=False
        self.timeout=5
        self.platform=self.platform_name()
        self.debug_level=0
        self.send_bjam_log=False
        self.bjam_toolset=''
        self.pjl_toolset=''
        ( _opt_, self.actions ) = opt.parse_args(None,self)
        
        self.regression_root = root
        self.boost_root = os.path.join( self.regression_root, 'boost' )
        self.regression_results = os.path.join( self.regression_root, 'results' )
        self.regression_log = os.path.join( self.regression_results, 'bjam.log' )
        self.tools_bb_root = os.path.join( self.regression_root,'tools_bb' )
        self.tools_bjam_root = os.path.join( self.regression_root,'tools_bjam' )
        self.tools_regression_root = os.path.join( self.regression_root,'tools_regression' )
        self.xsl_reports_dir = os.path.join( self.tools_regression_root, 'xsl_reports' )
        self.timestamp_path = os.path.join( self.regression_root, 'timestamp' )
        if sys.platform == 'win32':
            self.patch_boost = 'patch_boost.bat'
            self.bjam = {
                'name' : 'bjam.exe',
                #~ 'build_cmd' : 'build.bat %s' % self.bjam_toolset,
                'path' : os.path.join(self.regression_root,'bjam.exe'),
                'source_dir' : self.tools_bjam_root
                }
            self.process_jam_log = {
                'name' : 'process_jam_log.exe',
                'source_dir' : os.path.join(self.tools_regression_root,'build')
                }
        else:
            self.patch_boost = 'patch_boost'
            self.bjam = {
                'name' : 'bjam',
                #~ 'build_cmd' : './build.sh %s' % self.bjam_toolset,
                'path' : os.path.join(self.regression_root,'bjam'),
                'source_dir' : self.tools_bjam_root
                }
            self.process_jam_log = {
                'name' : 'process_jam_log',
                'source_dir' : os.path.join(self.tools_regression_root,'build')
                }
        
        if self.debug_level > 0:
            self.log('Regression root =     %s'%self.regression_root)
            self.log('Boost root =          %s'%self.boost_root)
            self.log('Regression results =  %s'%self.regression_results)
            self.log('Regression log =      %s'%self.regression_log)
            self.log('BB root =             %s'%self.tools_bb_root)
            self.log('Bjam root =           %s'%self.tools_bjam_root)
            self.log('Tools root =          %s'%self.tools_regression_root)
            self.log('XSL reports dir =     %s'%self.xsl_reports_dir)
            self.log('Timestamp =           %s'%self.timestamp_path)
            self.log('Patch Boost script =  %s'%self.patch_boost)
        
        self.main()
    
    #~ The various commands that make up the testing sequence...
    
    def command_cleanup(self,*args):
        if args == []: args = [ 'source', 'bin' ]

        if 'source' in args:
            self.log( 'Cleaning up "%s" directory ...' % self.boost_root )
            self.rmtree( self.boost_root )

        if 'bin' in args:
            boost_bin_dir = os.path.join( self.boost_root, 'bin' )
            self.log( 'Cleaning up "%s" directory ...' % self.boost_bin_dir )
            self.rmtree( self.boost_bin_dir )

            boost_binv2_dir = os.path.join( self.boost_root, 'bin.v2' )
            self.log( 'Cleaning up "%s" directory ...' % boost_binv2_dir )
            self.rmtree( boost_binv2_dir )

            self.log( 'Cleaning up "%s" directory ...' % self.regression_results )
            self.rmtree( self.regression_results )
    
    def command_get_tools(self):
        #~ Get Boost.Build v2...
        os.chdir( os.path.dirname(self.tools_bb_root) )
        self.svn_command( 'co %s %s' % (
            self.svn_repository_url(repo_path['build']),
            os.path.basename(self.tools_bb_root) ) )
        #~ Get Boost.Jam...
        os.chdir( os.path.dirname(self.tools_bjam_root) )
        self.svn_command( 'co %s %s' % (
            self.svn_repository_url(repo_path['jam']),
            os.path.basename(self.tools_bjam_root) ) )
        #~ Get the regression tools and utilities...
        os.chdir( os.path.dirname(self.tools_regression_root) )
        self.svn_command( 'co %s %s' % (
            self.svn_repository_url(repo_path['regression']),
            os.path.basename(self.tools_regression_root) ) )
    
    def command_get_source(self):
        self.refresh_timestamp()
        self.log( 'Getting sources (%s)...' % self.timestamp() )

        if hasattr(self,'user') and self.user is not None:
            self.retry( self.svn_checkout )
        else:
            self.retry( self.get_tarball )
        pass
    
    def command_update_source(self):
        if hasattr(self,'user') \
            and self.user is not None \
            or os.path.exists( os.path.join( self.boost_root, '.svn' ) ):
            open( self.timestamp_path, 'w' ).close()
            self.log( 'Updating sources from SVN (%s)...' % self.timestamp() )
            self.retry( self.svn_update )
        else:
            self.command_get_source( )
        pass
    
    def command_patch(self):
        self.import_utils()
        patch_boost_path = os.path.join( self.regression_root, self.patch_boost )
        if os.path.exists( self.patch_boost ):
            self.log( 'Found patch file "%s". Executing it.' % self.patch_boost )
            os.chdir( self.regression_root )
            utils.system( [ self.patch_boost ] )
        pass
    
    def command_setup(self):
        pass
    
    def command_install(self):
        pass
    
    def command_test(self):
        pass
    
    def command_collect_logs(self):
        pass
    
    def command_upoad_logs(self):
        pass
    
    def command_regression(self):
        pass

    def command_show_revision(self):
        modified = '$Date$'
        revision = '$Revision$'

        import re
        re_keyword_value = re.compile( r'^\$\w+:\s+(.*)\s+\$$' )
        print '\n\tRevision: %s' % re_keyword_value.match( revision ).group( 1 )
        print '\tLast modified on: %s\n' % re_keyword_value.match( modified ).group( 1 )
    
    #~ Utilities...
    
    def main(self):
        for action in self.actions:
            action_m = "command_"+action.replace('-','_')
            if hasattr(self,action_m):
                getattr(self,action_m)()

    def platform_name(self):
        # See http://article.gmane.org/gmane.comp.lib.boost.testing/933
        if sys.platform == 'win32':
            return 'Windows'
        elif sys.platform == 'cygwin':
            return 'Windows/Cygwin'
        return platform.system()

    def log(self,message):
        sys.stdout.flush()
        sys.stderr.flush()
        sys.stderr.write( '# %s\n' % message )
        sys.stderr.flush()

    def rmtree(self,path):
        if os.path.exists( path ):
            if sys.platform == 'win32':
                os.system( 'del /f /s /q "%s" >nul 2>&1' % path )
                shutil.rmtree( unicode( path ) )
            else:
                os.system( 'rm -f -r "%s"' % path )

    def refresh_timestamp( self ):
        if os.path.exists( self.timestamp_path ):
           os.unlink( self.timestamp_path )
        open( self.timestamp_path, 'w' ).close()

    def timestamp( self ):
        return time.strftime(
            '%Y-%m-%dT%H:%M:%SZ',
            time.gmtime( os.stat( self.timestamp_path ).st_mtime ) )

    def retry( self, f, max_attempts=5, sleep_secs=10 ):
        for attempts in range( max_attempts, -1, -1 ):
            try:
                return f()
            except Exception, msg:
                self.log( '%s failed with message "%s"' % ( f.__name__, msg ) )
                if attempts == 0:
                    self.log( 'Giving up.' )
                    raise

                self.log( 'Retrying (%d more attempts).' % attempts )
                time.sleep( sleep_secs )

    def http_get( self, source_url, destination_file ):
        import urllib
        
        proxies = None
        if hasattr(self,'proxy') and self.proxy is not None:
            proxies = { 'http' : self.proxy }

        src = urllib.urlopen( source_url, proxies = proxies )

        f = open( destination_file, 'wb' )
        while True:
            data = src.read( 16*1024 )
            if len( data ) == 0: break
            f.write( data )

        f.close()
        src.close()

    def import_utils(self):
        global utils
        if utils is None:
            sys.path.append( self.xsl_reports_dir )
            import utils as utils_module
            utils = utils_module

    def build_if_needed( tool, toolset, toolsets ):
        if os.path.exists( tool[ 'path' ] ):
            log( 'Found preinstalled "%s"; will use it.' % tool[ 'path' ] )
            return

        log( 'Preinstalled "%s" is not found; building one...' % tool[ 'path' ] )

        if toolset is None:
            if toolsets is not None:
                toolset = string.split( toolsets, ',' )[0]
                if not tool[ 'is_supported_toolset' ]( toolset ):
                    log( 'Warning: Specified toolset (%s) cannot be used to bootstrap "%s".'\
                         % ( toolset, tool[ 'name' ] ) )

                    toolset = tool[ 'default_toolset' ](v2)
                    log( '         Using default toolset for the platform (%s).' % toolset )
            else:
                toolset = tool[ 'default_toolset' ](v2)
                log( 'Warning: No bootstrap toolset for "%s" was specified.' % tool[ 'name' ] )
                log( '         Using default toolset for the platform (%s).' % toolset )

        if os.path.exists( tool[ 'source_dir' ] ):
            log( 'Found "%s" source directory "%s"' % ( tool[ 'name' ], tool[ 'source_dir' ] ) )
            build_cmd = tool[ 'build_cmd' ]( toolset, v2 )
            log( 'Building "%s" (%s)...' % ( tool[ 'name'], build_cmd ) )
            utils.system( [
                  'cd "%s"' % tool[ 'source_dir' ]
                , build_cmd
                ] )
        else:
            raise 'Could not find "%s" source directory "%s"' % ( tool[ 'name' ], tool[ 'source_dir' ] )

        if not tool.has_key( 'build_path' ):
            tool[ 'build_path' ] = tool_path( tool, v2 )

        if not os.path.exists( tool[ 'build_path' ] ):
            raise 'Failed to find "%s" after build.' % tool[ 'build_path' ]

        log( '%s succesfully built in "%s" location' % ( tool[ 'name' ], tool[ 'build_path' ] ) )
    
    #~ Dowloading source, from SVN...

    def svn_checkout( self ):
        os.chdir( self.regression_root )
        self.svn_command( 'co %s %s' % (self.svn_repository_url(self.tag),'boost') )

    def svn_update( self ):
        os.chdir( self.boost_root )
        self.svn_command( 'update' )

    def svn_command( self, command ):
        svn_anonymous_command_line  = 'svn %(command)s'
        svn_command_line            = 'svn --non-interactive --username=%(user)s %(command)s'
        
        if not hasattr(self,'user') or self.user is None or self.user == 'anonymous':
            cmd = svn_anonymous_command_line % { 'command': command }
        else:
            cmd = svn_command_line % { 'user': self.user, 'command': command }

        self.log( 'Executing SVN command "%s"' % cmd )
        rc = os.system( cmd )
        if rc != 0:
            raise Exception( 'SVN command "%s" failed with code %d' % ( cmd, rc ) )

    def svn_repository_url( self, path ):
        if hasattr(self,'user') and self.user is not None and self.user != 'anonymous':
            return '%s%s' % (repo_root['user'],path)
        else:
            return '%s%s' % (repo_root['anon'],path)
    
    #~ Downloading and extracting source archives, from tarballs or zipballs...
    
    def get_tarball( self, *args ):
        if not args or args == []:
            args = [ 'download', 'unpack' ]

        tarball_path = None

        if hasattr(self,'local') and self.local is not None:
            tarball_path = self.local
        elif 'download' in args:
            tarball_path = self.download_boost_tarball()
        if not tarball_path:
            tarball_path = os.path.join( self.regression_root, self.boost_tarball_url() )

        if 'unpack' in args:
            self.unpack_tarball( tarball_path )
        pass

    def download_boost_tarball( self ):
        tarball_name = self.boost_tarball_name()
        tarball_path = os.path.join( self.regression_root, tarball_name )
        tarball_url = self.boost_tarball_url()

        self.log( 'Downloading "%s" to "%s"...'  % ( tarball_url, os.path.dirname( tarball_path ) ) )

        if os.path.exists( tarball_path ):
            os.unlink( tarball_path )
        self.http_get( tarball_url, tarball_path )

        return tarball_path

    def boost_tarball_name( self ):
        return 'boost-%s.tar.bz2' % self.tag.split( '/' )[-1]

    def boost_tarball_url( self ):
        return 'http://beta.boost.org/development/snapshot.php/%s' % self.tag

    def unpack_tarball( self, tarball_path ):
        self.log( 'Looking for old unpacked archives...' )
        old_boost_dirs = self.find_boost_dirs( )

        for old_boost_dir in old_boost_dirs:
            if old_boost_dir != tarball_path:
                self.log( 'Deleting old directory %s.' % old_boost_dir )
                self.rmtree( old_boost_dir )

        self.log( 'Unpacking boost tarball ("%s")...' % tarball_path )

        tarball_name = os.path.basename( tarball_path )
        extension = tarball_name[ tarball_name.find( '.' ) : ]

        if extension in ( ".tar.gz", ".tar.bz2" ):
            import tarfile
            
            mode = os.path.splitext( extension )[1][1:]
            tar = tarfile.open( tarball_path, 'r:%s' % mode )
            for tarinfo in tar:
                tar.extract( tarinfo, self.regression_root )
                if sys.platform == 'win32' and not tarinfo.isdir():
                    # workaround what appears to be a Win32-specific bug in 'tarfile'
                    # (modification times for extracted files are not set properly)
                    f = os.path.join( self.regression_root, tarinfo.name )
                    os.chmod( f, stat.S_IWRITE )
                    os.utime( f, ( tarinfo.mtime, tarinfo.mtime ) )
            tar.close()
        elif extension in ( ".zip" ):
            import zipfile

            z = zipfile.ZipFile( tarball_path, 'r', zipfile.ZIP_DEFLATED )
            for f in z.infolist():
                destination_file_path = os.path.join( self.regression_root, f.filename )
                if destination_file_path[-1] == "/": # directory
                    if not os.path.exists( destination_file_path  ):
                        os.makedirs( destination_file_path  )
                else: # file
                    result = open( destination_file_path, 'wb' )
                    result.write( z.read( f.filename ) )
                    result.close()
            z.close()
        else:
            raise 'Do not know how to unpack archives with extension \"%s\"' % extension

        boost_dir = self.find_boost_dirs()[0]
        self.log( '    Unpacked into directory "%s"' % boost_dir )

        if os.path.exists( boost_root ):
            self.log( 'Deleting "%s" directory...' % boost_root )
            self.rmtree( boost_root )

        self.log( 'Renaming "%s" into "%s"' % ( boost_dir, boost_root ) )
        os.rename( boost_dir, boost_root )

    def find_boost_dirs( self ):
        return [
            x for x in
                glob.glob( os.path.join( self.regression_root, 'boost[-_]*' ) )
                if os.path.isdir( x )
            ]


