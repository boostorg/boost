#!/usr/bin/python24

# A pre-commit hook to detect case-insensitive filename clashes.
#
# What this script does:
#  - Detects new paths that 'clash' with existing, or other new, paths.
#  - Ignores existings paths that already 'clash'
#  - Exits with an error code, and a diagnostic on stderr, if 'clashes'
#    are detected.
#  - DPG: Checks that there are no file names > 31 characters
#
# How it does it:
#  - Get a list of changed paths.
#  - From that list extract the new paths that represent adds or replaces.
#  - For each new path:
#    - Split the path into a directory and a name.
#    - Get the names of all the entries in the version of the directory
#      within the txn.
#    - Compare the canonical new name with each canonical entry name.
#    - If the canonical names match and the pristine names do not match
#      then we have a 'clash'.
#
# Notes:
#  - All the paths from the Subversion filesystem bindings are encoded
#    in UTF-8 and the separator is '/' on all OS's.
#  - The canonical form determines what constitutes a 'clash', at present
#    a simple 'lower case' is used.  That's probably not identical to the
#    behaviour of Windows or OSX, but it might be good enough.
#  - Hooks get invoked with an empty environment so this script explicitly
#    sets a locale; make sure it is a sensible value.
#  - If used with Apache the 'clash' diagnostic must be ASCII irrespective
#    of the locale, see the 'Force' comment near the end of the script for
#    one way to achieve this.
#
# How to call it:
#
#   On a Unix system put this script in the hooks directory and add this to
#   the pre-commit script:
#
#     $REPOS/hooks/case-insensitive.py "$REPOS" "$TXN" || exit 1
#
#   On a windows machine add this to pre-commit.bat:
#
#     python <path-to-script>\case-insensitive.py %1 %2
#     if errorlevel 1 goto :ERROR
#     exit 0
#     :ERROR
#     echo Error found in commit 1>&2
#     exit 1
#
#   Make sure the python bindings are installed and working on Windows.  The
#   zip file can be downloaded from the Subversion site.  The bindings depend
#   on dll's shipped as part of the Subversion binaries, if the script cannot
#   load the _fs dll it is because it cannot find the other Subversion dll's.
#
# $HeadURL$
# $LastChangedRevision$
# $LastChangedDate$
# $LastChangedBy$

import sys, locale
sys.path.append('/usr/local/subversion/lib/svn-python')
from svn import repos, fs
locale.setlocale(locale.LC_ALL, 'en_GB')

def canonicalize(path):
  return path.decode('utf-8').lower().encode('utf-8')

def get_new_paths(txn_root):
  new_paths = []
  for path, change in fs.paths_changed(txn_root).iteritems():
    if (change.change_kind == fs.path_change_add
        or change.change_kind == fs.path_change_replace):
      new_paths.append(path)
  return new_paths

def split_path(path):
  slash = path.rindex('/')
  if (slash == 0):
    return '/', path[1:]
  return path[:slash], path[slash+1:]

def join_path(dir, name):
  if (dir == '/'):
    return '/' + name
  return dir + '/' + name

def ensure_names(path, names, txn_root):
  if (not names.has_key(path)):
     names[path] = []
     for name, dirent in fs.dir_entries(txn_root, path).iteritems():
       names[path].append([canonicalize(name), name])

names = {}   # map of: key - path, value - list of two element lists of names
clashes = {} # map of: key - path, value - map of: key - path, value - dummy

native = locale.getlocale()[1]
if not native: native = 'ascii'
repos_handle = repos.open(sys.argv[1].decode(native).encode('utf-8'))
fs_handle = repos.fs(repos_handle)
txn_handle = fs.open_txn(fs_handle, sys.argv[2].decode(native).encode('utf-8'))
txn_root = fs.txn_root(txn_handle)

new_paths = get_new_paths(txn_root)
for path in new_paths:
  dir, name = split_path(path)
  canonical = canonicalize(name)
  ensure_names(dir, names, txn_root)
  for name_pair in names[dir]:
    if (name_pair[0] == canonical and name_pair[1] != name):
      canonical_path = join_path(dir, canonical)
      if (not clashes.has_key(canonical_path)):
        clashes[canonical_path] = {}
      clashes[canonical_path][join_path(dir, name)] = True
      clashes[canonical_path][join_path(dir, name_pair[1])] = True
 
if (clashes):
  # native = 'ascii' # Force ASCII output for Apache
  for canonical_path in clashes.iterkeys():
    sys.stderr.write(u'Clash:'.encode(native))
    for path in clashes[canonical_path].iterkeys():
      sys.stderr.write(u' \''.encode(native) +
                       str(path).decode('utf-8').encode(native, 'replace') +
                       u'\''.encode(native))
    sys.stderr.write(u'\n'.encode(native))
  sys.exit(1)

# Check for filenames > 31 characters long
# [disable this test until name length limit dispute is resolved]
# new_paths = get_new_paths(txn_root)
new_paths = []
# [-]
any_names_too_long = False
for path in new_paths:
  dir, name = split_path(path)
  if len(name) > 31:
    if not any_names_too_long:
      sys.stderr.write(u'File names with > 31 characters:\n'.encode(native))
      any_names_too_long = True
    sys.stderr.write(u'  '.encode(native) +
                     str(path).decode('utf-8').encode(native, 'replace'))
    sys.stderr.write(u'\n'.encode(native))

if any_names_too_long:
  sys.exit(1)
