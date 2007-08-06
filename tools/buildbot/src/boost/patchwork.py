#~ Copyright Redshift Software, Inc. 2006-2007
#~ Distributed under the Boost Software License, Version 1.0. 
#~ (See accompanying file LICENSE_1_0.txt or copy at 
#~ http://www.boost.org/LICENSE_1_0.txt)

import __builtin__
import sys
import os
import os.path
import compiler
import imp
import zipimport
import cStringIO
import zipfile
import re
import stat
import calendar


class patchwork_module:
    
    def __init__(self,zip_path):
        self.zip_path = zip_path
        self.zip = zipfile.ZipFile(zip_path,'r')
    
    def execute(self, args, scope = globals(), scripts = None):
        if scripts:
            script = None
            files = self.zip.namelist()
            reScripts = re.compile(scripts)
            for zipPath in files:
                if reScripts.match(zipPath):
                    if not zipPath.endswith('/'):
                        zipPath = os.path.dirname(zipPath)
                    script = zipPath+os.path.basename(args[0])
                    break
            print "Running: %s" % (script)
            exec self.zip.read(script) in scope
        else:
            exec self.zip.read(args[0]) in scope

class patchwork_globals:
    
    def __init__(self):
        #~ The set of importers we need to look hook into.
        self.importers = {}

        #~ The packages we are patching into a cohesive single set.
        self.packages = {}

        #~ The packages to search for, in priority order.
        self.packages_to_search = None
        
        #~ The file info for each file in all the zip archives.
        self.zipinfo = {}

_g_ = patchwork_globals()

#~ Define a module path, which can be a zip file, and its packages.
def def_modules(dir_and_file,packages):
    #~ print "--- patchwork.def_modules(%s,{...})" % (dir_and_file)
    
    def _key_and_file_(file_match,file_entry):
        m = re.match(file_match,file_entry)
        if m:
            return [ map(lambda y: int(y), m.groups()), file_entry ]
        else:
            return None

    dir = filter(
            None,
            map(
                lambda x: _key_and_file_(dir_and_file[1],x),
                os.listdir(dir_and_file[0])
                )
            )
    dir.sort()
    dir.reverse()
    path = os.path.join(dir_and_file[0],dir[0][1])
    print "Using: %s" % (path)
    
    module = None
    if path.endswith('.zip') and not _g_.importers.has_key(path):
        module = patchwork_module(path)
        zip = module.zip
        files = zip.namelist()
        _g_.importers[path] = zipimport.zipimporter(path)
        for zipinfo in zip.infolist():
            _g_.zipinfo[os.path.join(path,zipinfo.filename)] = zipinfo
        for package in packages.keys():
            if os.path.exists(packages[package]):
                #~ print "--| SRC FILE: %s" % (packages[package]);
                _g_.packages[package] = { 'path' : packages[package], 'importer' : path }
            else:
                rePackage = re.compile(packages[package])
                for zipPath in files:
                    if rePackage.match(zipPath):
                        if not zipPath.endswith('/'):
                            zipPath = os.path.dirname(zipPath)
                        #~ print "--- patchwork.def_modules found zip path %s" % (zipPath)
                        #~ print "--| ZIP FILE: %s" % (zipPath);
                        _g_.packages[package] = { 'path' : zipPath, 'importer' : path }
                        break
        _g_.packages_to_search = _g_.packages.keys()
        _g_.packages_to_search.sort()
        _g_.packages_to_search.reverse()
        sys.path.insert(0,path)
    else:
        raise ImportError
    
    return module


def _open_(filename, mode = 'r', bufsize = -1):
    #~ print "--- patchwork.open(%s,%s,%d)\n" % (filename,mode,bufsize)
    for importer in _g_.importers.keys():
        if filename.startswith(importer):
            return cStringIO.StringIO(_g_.importers[importer].get_data(filename))
    return __builtin__.open(filename,mode,bufsize)

def _file_(filename, mode = 'r', bufsize = -1):
    #~ print "--- patchwork.file(%s,%s,%d)\n" % (filename,mode,bufsize)
    return _open_(filename,mode,bufsize)

def _stat_(filename):
    #~ print "--- patchwork.stat(%s)\n" % (filename)
    if filename in _g_.zipinfo:
        st_size = _g_.zipinfo[filename].file_size
        st_mtime = calendar.timegm(_g_.zipinfo[filename].date_time)
        return (
            #~ st_mode
            0100444,
            #~ st_ino
            0,
            #~ st_dev
            0,
            #~ st_nlink
            0,
            #~ st_uid
            0,
            #~ st_gid
            0,
            #~ st_size
            st_size,
            #~ st_atime
            st_mtime,
            #~ st_mtime
            st_mtime,
            #~ st_ctime
            st_mtime
            )
    return os.stat(filename)

#~ Direct loader of modules, and packages, from other importers.
class patchwork_loader:
    
    def __init__(self,importer,module,path):
        #~ print "--- patchwork_loader.__init__(self,importer,\n\t%s,\n\t%s)" % (module,path)
        
        self.importer = importer
        self.module = module
        self.path = path
    
    def find_module(self,fullname,path=None):
        #~ print "--- patchwork_loader.find_module(self,\n\t%s,\n\t%s)" % (fullname,path)
        return self.importer.find_module(fullname,path=path)
    
    def load_module(self,fullname):
        #~ print "--- patchwork_loader.load_module(self,\n\t%s)" % (fullname)
        
        source = ""
        if os.path.exists(self.path):
            #~ print "\tRC FILE: %s" % (self.path);
            source += file(self.path,"rU").read()
        else:
            #~ print "\tZIP FILE: %s" % (self.path);
            source += self.importer.get_data(self.path).replace("\r\n","\n").replace("\r","\n")
        source += '''
import __builtin__
if __builtin__.open == open:
    from boost.patchwork import _open_ as open
if isinstance(file,type):
    from boost.patchwork import _file_ as file
from boost.patchwork import _stat_ as stat
'''
        code = compiler.compile(source,self.path,'exec')
        mod = sys.modules.setdefault(fullname, imp.new_module(fullname))
        mod.__file__ = os.path.join(self.importer.archive,self.path)
        mod.__loader__ = self
        if self.path.endswith("__init__.py"):
            mod.__path__ = [ os.path.join(self.importer.archive,os.path.dirname(self.path)) ]
        exec code in mod.__dict__
        #~ We return the sys.modules entry instead of the mod variable directly
        #~ because it's possible for the module itself to override the sys.modules
        #~ entry with a custom one. For example, this is what Twisted 2.5 does.
        return sys.modules[fullname]


#~ Python 2.3 style importer that searches through our package patchwork set
#~ and loads according to the location for the package.
class patchwork_importer:
    
    def __init__(self,archivepath):
        #~ print "--- patchwork_importer.__init__(self,%s)" % (archivepath)
        
        found = None
        for importer in _g_.importers.keys():
            if archivepath.startswith(importer):
                found = 1
                break
        
        if not found:
            raise ImportError
    
    def find_module(self,fullname,path=None):
        #~ print "--- patchwork_importer.find_module(self,\n\t%s,\n\t%s)" % (fullname,path)
        
        loader = None
        for package in _g_.packages_to_search:
            
            if fullname.startswith(package):
                
                package_dirname = package.split('.')
                fullname_base = fullname.split('.')[len(package_dirname):]
                
                importer = _g_.importers[_g_.packages[package]['importer']]
                if os.path.exists(_g_.packages[package]['path']):
                    path_base = _g_.packages[package]['path']
                else:
                    path_base = os.path.join(_g_.packages[package]['path'],*fullname_base)
                
                if os.path.exists(os.path.join(path_base,"__init__")+".py"):
                    #~ Source package.
                    loader = patchwork_loader(importer,fullname,
                        os.path.join(path_base,"__init__")+".py")
                elif os.path.exists(path_base):
                    #~ Source module.
                    loader = patchwork_loader(importer,fullname,
                        path_base)
                elif os.path.exists(path_base+".py"):
                    #~ Source module.
                    loader = patchwork_loader(importer,fullname,
                        path_base+".py")
                elif importer._files.has_key(os.path.join(path_base,"__init__")+".py"):
                    #~ Source package.
                    loader = patchwork_loader(importer,fullname,
                        os.path.join(path_base,"__init__")+".py")
                elif importer._files.has_key(path_base+".py"):
                    #~ Source module.
                    loader = patchwork_loader(importer,fullname,
                        path_base+".py")
                
                if loader:
                    #~ print "--- patchwork_importer.find_module(self,%s,%s)" % (fullname,path)
                    #~ print "---      package = %s" % (package)
                    #~ print "---      %s.path = %s" % (loader,loader.path)
                    break;
        
        return loader

#~ Shove our special importer into the global importer hooks.
sys.path_hooks.insert(0,patchwork_importer)
