#!/bin/sh
# Copyright (C) 2005, 2006 Douglas Gregor.
# Copyright (C) 2006 The Trustees of Indiana University
# Copyright (C) 2010 Bryce Lelbach
# Copyright 2018-2020 Rene Rivera
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

# Build b2
echo "Building the B2 engine.."
pwd=`pwd`
"${pwd}/src/engine/build.sh" "$@"
if [ $? -ne 0 ]; then
    echo
    echo "Failed to build the B2 engine." 1>&2
    exit 1
fi
cd "$pwd"
cp "./src/engine/b2" .

cat << EOF

Building is done. To install, run:

    ./b2 install --prefix=<DIR>

EOF
