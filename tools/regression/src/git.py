#!/usr/bin/python

# Copyright Redshift Software, Inc. 2012-2013
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import os
import sys
import shutil
import stat
import dulwich
import dulwich.index
from dulwich.client import get_transport_and_path
from dulwich.repo import Repo
from dulwich.objects import ZERO_SHA, Tree
from dulwich.file import ensure_dir_exists
from dulwich.index import changes_from_tree, Index, index_entry_from_stat

class RemoteRepo(Repo):
    def __init__(self, root, remote_uri, branch='develop', reset=False):
        if reset:
            if os.path.exists(root):
                shutil.rmtree(root)
        if not os.path.isdir(root):
            dulwich.file.ensure_dir_exists(root)
            controldir = os.path.join(root, ".git")
            os.mkdir(controldir)
            Repo._init_maybe_bare(controldir, False)
        Repo.__init__(self,root)
        self.remote_uri = remote_uri
        self.client, self.host_path = get_transport_and_path(self.remote_uri)
        self.want_branch = branch
        self.want_reset = reset
        print "#  Remote repo %s @ %s => %s"%(self.remote_uri,self._branch_ref(),self.path)
    
    def _branch_ref(self):
        return "refs/heads/"+self.want_branch
    
    def _head_ref(self):
        return 'HEAD'
    
    def _determine_wants(self,refs):
        result = []
        for (ref,sha) in refs.iteritems():
            if sha == ZERO_SHA:
                continue
            if sha in self.object_store:
                continue
            if ref == self._branch_ref():
                result.append(sha)
        return result
    
    def _fetch(self):
        print "#  Fetching repo %s => %s.."%(self.remote_uri,self.path)
        remote_refs = self.client.fetch(
            self.host_path,
            self,
            determine_wants=self._determine_wants,
            progress=sys.stdout.write)
        self[self._branch_ref()] = remote_refs[self._branch_ref()]
    
    def _get_tree(self, sha=None):
        if not sha:
            return self[self._branch_ref()].tree
        else:
            return self[sha].tree
    
    def _get_entry_at_path(self, path, sha=None):
        for entry in self.object_store.iter_tree_contents(self._get_tree(sha)):
            if entry.path == path:
                return entry
        return None
    
    def _init_subrepos(self, sha=None):
        self.subrepo = {}
        gitmodules_entry = self._get_entry_at_path(".gitmodules",sha)
        if gitmodules_entry:
            modules = self.object_store.get_raw(gitmodules_entry.sha)[1].splitlines()
            modules.append(None)
            submodule = None
            submodule_path = None
            submodule_url = None
            for l in modules:
                if None == l or l.startswith("[submodule "):
                    if None != submodule and None != submodule_path and \
                        None != submodule_url:
                        submodule_entry = self._get_entry_at_path(submodule_path)
                        self.subrepo[submodule_path] \
                            = (submodule_entry.sha,
                                RemoteRepo(
                                    os.path.join(self.path,submodule_path),
                                    submodule_url,
                                    branch = self.want_branch,
                                    reset = self.want_reset))
                    if None != l:
                        submodule = l.split("\"")[1]
                        submodule_path = None
                        submodule_url = None
                elif l.strip().startswith("path = "):
                    submodule_path = l.strip().rsplit(" ",1)[-1]
                elif l.strip().startswith("url = "):
                    submodule_url = l.strip().rsplit(" ",1)[-1]
                    if submodule_url.startswith('git:'):
                        submodule_url = submodule_url.replace("git:","https:")
    
    def _checkout_entry(self,index,path,mode,sha):
        full_path = os.path.join(self.path, path)
        checked_out_entry = False
        if stat.S_ISREG(mode):
            if not os.path.exists(os.path.dirname(full_path)):
                os.makedirs(os.path.dirname(full_path))
            f = open(full_path, 'wb')
            try:
                f.write(self.object_store[sha].as_raw_string())
            finally:
                f.close()
            os.chmod(full_path, mode)
            checked_out_entry = True
        elif dulwich.objects.S_ISGITLINK(mode) and path in self.subrepo:
            self.subrepo[path][1].checkout(self.subrepo[path][0])
            checked_out_entry = True
        if checked_out_entry:
            st = os.lstat(full_path)
            index[path] = index_entry_from_stat(st, sha, 0, mode=mode)
            index.write()
    
    def checkout(self, sha=None):
        self._fetch()
        print "#  Checkout repo %s @ %s => %s.."%(self.remote_uri,sha,self.path)
        self._init_subrepos(sha)
        target = self['HEAD']
        if sha:
            target = self[sha]
        target_tree = target.tree
        index = Index(self.index_path())
        for e_path,e_mode,e_sha in index.changes_from_tree(self.object_store, target_tree, want_unchanged=True):
            e_source = (e_path[1],e_mode[1],e_sha[1])
            e_target = (e_path[0],e_mode[0],e_sha[0])
            e_to_checkout = None
            if e_source[0] and e_target[0]:
                if not os.path.exists(os.path.join(self.path, e_target[0])):
                    e_to_checkout = ("A",e_target)
                elif e_source[2] != e_target[2]:
                    e_to_checkout = ("U",e_target)
            elif not e_target[0]:
                print "D  %s"%(os.path.join(self.path, e_source[0]))
                if stat.S_ISREG(e_source[1]):
                    os.unlink(os.path.join(self.path, e_source[0]))
                    del index[e_source[0]]
            else:
                e_to_checkout = ("A",e_target)
            if e_to_checkout:
                print "%s  %s"%(e_to_checkout[0],os.path.join(self.path,e_to_checkout[1][0]))
                self._checkout_entry(index, *e_to_checkout[1])
        self._init_subrepos(sha)
        for subrepo in self.subrepo.values():
            subrepo[1].checkout(subrepo[0])
    
    def status(self, sha=None):
        print "#  Status.."
        target = self['HEAD']
        if sha:
            target = self[sha]
        target_tree = target.tree
        index = Index(self.index_path())
        for e_path,e_mode,e_sha in index.changes_from_tree(self.object_store, target_tree, want_unchanged=True):
            e_source = (e_path[0],e_mode[0],e_sha[0])
            e_target = (e_path[1],e_mode[1],e_sha[1])
            if e_source[0] and e_target[0]:
                if not os.path.exists(os.path.join(self.path, e_source[0])):
                    print "D  %s"%(os.path.join(self.path, e_source[0]))
                elif e_source[2] != e_target[2]:
                    print "M  %s"%(os.path.join(self.path, e_source[0]))
            elif not e_target[0]:
                print "D  %s"%(os.path.join(self.path, e_source[0]))
            else:
                print "A  %s"%(os.path.join(self.path, e_target[0]))

if __name__ == '__main__':
    if True:
        g = RemoteRepo(
            'git_test_boost',
            'https://github.com/boostorg/boost.git',
            branch='master',
            reset=True)
        g.checkout()
        g.status()
    if False:
        g = RemoteRepo(
            'git_test_predef',
            'https://github.com/grafikrobot/boost-predef.git',
            branch='master',
            reset=True)
        g.checkout()
        g.status()
