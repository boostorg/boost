
# Copyright Redshift Software, Inc. 2005-2007
#
# Distributed under the Boost Software License, Version 1.0. 
# (See accompanying file LICENSE_1_0.txt or copy at 
# http://www.boost.org/LICENSE_1_0.txt)

from buildbot.steps.shell import ShellCommand
from buildbot.process.buildstep import LoggedRemoteCommand
import re
import string
import twisted.python

class command_base(ShellCommand):
    def __init__(self, _name, _description, **kwargs):
        if kwargs.get('name'): _name = kwargs.get('name')
        if not kwargs.get('description'): kwargs['description'] = _description
        ShellCommand.__init__(self,**kwargs)
        self._name = _name
    
    def start(self):
        #~ command = self._interpolateProperties(self.command)
        #~ assert isinstance(command, (list, tuple, str))
        kwargs = self.remote_kwargs
        #~ kwargs['command'] = command
        if kwargs.get('env'): kwargs['env'] = kwargs['env'].copy()
        kwargs['logfiles'] = self.logfiles
        cmd = LoggedRemoteCommand(self._name,kwargs)
        self.setupEnvironment(cmd)
        self.checkForOldSlaveAndLogfiles()
        self.startCommand(cmd)

class SelfUpdate(command_base):
    def __init__(self, **kwargs):
        command_base.__init__(self, 'selfupdate', ["self","update"], **kwargs)

class Tarball(command_base):
    def __init__(self, **kwargs):
        command_base.__init__(self, 'tarball', ["tarball"], **kwargs)

class Boost_Jam_Build(command_base):
    def __init__(self, **kwargs):
        command_base.__init__(self, 'boost.jam.build', ["bjam","build"], **kwargs)

class Boost_Jam(command_base):
    def __init__(self, **kwargs):
        command_base.__init__(self, 'boost.jam', ["bjam"], **kwargs)

class Boost_Test(command_base):
    def __init__(self, **kwargs):
        self.tests = kwargs.get('tests');
        if kwargs.has_key('tests'): del kwargs['tests']
        
        self._kwargs = kwargs
        
        command_base.__init__(self, 'boost.jam', ["btest"], **kwargs)
    
    def commandComplete(self, cmd):
        
        def test_match(t,r):
            return t or r.match(parts[1])
        
        #~ Get the log so we can parse it to find all the targets
        #~ we can test.
        out = cmd.log.getText()
        lines = string.split(out,"\n")
        test_targets = {}
        test_re = []
        for test in self.tests:
            test_re.append(re.compile(test))
        for line in lines:
            parts = re.split('(?:" ")|(?:" ")|(?: ")|(?:" )|(?: [[]")|(?:"[]] )|(?:")',line)
            if not parts: continue
            if parts[0] != 'boost-test(TARGET)': continue
            if not reduce(test_match,test_re,False): continue
            try:
                target_i = parts.index(':')+1
            except:
                continue
            twisted.python.log.msg("Boost_Test.commandComplete: TEST = %s -- TARGETS = %s" %
                (parts[1],string.join(parts[target_i:-1],' ')) )
            for t in parts[target_i:-1]:
                test_targets[t] = True
        test_targets = test_targets.keys()
        test_targets.sort()
        
        #~ Construct new steps for each of the targets we want to test. It would be much
        #~ better to tell bjam all targets to test in groups instead of one per invocation.
        #~ But there's no "easy" way to do that. Passing in args can blow the command line
        #~ limits. Setting an env can also blow that limit, but this may be a higher limit
        #~ and we could do them piecemeal.
        kwargs = self._kwargs.copy()
        kwargs.update({
            'flunkOnFailure': False,
            'appendToLog': True
            })
        kwargs['options'].remove('--dump-tests')
        kwargs['options'].remove('--dump-test-targets')
        count = 0
        for test_target in test_targets:
            kwargs['target'] = test_target
            step = Boost_Jam(**kwargs)
            count += 1
            step.name = "%s.%d" % (step.name,count)
            #~ The steps up to our point have been eaten away already. So we
            #~ can add to the front so that the additional steps get executed
            #~ before the rest.
            self.build.steps.insert(count-1,step)
            self.build.build_status.addStep(step)
        #~ Rearrange the steps on the build_status to match the order in the
        #~ actual build.
        existing_count = len(self.build.steps)-count
        new_count = count
        a = self.build.build_status.steps[0:-new_count-existing_count]
        c = self.build.build_status.steps[-new_count-existing_count:-new_count]
        b = self.build.build_status.steps[-new_count:]
        self.build.build_status.steps = a+b+c

class Boost_Process_Jam_Log(command_base):
    def __init__(self, **kwargs):
        command_base.__init__(self, 'boost.process_jam_log', ["process log"], **kwargs)

class Boost_Collect_Results(command_base):
    def __init__(self, **kwargs):
        command_base.__init__(self, 'boost.collect_results', ["collect results"], **kwargs)

class Boost_Publish_Results(command_base):
    def __init__(self, **kwargs):
        command_base.__init__(self, 'boost.publish_results', ["publish results"], **kwargs)
