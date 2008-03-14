
# Copyright Redshift Software, Inc. 2005-2007
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

import boost.buildbot.step
import buildbot
import buildbot.process.base
import buildbot.process.factory
import buildbot.process.step
import os.path
import re
import string
import time
import twisted.python
import types
import urllib

from buildbot.process.factory import s

def action(_action,*_args,**_kwargs):
    _args = _args or []
    _kwargs = _kwargs or {}
    return (_action,_args,_kwargs)

def defaults(_defaults = {},**_kwargs):
    _defaults.update({
        'haltOnFailure': _kwargs.get('haltOnFailure',False),
        'flunkOnWarnings': _kwargs.get('flunkOnWarnings',False),
        'flunkOnFailure': _kwargs.get('flunkOnFailure',True),
        'warnOnWarnings': _kwargs.get('warnOnWarnings',False),
        'warnOnFailure': _kwargs.get('warnOnFailure',False),
        'timeout': _kwargs.get('timeout',30*60)
        })
    return _defaults

class Boost_BuildFactory(buildbot.process.factory.BuildFactory):
    
    def __init__(self, *actions, **args):
        buildbot.process.factory.BuildFactory.__init__(self)
        self.actions = actions or []
        self.options = args or {}
        #~ --
        self.steps = []
        self.treeStableTimer = 5*60
        self.buildClass = Boost_Build
    
    def newBuild(self):
        b = buildbot.process.factory.BuildFactory.newBuild(self)
        b.setOptions(self.options)
        steps = []
        files = []
        for (_action,_args,_kwargs) in self.actions:
            action_call = getattr(self,'action_%s' % _action,None)
            if callable(action_call):
                for k in _kwargs.keys():
                    if _kwargs[k] == None: del _kwargs[k]
                _kwargs.update(self.options)
                (action_steps,action_files) = action_call(b,*_args,**_kwargs)
                steps = steps + action_steps
                files = files + action_files
        b.important_files = files
        b.setSteps(steps)
        return b
    
    def action_cvs(self,b,*args,**kwargs):
        opt = {
            'cvsmodule'         : kwargs.get('module',"boost"),
            'global_options'    : ["-z9"],
            'mode'              : kwargs.get('mode',"copy"),
            'branch'            : kwargs.get('branch','HEAD'),
            'cvsroot'           : kwargs.get('root')
            }
        if kwargs.has_key('passwd'):
            opt['login'] = kwargs['passwd'] or ""
        opt.update(defaults(**kwargs))
        return (
            [ s(buildbot.process.step.CVS,**opt) ],
            kwargs.get('files',[".*"]) )

    def action_tarball(self,b,*args,**kwargs):
        return (
            [ s( boost.buildbot.step.Tarball
                ,description = kwargs.get('description')
                ,archive = kwargs.get('archive',b.workdir)
                ,publishdir = kwargs['publishdir']
                ,branch = kwargs.get('branch','HEAD')
                ,**defaults(**kwargs)
                ) ],
            kwargs.get('files',[]) )

    def action_selfupdate(self,b,*args,**kwargs):
        return (
            [ s( boost.buildbot.step.SelfUpdate
                ,description = kwargs.get('description')
                ,**defaults(**kwargs)
                ) ],
            kwargs.get('files',[]) )
    
    def action_bjam_build(self,b,*args,**kwargs):
        return (
            [ s( boost.buildbot.step.Boost_Jam_Build
                ,description = kwargs.get('description')
                ,workdir = b.workdir
                ,jam_src = kwargs.get('jam_src','tools/build/jam_src')
                ,toolset = kwargs.get('toolset',None)
                ,**defaults(**kwargs)
                ) ],
            kwargs.get('files',[]) )
    
    def action_bjam(self,b,*args,**kwargs):
        return (
            [ s( boost.buildbot.step.Boost_Jam
                ,description = kwargs.get('description')
                ,workdir = b.workdir
                ,bjam = kwargs.get('bjam','tools/build/jam_src/bin/bjam')
                ,project = kwargs.get('project','.')
                ,options = kwargs.get('options',[])
                ,target = kwargs.get('target','all')
                ,locate = kwargs.get('locate','build')
                ,env = kwargs.get('env',{})
                ,logfile = kwargs.get('logfile',False)
                ,**defaults(**kwargs)
                ) ],
            kwargs.get('files',[]) )
    
    def action_test_tools_build(self,b,*args,**kwargs):
        return self.action_bjam( b
            ,description = kwargs.get('description',['test tools','build'])
            ,project = 'tools/regression/build'
            ,options = [
                '-sBUILD=release',
                '-sTOOLS=%s' % kwargs['toolset']
                ] + kwargs.get('options',[])
            ,target = 'run'
            ,locate = kwargs.get('locate','build')
            ,env = kwargs.get('env',{})
            ,**defaults(**kwargs)
            )

    def action_btest(self,b,*args,**kwargs):
        return (
            [ s( boost.buildbot.step.Boost_Test
                ,description = kwargs.get('description')
                ,workdir = b.workdir
                ,tests = kwargs.get('tests',['.*'])
                ,bjam = kwargs.get('bjam','tools/build/jam_src/bin/bjam')
                ,project = kwargs.get('project','status')
                ,options = kwargs.get('options',[
                    '--dump-tests',
                    '--dump-test-targets',
                    '-sBUILD=%s' % kwargs.get('build','debug'),
                    '-sTOOLS=%s' % kwargs['toolset']
                    ] + kwargs.get('options',[]))
                ,target = 'nothing'
                ,locate = kwargs.get('locate','build')
                ,env = kwargs.get('env',{})
                ,logfile = kwargs.get('logfile','bjam.log')
                ,**defaults(**kwargs)
                ) ],
            kwargs.get('files',[]) )

    def action_btest_all(self,b,*args,**kwargs):
        return self.action_bjam( b
            ,description = kwargs.get('description',['btest','all'])
            ,project = kwargs.get('project','status')
            ,options = [
                '--dump-tests',
                '--dump-test-targets',
                '-sBUILD=%s' % kwargs.get('build','debug'),
                '-sTOOLS=%s' % kwargs['toolset']
                ] + kwargs.get('options',[])
            ,target = 'test'
            ,locate = kwargs.get('locate','build')
            ,env = kwargs.get('env',{})
            ,logfile = kwargs.get('logfile','bjam.log')
            ,files = kwargs.get('files',['boost.*','libs.*','status.*'])
            ,**defaults(**kwargs)
            )

    def action_process_jam_log(self,b,*args,**kwargs):
        return (
            [ s( boost.buildbot.step.Boost_Process_Jam_Log
                ,description = kwargs.get('description',['process log'])
                ,workdir = b.workdir
                ,projcess_jam_log = kwargs.get('projcess_jam_log','tools/regression/build/run/process_jam_log')
                ,locate = kwargs.get('locate','build')
                ,logfile = kwargs.get('logfile','bjam.log')
                ,**defaults(**kwargs)
                ) ],
            kwargs.get('files',[]) )
    
    def action_collect_results(self,b,*args,**kwargs):
        return (
            [ s( boost.buildbot.step.Boost_Collect_Results
                ,description = kwargs.get('description')
                ,workdir = b.workdir
                ,locate = kwargs.get('locate',b.options.get('locate','build'))
                ,runner = kwargs['runner']
                ,branch = kwargs['branch']
                ,source_type = kwargs['source_type']
                ,**defaults(**kwargs)
                ) ],
            kwargs.get('files',[]) )
    
    def action_publish_results(self,b,*args,**kwargs):
        return (
            [ s( boost.buildbot.step.Boost_Publish_Results
                ,description = kwargs.get('description')
                ,workdir = b.workdir
                ,locate = kwargs.get('locate',b.options.get('locate','build'))
                ,runner = kwargs['runner']
                ,branch = kwargs['branch']
                ,source_type = kwargs['source_type']
                ,publish_location = kwargs['publish_location']
                ,proxy = kwargs.get('proxy')
                ,**defaults(**kwargs)
                ) ],
            kwargs.get('files',[]) )

class Boost_Build(buildbot.process.base.Build):
    
    def __init__(self):
        buildbot.process.base.Build.__init__(self)
        self.important_files = []
        self.important_re = None
    
    def isFileImportant(self, filename):
        if self.important_re == None:
            self.important_re = []
            for file in self.important_files:
                self.important_re.append(re.compile(file))
        for file_re in self.important_re:
            if file_re.search(filename):
                return 1;
        return 0
    
    def setOptions(self,options = {}):
        self.options = options or {}
        self.workdir = self.options.get('workdir','build')

    def setupBuild(self, expectations):
        #~ Hack the stamp as an allowed arg for steps.
        if 'stamp' not in buildbot.process.step.BuildStep.parms:
            buildbot.process.step.BuildStep.parms.append('stamp')
        
        return buildbot.process.base.Build.setupBuild(self,expectations)
    
    def getNextStep(self):
        s = buildbot.process.base.Build.getNextStep(self)
        if s:
            #~ Add a stamp arg for the steps to use as needed.
            stamp = self._get_stamp()
            s.stamp = stamp
            if hasattr(s,'cmd'):
                if hasattr(s.cmd,'args'):
                    s.cmd.args.update( { 'stamp' : stamp } )
        return s
    
    def _get_stamp(self):
        #~ The default is to use the revision sequence as the "time".
        #~ If not available, because of a forced build for example, we 
        #~ use the current time.
        stamp = time.strftime( '%Y-%m-%dT%H:%M:%S', time.gmtime() )
        revision, patch = self.getSourceStamp()
        if not revision:
            changes = self.allChanges()
            if changes:
                last_change_time = max([c.when for c in changes])
                last_change_revision = max([c.revision for c in changes])
                #~ Prefer using the revision change if present. If it's not
                #~ it's likely a CVS like time sequence, so use the time sequence
                #~ int that case (adjusted with the tree timer).
                if last_change_revision:
                    stamp = last_change_revision
                else:
                    stamp = time.strftime( '%Y-%m-%dT%H:%M:%S',
                        time.gmtime(last_change_time + self.treeStableTimer / 2) )
        return stamp
