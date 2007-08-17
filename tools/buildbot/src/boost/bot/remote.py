
# Copyright Redshift Software, Inc. 2005-2007
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import boost.bot.char_translation_table
import ftplib
import platform
import re
import os
import os.path
import shutil
import string
import sys
import tarfile
import urlparse
import xml.sax.saxutils
import zipfile

from buildbot.slave.commands import Command, AbandonChain, ShellCommand
from buildbot.slave.registry import registerSlaveCommand
from twisted.internet import reactor, defer
from twisted.python import failure, log, runtime


_ver = '$Revision: 37651 $'[1+len("Revision: "):-2]

class LoggedShellCommand(ShellCommand):
    
    def __init__(self, builder, command, workdir, **kwargs):
        ShellCommand.__init__(self,builder,command,workdir
            ,environ = kwargs.get('environ',{})
            ,sendStdout = kwargs.get('sendStdout',True)
            ,sendStderr = kwargs.get('sendStderr',True)
            ,sendRC = kwargs.get('sendRC',True)
            ,timeout = kwargs.get('timeout',None)
            ,initialStdin = kwargs.get('stdin',kwargs.get('initialStdin',None))
            ,keepStdinOpen = kwargs.get('keepStdinOpen',False)
            ,keepStdout = kwargs.get('keepStdout',False)
            )
        self.logfile = None
        logfile = kwargs.get('logfile')
        if logfile:
            logdir = os.path.dirname(logfile)
            if not os.path.exists(logdir):
                os.makedirs(logdir)
            if kwargs.get('appendToLog',False) and os.path.exists(logfile):
                self.logfile = file(logfile,"a")
            else:
                self.logfile = file(logfile,"w")
    
    def addStdout(self, data):
        ShellCommand.addStdout(self,data)
        if self.logfile: self.logfile.write(data)
    
    def addStdout(self, data):
        ShellCommand.addStdout(self,data)
        if self.logfile: self.logfile.write(data)
    
    def finished(self, sig, rc):
        if self.logfile: self.logfile.close()
        ShellCommand.finished(self,sig,rc)

def c(callback, *args, **kwargs):
    args = args or []
    kwargs = kwargs or {}
    return (callback,args,kwargs)

class NoOpCommand(Command):

    def start(self):
        return self._start("noop",c(self.doNoOp))

    def doNoOp(self):
        self.stdout("do noop")
        return 0

    def stdout(self, message):
        self.sendStatus({'stdout': message+"\n"})

    def interrupt(self):
        self.interrupted = True

    def _start(self, name, *callbacks):
        d = defer.Deferred()
        self.stdout("starting %s operation" % name)
        self.name = name
        self.command = None
        for call,args,kwargs in callbacks:
            d.addCallbacks(self._do_call,None,[call]+args,kwargs)
            d.addCallback(self._result_check)
        d.addCallbacks(self._success,self._failure)
        reactor.callLater(2,d.callback,0)
        return d

    def _do_call(self, rc, call, *args, **kwargs):
        return call(*args,**kwargs)
    
    def _result_check(self, rc):
        if self.interrupted:
            raise AbandonChain(-1)
        if rc != 0:
            raise AbandonChain(rc)
        return 0

    def _success(self, rc):
        self.sendStatus({'rc': 0})
        return None

    def _failure(self, fail):
        fail.trap(AbandonChain)
        self.sendStatus({'rc': fail.value.args[0]})
        return None

registerSlaveCommand("noop", NoOpCommand, _ver)

class SelfUpdateCommand(NoOpCommand):

    def start(self):
        return self._start("selfupdate",c(self.doUpdateCommandRegistry))
    
    def doUpdateCommandRegistry(self):
        import buildbot.slave.registry
        import buildbot.slave.commands
        import boost.buildbot.remote

        self.stdout("updating command registry")
        reload(buildbot.slave.registry)
        self.stdout("reloading standard commands")
        reload(buildbot.slave.commands)
        self.stdout("reloading boost commands")
        reload(boost.buildbot.remote)
        self.stdout("command registry update complete")

        self.stdout("commands:")
        for name, (factory, version) in buildbot.slave.registry.commandRegistry.items():
            self.stdout("  %s (%s)" % (name,version))

        return 0

registerSlaveCommand("selfupdate", SelfUpdateCommand, _ver)

class TarballCommand(NoOpCommand):

    def start(self):
        stamp = self.args.get('stamp','')
        stamp = stamp.replace(' ','-')
        stamp = stamp.replace(':','_')
        archive_stamped = os.path.normpath(os.path.join(self.builder.basedir,
            "%s-%s-%s" % (self.args['archive'],self.args.get('branch','X'),stamp)))
        return self._start( "tarball",
            c( self.doCleanRepository,
                repository = os.path.normpath(os.path.join(self.builder.basedir, self.args['workdir'])) ),
            c( self.doArchive,
                source = os.path.normpath(os.path.join(self.builder.basedir, self.args['workdir'])),
                archive = archive_stamped ),
            c( self.doPublish,
                archive = archive_stamped,
                publishdir = os.path.normpath(self.args['publishdir']) ) )
    
    def doCleanRepository(self,*args,**kwargs):
        
        self.stdout("cleaning repository at %s..." % kwargs['repository'])
        
        self._clean_r(kwargs['repository'])
        return 0
    
    def doArchive(self,*args,**kwargs):
        source_path = kwargs['source']
        archive_path = "%s.tar.bz2" % kwargs['archive']
        archive_dir = os.path.basename( kwargs['archive'] )
        
        self.stdout("creating archive %s for %s" % ( archive_path, source_path ))
        
        previous_umask = os.umask(0022)
        tar = tarfile.open(archive_path, 'w:bz2')
        #~ Disabling posix allows for longer names and hence deeper directories. 
        tar.Posix = False
        tar.add(source_path, archive_dir)
        tar.close()
        os.umask(previous_umask)
        
        return 0
    
    def doPublish(self,*args,**kwargs):
        archive_path = "%s.tar.bz2" % kwargs['archive']
        
        self.stdout("publishing archive %s to %s" % ( archive_path, kwargs['publishdir'] ))
        
        previous_umask = os.umask(0022)
        try:
            os.makedirs(kwargs['publishdir'],0755)
        except:
            pass
        #~ shutil.move is available on py2.3, consider copy/rename implementation to
        #~ support py2.2. Or possibly do an external async "mv" command.
        shutil.move(archive_path,kwargs['publishdir'])
        self._clean_archives( kwargs['publishdir'], '[^\.]+\.tar\.bz2',
            ( os.path.basename(archive_path) ) )
        os.umask(previous_umask)
        return 0
    
    def _clean_r(self,dir):
        names = os.listdir(dir)
        names.sort()
        for name in names:
            entry = os.path.join(dir,name)
            if name == 'CVS':
                self.stdout("[REMOVE] %s" % entry)
                shutil.rmtree( entry )
            elif os.path.isdir(entry):
                self._clean_r(entry)
    
    def _clean_archives(self,dir,m,exclude):
        m_re = re.compile(m)
        names = os.listdir(dir)
        names.sort()
        for name in names:
            if m_re.search(name) and name not in exclude:
                entry = os.path.join(dir,name)
                self.stdout("[REMOVE] %s" % entry)
                os.remove( entry )

registerSlaveCommand("tarball", TarballCommand, _ver)

class Command_Boost_Jam_Build(NoOpCommand):

    def start(self):
        return self._start( "boost.bjam.build",
            c( self.doBJamBuild,
                jam_src = os.path.normpath(os.path.join(
                    self.builder.basedir, self.args['workdir'], self.args['jam_src'])),
                toolset = self.args.get('toolset',None),
                timeout = self.args.get('timeout',60*5))
            )
    
    def doBJamBuild(self,*args,**kwargs):
        self.stdout("building bjam at %s..." % kwargs['jam_src'])
        if runtime.platformType != 'posix':
            command = [ '.\build.bat' ]
        else:
            command = [ 'sh', './build.sh' ]
        if kwargs['toolset']:
            command.append(kwargs['toolset'])
        self.command = ShellCommand(self.builder, command,
            kwargs['jam_src'], { 'LOCATE_TARGET' : 'bin' },
            sendRC = False, timeout = kwargs['timeout'] )
        return self.command.start()

registerSlaveCommand("boost.jam.build", Command_Boost_Jam_Build, _ver)

class Command_Boost_Jam(NoOpCommand):

    def start(self):
        _builddir = os.path.normpath(os.path.join(
            self.builder.basedir,self.args.get('locate','results')))
        _env = self.args.get('env',{})
        _env.update({
            'BOOST_BUILD_PATH': "%s:%s:%s" % (
                os.path.normpath(self.builder.basedir),
                os.path.normpath(os.path.join(self.builder.basedir,'..')),
                _env.get('BOOST_BUILD_PATH','.') )
            })
        _logfile = False
        if self.args.get('logfile'):
            _logfile = os.path.normpath(os.path.join(
                _builddir,self.args['logfile']))
        return self._start( "boost.bjam",
            c( self.doBJam
                ,bjam = os.path.normpath(os.path.join(self.builder.basedir,
                    self.args['workdir'], self.args['bjam']))
                ,project = os.path.normpath(os.path.join(self.builder.basedir, 
                    self.args['workdir'], self.args.get('project','.')))
                ,options = self.args.get('options',[])
                ,builddir = _builddir
                ,target = self.args.get('target','all')
                ,env = _env
                ,logfile = _logfile
                ,appendToLog = self.args.get('appendToLog',False)
                ,timeout = self.args.get('timeout',60*5)
                )
            )
    
    def doBJam(self,*args,**kwargs):
        self.stdout("bjam %s..." % kwargs['target'])
        self.stdout("  env:")
        env = os.environ.copy()
        env.update(kwargs['env'])
        for item in env.items():
            self.stdout("    %s = '%s'" % item)
        
        command = []
        command += [ kwargs['bjam'] ]
        command += [ '--build-dir=%s' % (kwargs['builddir']) ]
        command += kwargs['options']
        if kwargs.get('target'):
            command += [ kwargs['target'] ]
        self.command = LoggedShellCommand(self.builder
            ,command
            ,kwargs['project']
            ,environ = kwargs['env']
            ,sendRC = False
            ,timeout = kwargs['timeout']
            ,logfile = kwargs['logfile']
            ,appendToLog = kwargs['appendToLog']
            )
        return self.command.start()

registerSlaveCommand("boost.jam", Command_Boost_Jam, _ver)

class Command_Boost_ProcessJamLog(NoOpCommand):
    
    def start(self):
        return self._start( "boost.process_jam_log"
            ,c( self.doProcessJamLog
                ,process_jam_log = os.path.normpath(os.path.join(
                    self.builder.basedir,self.args.get('locate','build'),
                    self.args.get('process_jam_log','dist/bin/process_jam_log')))
                ,boostroot = os.path.normpath(os.path.join(
                    self.builder.basedir,self.args.get('boostroot',self.args.get('workdir','.'))))
                ,logfile = os.path.normpath(os.path.join(
                    self.builder.basedir,self.args.get('locate','build'),
                    self.args.get('logfile','bjam.log')))
                ,locate = os.path.normpath(os.path.join(
                    self.builder.basedir,self.args.get('locate','build')))
                ,timeout = self.args.get('timeout',60*15)
                )
            )
    
    def doProcessJamLog(self,*args,**kwargs):
        self.stdout("processing the regression test results...")
        if runtime.platformType != 'posix':
            command = 'type "%s" | "%s" "%s"' % (kwargs['logfile'], kwargs['process_jam_log'], kwargs['locate'])
        else:
            command = 'cat "%s" | "%s" "%s"' % (kwargs['logfile'], kwargs['process_jam_log'], kwargs['locate'])
        self.command = ShellCommand(self.builder
            ,command
            ,kwargs['boostroot']
            ,timeout = kwargs['timeout']
            )
        return self.command.start()

registerSlaveCommand("boost.process_jam_log", Command_Boost_ProcessJamLog, _ver)

class Command_Boost_CollectResults(NoOpCommand):

    def start(self):
        return self._start( "boost.collect_results",
            c( self.doCollectResults
                ,results = os.path.normpath(os.path.join(
                    self.builder.basedir,self.args.get('locate','build'),
                    '%s.xml' % self.args['runner']))
                ,locate = os.path.normpath(os.path.join(
                    self.builder.basedir,self.args.get('locate','build')))
                ,runner = self.args['runner']
                ,timestamp = string.replace(self.args['stamp'],'T',' ')
                ,tag = '%s-%s' % (self.args['source_type'],self.args['branch'])
                ,source = self.args['source_type']
                ,comments = self.args.get('comments',
                    os.path.normpath(os.path.join(self.builder.basedir,'..','comments.html')))
                ,platform = self.args.get('platform',platform.system())
                ,timeout = self.args.get('timeout',60*15)
                ),
            c( self.doZipArchive
                ,source = os.path.normpath(os.path.join(
                    self.builder.basedir,self.args.get('locate','build'),
                    '%s.xml' % self.args['runner']))
                ,archive = os.path.normpath(os.path.join(
                    self.builder.basedir,self.args.get('locate','build'),
                    '%s.zip' % self.args['runner']))
                ,timeout = self.args.get('timeout',60*15)
                )
            )
    
    def doCollectResults(self,*args,**kwargs):
        self.stdout("collecting the regression test results...")
        result = 0
        previous_umask = os.umask(0022)
        results_writer = open( kwargs['results'], 'w' )
        self.stdout( 'Collecting test logs into "%s"...' % kwargs['results'] )
        
        results_xml = xml.sax.saxutils.XMLGenerator( results_writer )
        results_xml.startDocument()
        results_xml.startElement( 'test-run' ,{
            'tag': kwargs['tag']
            ,'platform': kwargs['platform']
            ,'runner': kwargs['runner']
            ,'timestamp': kwargs['timestamp']
            ,'source': kwargs['source']
            ,'run-type': 'incremental'
            })
        
        self._copy_comments( results_xml, kwargs['comments'] )
        self._collect_test_logs( [ kwargs['locate'] ], results_writer )
    
        results_xml.endElement( "test-run" )
        results_xml.endDocument()
        results_writer.close()
        self.stdout( 'Done writing "%s".' % kwargs['results'] )
        os.umask(previous_umask)
        return result

    def _copy_comments(self,results_xml,comment_file):
        results_xml.startElement( 'comment', {} )
    
        if os.path.exists( comment_file ):
            self.stdout( 'Reading comments file "%s"...' % comment_file )
            f = open( comment_file, 'r' )
            try:
                results_xml.characters( f.read() )
            finally:
                f.close()    
        else:
            self.stdout( 'Warning: comment file "%s" is not found.' % comment_file )
     
        results_xml.endElement( 'comment' )

    def _collect_test_logs(self,input_dirs,test_results_writer):
        self.stdout( 'Collecting test logs ...' )
        for input_dir in input_dirs:
            self.stdout( 'Walking directory "%s" ...' % input_dir )
            os.path.walk( input_dir, self._process_test_log_files, test_results_writer )

    def _process_test_log_files(self,output_file,dir,names):
        for file in names:
            if os.path.basename( file ) == 'test_log.xml':
                self._process_xml_file( os.path.join( dir, file ), output_file )

    def _process_xml_file(self,input_file,output_file):
        self.stdout( 'Processing test log "%s"' % input_file )
        
        f = open( input_file, 'r' )
        xml = f.readlines()
        f.close()
        
        for i in range( 0, len(xml)):
            xml[i] = string.translate( xml[i], boost.buildbot.char_translation_table.char_translation_table )
    
        output_file.writelines( xml )
    
    def doZipArchive(self,*args,**kwargs):
        source_path = kwargs['source']
        archive_path = kwargs['archive']
        self.stdout("creating archive %s for %s" % ( archive_path, source_path ))
        result = 0
        previous_umask = os.umask(0022)
        try:
            z = zipfile.ZipFile( archive_path, 'w', zipfile.ZIP_DEFLATED )
            z.write( source_path, os.path.basename( source_path ) )
            z.close()
            self.stdout( 'Done writing "%s".'% archive_path )
        except Exception, msg:
            self.stdout( 'Warning: Compressing failed (%s)' % msg )
            self.stdout( '         Trying to compress using a platform-specific tool...' )
            try: import zip_cmd
            except ImportError:
                script_dir = os.path.dirname( os.path.abspath( sys.argv[0] ) )
                self.stdout( 'Could not find \'zip_cmd\' module in the script directory (%s).' % script_dir )
                result = -1
            else:
                if os.path.exists( archive_path ):
                    os.unlink( archive_path )
                    self.stdout( 'Removing stale "%s".' % archive_path )
                    
                zip_cmd.main( source_path, archive_path )
                self.stdout( 'Done compressing "%s".' % archive_path )
        os.umask(previous_umask)
        return result

registerSlaveCommand("boost.collect_results", Command_Boost_CollectResults, _ver)

class Command_Boost_PublishResults(NoOpCommand):

    def start(self):
        return self._start( "boost.publish_results",
            c( self.doPublish
                ,source = os.path.normpath(os.path.join(
                    self.builder.basedir,self.args.get('locate','build'),
                    '%s.zip' % self.args['runner']))
                ,target = '%s/%s-%s' % (self.args['publish_location'],self.args['source_type'],self.args['branch'])
                ,proxy = self.args.get('proxy')
                ,timeout = self.args.get('timeout',60*15)
                )
            )
    
    def doPublish(self,*args,**kwargs):
        self.stdout("publishing the regression test results...")
        result = 0
        
        (scheme,site,path,query,fragment) = urlparse.urlsplit(kwargs['target'])
        publish_call = getattr(self,'_publish_%s' % scheme,None)
        if callable(publish_call):
            result = publish_call(scheme,site,path,query,fragment,**kwargs)
        else:
            self.stdout('unknown publish method "%s"' % scheme)
            result = -1
        
        return result

    def _publish_ftp(self,scheme,site,path,query,fragment,**kwargs):
        self.stdout( 'Uploading log archive "%s" to %s' % ( kwargs['source'], kwargs['target'] ) )
        
        if not kwargs['proxy']:
            ftp = ftplib.FTP( site )
            ftp.set_debuglevel( 1 )
            ftp.login()
        else:
            utils.log( '    Connecting through FTP proxy server "%s"' % kwargs['proxy'] )
            ftp = ftplib.FTP( kwargs['proxy'] )
            ftp.set_debuglevel( 1 )
            ftp.set_pasv (0) # turn off PASV mode
            ftp.login( 'anonymous@%s' % site, 'anonymous@' )
        
        ftp.cwd( os.path.dirname(path) )
        try:
            ftp.cwd( os.path.basename(path) )
        except ftplib.error_perm:
            ftp.mkd( os.path.basename(path) )
            ftp.cwd( os.path.basename(path) )
    
        f = open( kwargs['source'], 'rb' )
        ftp.storbinary( 'STOR %s' % os.path.basename( kwargs['source'] ), f )
        ftp.quit()
        return 0

registerSlaveCommand("boost.publish_results", Command_Boost_PublishResults, _ver)
