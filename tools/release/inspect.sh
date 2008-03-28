#!/bin/sh

# Inspect snapshot

# © Copyright 2008 Beman Dawes
# Distributed under the Boost Software License, Version 1.0. See http://www.boost.org/LICENSE_1_0.txt

pushd posix/tools/inspect/build
bjam
popd
echo inspect...
pushd posix
dist/bin/inspect >../inspect.html
popd

# create the ftp script
echo "dir" >inspect.ftp
echo "binary" >>inspect.ftp
echo "put inspect.html" >>inspect.ftp
echo "delete inspect-snapshot.html" >>inspect.ftp
echo "rename inspect.html inspect-snapshot.html" >>inspect.ftp
echo "dir" >>inspect.ftp
echo "bye" >>inspect.ftp
# use cygwin ftp rather than Windows ftp
/usr/bin/ftp -v -i boost.cowic.de <inspect.ftp
