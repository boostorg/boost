#!/bin/bash
#
# bash script to build a docker build environment
#
# Copyright (C) 2018 James E. King III
#
# Use, modification, and distribution are subject to the
# Boost Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#
# Usage: build.sh <distro> <version> [<group>]
# Example: build ubuntu bionic
# Result: a local docker image named boost:ubuntu-bionic
#

distro=$1
version=$2
group=${3:-boost}
scriptdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

docker build --build-arg http_proxy --build-arg https_proxy --build-arg ftp_proxy -t ${group}:${distro}-${version} ${scriptdir}/${distro}/${version}
