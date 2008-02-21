#!/bin/sh

# Build a branches/release snapshot for Windows, using CRLF line termination

# © Copyright 2008 Beman Dawes
# Distributed under the Boost Software License, Version 1.0. See http://www.boost.org/LICENSE_1_0.txt

rm -r -f windows
svn export --non-interactive --native-eol CRLF http://svn.boost.org/svn/boost/branches/release windows
pushd windows/doc
bjam --v2 >../../windows-bjam.log
popd
rm -r windows/bin.v2
SNAPSHOT_DATE=`eval date +%Y-%m-%d`
echo SNAPSHOT_DATE is $SNAPSHOT_DATE
mv windows boost-windows-$SNAPSHOT_DATE

rm -f windows.zip
zip -r windows.zip boost-windows-$SNAPSHOT_DATE

rm -f windows.7z
7z a -r windows.7z boost-windows-$SNAPSHOT_DATE

mv boost-windows-$SNAPSHOT_DATE windows

# create the ftp script
echo "dir" >windows.ftp
echo "binary" >>windows.ftp

echo "put windows.zip" >>windows.ftp
echo "mdelete boost-windows*.zip" >>windows.ftp
echo "rename windows.zip boost-windows-$SNAPSHOT_DATE.zip" >>windows.ftp

echo "put windows.7z" >>windows.ftp
echo "mdelete boost-windows*.7z" >>windows.ftp
echo "rename windows.7z boost-windows-$SNAPSHOT_DATE.7z" >>windows.ftp

echo "dir" >>windows.ftp
echo "bye" >>windows.ftp
# use cygwin ftp rather than windows ftp
/usr/bin/ftp -v -i boost.cowic.de <windows.ftp
