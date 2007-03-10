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


class patchwork_globals:
    
    def __init__(self):
        #~ The set of importers we need to look hook into.
        self.importers = {}

        #~ The packages we are patching into a cohesive single set.
        self.packages = {}

        #~ The packages to search for, in priority order.
        self.packages_to_search = None

_g_ = patchwork_globals()

def _key_and_file_(file_match,file_entry):
    m = re.match(file_match,file_entry)
    if m:
        return [ map(lambda y: int(y), m.groups()), file_entry ]
    else:
        return None

#~ Define a module path, which can be a zip file, and its packages.
def def_modules(dir_and_file,packages):
    #~ print "--- patchwork.def_modules(%s,{...})" % (dir_and_file)
    
    #~ pathDir = os.path.dirname
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
    
    if path.endswith('.zip') and not _g_.importers.has_key(path):
        zip = zipfile.ZipFile(path,'r')
        files = zip.namelist()
        _g_.importers[path] = zipimport.zipimporter(path)
        for package in packages.keys():
            rePackage = re.compile(packages[package])
            for zipPath in files:
                if rePackage.match(zipPath):
                    if not zipPath.endswith('/'):
                        zipPath = os.path.dirname(zipPath)
                    #~ print "--- patchwork.def_modules found zip path %s" % (zipPath)
                    _g_.packages[package] = { 'path' : zipPath, 'importer' : path }
                    break
        _g_.packages_to_search = _g_.packages.keys()
        _g_.packages_to_search.sort()
        _g_.packages_to_search.reverse()
        sys.path.insert(0,path)
    else:
        raise ImportError
    

def _open_(filename, mode = 'r', bufsize = -1):
    #~ print "--- patchwork.open(%s,%s,%d)\n" % (filename,mode,bufsize)
    for importer in _g_.importers.keys():
        if filename.startswith(importer):
            return cStringIO.StringIO(_g_.importers[importer].get_data(filename))
    return __builtin__.open(filename,mode,bufsize)

def _file_(filename, mode = 'r', bufsize = -1):
    #~ print "--- patchwork.file(%s,%s,%d)\n" % (filename,mode,bufsize)
    return _open_(filename,mode,bufsize)

#~ Direct loader of modules, and packages, from other importers.
class patchwork_loader:
    
    def __init__(self,importer,path):
        #~ print "--- patchwork.patchwork_loader.__init__"
        
        self.importer = importer
        self.path = path
    
    def load_module(self,fullname):
        #~ print "--- %s.load_module(self,%s)" % (self,fullname)
        
        source = ""
        source += self.importer.get_data(self.path).replace("\r\n","\n").replace("\r","\n")
        source += "\n\n"
        source += "from boost.patchwork import _open_ as open, _file_ as file\n"
        code = compiler.compile(source,self.path,'exec')
        mod = sys.modules.setdefault(fullname, imp.new_module(fullname))
        mod.__file__ = os.path.join(self.importer.archive,self.path)
        mod.__loader__ = self
        if self.path.endswith("__init__.py"):
            mod.__path__ = [ os.path.join(self.importer.archive,os.path.dirname(self.path)) ]
        exec code in mod.__dict__
        return mod


#~ Python 2.3 style importer that searches through our package patchwork set
#~ and loads according to the location for the package.
class patchwork_importer:
    
    def __init__(self,archivepath):
        #~ print "--- %s.__init__(self,%s)" % (self,archivepath)
        
        found = None
        for importer in _g_.importers.keys():
            if archivepath.startswith(importer):
                found = 1
                break
        
        if not found:
            raise ImportError
    
    def find_module(self,fullname,path=None):
        #~ print "--- %s.find_module(self,%s,%s)" % (self,fullname,path)
        
        loader = None
        for package in _g_.packages_to_search:
            
            if fullname.startswith(package):
                
                package_dirname = package.split('.')
                fullname_base = fullname.split('.')[len(package_dirname):]
                
                importer = _g_.importers[_g_.packages[package]['importer']]
                path_base = os.path.join(_g_.packages[package]['path'],*fullname_base)
                
                if importer._files.has_key(os.path.join(path_base,"__init__")+".py"):
                    #~ Source package.
                    loader = patchwork_loader(importer,
                        os.path.join(path_base,"__init__")+".py")
                elif importer._files.has_key(path_base+".py"):
                    #~ Source module.
                    loader = patchwork_loader(importer,
                        path_base+".py")
                
                if loader:
                    #~ print "--- %s.find_module(self,%s,%s)" % (self,fullname,path)
                    #~ print "---      package = %s" % (package)
                    #~ print "---      %s.path = %s" % (loader,loader.path)
                    break;
        
        return loader

#~ Shove our special importer into the global importer hooks.
sys.path_hooks.insert(0,patchwork_importer)
