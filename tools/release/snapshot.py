#!/usr/bin/python
#
#	Usage:
#		snapshot.py suffix --server server --user username --pass password -nodocs -noupload
#
#	Our goal is to make four files:
#		boost-posix<suffix>.tar.gz
#		boost-posix<suffix>.tar.bz2
#		boost-windows<suffix>.zip
#		boost-windows<suffix>.7z
#
#	And them we upload them (via ftp) to <server>
#

import os, sys
import shutil
import hashlib
import argparse
import subprocess

# import svn.core
# import svn.client

from ftplib import FTP

kSVNReleaseURL	= "http://svn.boost.org/svn/boost/branches/release"
kDocsFileName	= "boost-release-docs.7z"
kDocsTemp		= "docs_temp"
kDocsFolder		= "docs_temp"
k7zName			= "7za"


def mergetree(src, dst, symlinks=False, ignore=None):
	from shutil import copy2, copystat, Error
	import os

	names = os.listdir(src)
	if ignore is not None:
		ignored_names = ignore(src, names)
	else:
		ignored_names = set()

	try:
		os.makedirs(dst)
	except OSError, exc:
		# XXX - this is pretty ugly
		if "file already exists" in exc[1]:	 # Windows
			pass
		elif "File exists" in exc[1]:		 # Linux
			pass
		else:
			raise

	errors = []
	for name in names:
		if name in ignored_names:
			continue
		srcname = os.path.join(src, name)
		dstname = os.path.join(dst, name)
		try:
			if symlinks and os.path.islink(srcname):
				linkto = os.readlink(srcname)
				os.symlink(linkto, dstname)
			elif os.path.isdir(srcname):
				mergetree(srcname, dstname, symlinks, ignore)
			else:
				copy2(srcname, dstname)
			# XXX What about devices, sockets etc.?
		except (IOError, os.error), why:
			errors.append((srcname, dstname, str(why)))
		# catch the Error from the recursive mergetree so that we can
		# continue with other files
		except Error, err:
			errors.extend(err.args[0])
	try:
		copystat(src, dst)
	except WindowsError:
		# can't copy file access times on Windows
		pass
	except OSError, why:
		errors.extend((src, dst, str(why)))
	if errors:
		raise Error, errors

# svnPool = svn.core.Pool()
# svnCTX  = svn.client.create_context(svnPool)
# svnCTX.auth_baton = svn.core.svn_auth_open([
# 	svn.client.get_simple_provider(),
# 	svn.client.get_username_provider(),
# 	svn.client.get_ssl_server_trust_file_provider(),
# 	svn.client.get_ssl_client_cert_file_provider(),
# 	svn.client.get_ssl_client_cert_pw_file_provider(),
# 	])


def svnExport(client, url, eol, revisionStr, dest):
#	rev = svn.core.svn_opt_revision_t()
#	rev.kind = svn.core.svn_opt_revision_number
#	rev.value.number = revNo
#	svn.client.export2 ( kSVNReleaseURL, dest, rev, True, eol, svnCTX, svnPool )
	command_arr = [ "svn", "export", "--non-interactive", "--native-eol", eol, "-r", revisionStr, url, dest ]
	subprocess.check_output ( command_arr )

# def svnLastRevision(url):
# 	rev = svn.core.svn_opt_revision_t()
# 	rev.kind = svn.core.svn_opt_revision_head
# 	res = []
# 	def receiver(path, raw_data, svnPool, retval=res):
# 		res.append(raw_data)
# 	svn.client.info ( kSVNReleaseURL, rev, rev, receiver, False, svnCTX )
# 	return res[0].rev

def ftp_get_file(ftp, filename, dest_file):
	f = open(dest_file, 'wb')
	ftp.retrbinary( "RETR " + filename, f.write)
	f.close ()
	
def ftp_put_file(ftp, filename):
	ftp.storbinary( "STOR " + filename, open(filename, 'rb'))

def hash_file(filename):
	blockSize = 65536
	hasher = hashlib.md5();
	aFile = open(filename, "rb")
	buf = aFile.read(blockSize)
	while len(buf) > 0:
		hasher.update(buf)
		buf = aFile.read(blockSize)
	aFile.close ()
	return hasher

def print_hash(filename):
	print "%s *%s" % (hash_file(filename).hexdigest(), filename)

def compress_7z (source, dest):
	command_arr = [ k7zName, "a", "-r", dest + ".7z",  source ]
	subprocess.check_output ( command_arr )
#	os.system ( "%s a -r %s.7z %s > /dev/null" % (k7zName, dest, source))
	
def expand_7z (source, dest):
	command_arr = [ k7zName, "x", "-y", "-o" + dest, source ]
	subprocess.check_output ( command_arr )
#	os.system ( "%s x -y -o%s %s > /dev/null"  % (k7zName, dest, source))

def do_it(suffix, releaseRevision, server, username, password, doUpload, doDocs):
	client = None  # pysvn.Client ()
	windowsName = "boost-windows" + suffix
	posixName   = "boost-posix"   + suffix

	# Download the docs - stash them somewhere (docs_temp)
	## Old script "snapshot_download_docs.bat"
	if doDocs:
		ftp = FTP(server)
		ftp.login(username, password)
		ftp_get_file ( ftp, kDocsFileName, kDocsFileName )
		expand_7z (	kDocsFileName, kDocsTemp )
		ftp.quit ();		
		
	# Make Posix folder and export with LF line endings
	svnExport(client, kSVNReleaseURL, "LF", releaseRevision, posixName)
	# Merge in the docs
	if doDocs:
		mergetree ( kDocsTemp, posixName )
		
	# Make Windows folder export with CRLF line endings
	svnExport(client, kSVNReleaseURL, "CRLF", releaseRevision, windowsName)
	# Merge in the docs
	if doDocs:
		mergetree ( kDocsTemp, windowsName )

	# Create tar.gz and tar.bz2 files
	shutil.make_archive (posixName, "bztar", posixName)
	shutil.make_archive (posixName, "gztar", posixName)
	shutil.make_archive (windowsName, "zip", windowsName)
	compress_7z (windowsName, windowsName)
	files = [
		posixName   + ".tar.gz", 
		posixName   + ".tar.bz2", 
		windowsName + ".zip", 
		windowsName + ".7z"
		]
	
	# Create the MD5 checksums; list them for easy checking
	for f in files:
		print_hash ( f );
	
	# Upload the four files
	if doUpload:
		ftp = FTP(server)
		ftp.login(username, password)
		for f in files:
			ftp_put_file(ftp, f );
		ftp.quit ();		

	# Clean up the remains
	shutil.rmtree(posixName)
	shutil.rmtree(windowsName)
	if doDocs:
		shutil.rmtree(kDocsTemp)
		os.remove(kDocsFileName)


# $ date "+%Y-%m-%d"
# --> 2012-07-23	

parser = argparse.ArgumentParser ( description='Build Boost snapshot')

parser.add_argument ('-nodocs',  action='store_true', default=False, dest='nodocs',
                    help='Do not fetch (and merge) documentation into the snapshot')
parser.add_argument ('-noupload',  action='store_true', default=False, dest='noupload',
                    help='Do not upload the snapshots to the server')
parser.add_argument ('-revision',  dest="revision", default="HEAD", 
                    help='svn revision to snapshot (default=HEAD)')
parser.add_argument('--server', dest='server', default="boost.cowic.de", action="store",
					help='ftp server to download docs from and upload snapshots to')
parser.add_argument('--user', dest='username', default="", action="store")
parser.add_argument('--pass', dest='password', default="", action="store")
parser.add_argument('suffix', help='the tag to append to the snapshot name')

results = parser.parse_args()
do_it(results.suffix, results.revision, results.server, results.username, results.password, not results.noupload, not results.nodocs)