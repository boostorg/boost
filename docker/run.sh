#!/bin/bash
#
# bash script to run a docker build environment shell
#
# Copyright (C) 2018 James E. King III
#
# Use, modification, and distribution are subject to the
# Boost Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#
# Usage: run.sh <distro> <version> [<group>]
# Example: run ubuntu bionic
# Result: a bash shell in the docker image named boost:ubuntu-bionic
#

distro=$1
version=$2
group=${3:-boost}
scriptdir="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

docker run --cap-add=SYS_PTRACE --privileged --security-opt seccomp=unconfined -v ${scriptdir}/..:/boost -it ${group}:${distro}-${version}
