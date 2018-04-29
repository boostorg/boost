# Powershell script to build a docker build environment
#
# Copyright (C) 2018 James E. King III
#
# Use, modification, and distribution are subject to the
# Boost Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#
# Usage: build.ps1 -distro <distro> -version <version> [-group <group>]
# Example: build -distro ubuntu -version bionic
# Result: a local docker image named boost:ubuntu-bionic
#

param
(
    [Parameter(Mandatory=$true)][string]$distro,
    [Parameter(Mandatory=$true)][string]$version,
    [string]$group = "boost"
)

$scriptdir = Split-Path -Parent -Path $MyInvocation.MyCommand.Definition

docker build --build-arg http_proxy --build-arg https_proxy --build-arg ftp_proxy -t ${group}:${distro}-${version} ${scriptdir}\${distro}\${version}
