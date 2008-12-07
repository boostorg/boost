#!/usr/bin/env bash

# Build a branches/release snapshot for Windows, using CRLF line termination

# © Copyright 2008 Beman Dawes
# Distributed under the Boost Software License, Version 1.0.
# See http://www.boost.org/LICENSE_1_0.txt

# This script uses ftp, and thus assumes ~/.netrc contains a machine ... entry

echo "Build a branches/release snapshot for Windows, using CRLF line termination..."

echo "Removing old files..."
rm -r -f windows

echo "Exporting files from subversion..."
svn export --non-interactive --native-eol CRLF http://svn.boost.org/svn/boost/branches/release windows

#echo "Building documentation..."
#pushd windows/doc
#bjam-cygwin --v2 --toolset=gcc &>../../windows-bjam.log
#popd
#
#echo "Copying docs from posix tree..."
#cp --recursive posix/doc/html windows/doc

echo "Cleaning up and renaming..."
#rm -r windows/bin.v2
SNAPSHOT_DATE=`eval date +%Y-%m-%d`
echo SNAPSHOT_DATE is $SNAPSHOT_DATE
mv windows boost-windows-$SNAPSHOT_DATE

#rm -f windows.zip
#zip -r windows.zip boost-windows-$SNAPSHOT_DATE

echo "Building .7z..."
rm -f windows.7z
7z a -r windows.7z boost-windows-$SNAPSHOT_DATE

echo "Reverting name..."
mv boost-windows-$SNAPSHOT_DATE windows

echo "Creating ftp script..."
echo "dir" >windows.ftp
echo "binary" >>windows.ftp

#echo "put windows.zip" >>windows.ftp
#echo "mdelete boost-windows*.zip" >>windows.ftp
#echo "rename windows.zip boost-windows-$SNAPSHOT_DATE.zip" >>windows.ftp

echo "put windows.7z" >>windows.ftp
echo "mdelete boost-windows*.7z" >>windows.ftp
echo "rename windows.7z boost-windows-$SNAPSHOT_DATE.7z" >>windows.ftp
echo "dir" >>windows.ftp
echo "bye" >>windows.ftp

echo "Running ftp script..."
# use cygwin ftp rather than windows ftp
/usr/bin/ftp -v -i boost.cowic.de <windows.ftp

echo "Windows snapshot complete!
