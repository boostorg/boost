#!/bin/sh

# Build a branches/release snapshot for Posix, using LF line termination

# © Copyright 2008 Beman Dawes
# Distributed under the Boost Software License, Version 1.0. See http://www.boost.org/LICENSE_1_0.txt

rm -r -f posix
svn export --non-interactive --native-eol LF http://svn.boost.org/svn/boost/branches/release posix
pushd posix/doc
bjam --v2 >../../posix-bjam.log
popd
rm -r posix/bin.v2
SNAPSHOT_DATE=`eval date +%Y-%m-%d`
echo SNAPSHOT_DATE is $SNAPSHOT_DATE
mv posix boost-posix-$SNAPSHOT_DATE
rm -f posix.tar.gz
rm -f posix.tar.bz2
tar cfz posix.tar.gz boost-posix-$SNAPSHOT_DATE
gunzip -c posix.tar.gz | bzip2 >posix.tar.bz2
mv boost-posix-$SNAPSHOT_DATE posix
# create the ftp script
echo "dir" >posix.ftp
echo "binary" >>posix.ftp
echo "put posix.tar.gz" >>posix.ftp
echo "mdelete boost-posix*.gz" >>posix.ftp
echo "rename posix.tar.gz boost-posix-$SNAPSHOT_DATE.tar.gz" >>posix.ftp
echo "put posix.tar.bz2" >>posix.ftp
echo "mdelete boost-posix*.bz2" >>posix.ftp
echo "rename posix.tar.bz2 boost-posix-$SNAPSHOT_DATE.tar.bz2" >>posix.ftp
echo "dir" >>posix.ftp
echo "bye" >>posix.ftp
# use cygwin ftp rather than Windows ftp
/usr/bin/ftp -v -i boost.cowic.de <posix.ftp
