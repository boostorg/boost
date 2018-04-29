# Powershell script to run a docker build environment shell
#
# Copyright (C) 2018 James E. King III
#
# Use, modification, and distribution are subject to the
# Boost Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#
# Usage: run.ps1 -distro <distro> -version <version> [-group <group>]
# Example: run -distro ubuntu -version bionic
# Result: a bash shell in the docker image named boost:ubuntu-bionic
#

param
(
    [Parameter(Mandatory=$true)][string]$distro,
    [Parameter(Mandatory=$true)][string]$version,
    [string]$group = "boost"
)

$scriptdir = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition
$boostroot = Split-Path -Parent -Path $scriptdir

docker run -v ${boostroot}:/boost -it ${group}:${distro}-${version}