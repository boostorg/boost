# Copyright 2022 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

local library = "system";

local triggers =
{
    branch: [ "master", "develop", "feature/*" ]
};

local ubsan = { UBSAN: '1', UBSAN_OPTIONS: 'print_stacktrace=1' };
local asan = { ASAN: '1' };

local linux_pipeline(name, image, environment, packages = "", sources = [], arch = "amd64") =
{
    name: name,
    kind: "pipeline",
    type: "docker",
    trigger: triggers,
    platform:
    {
        os: "linux",
        arch: arch
    },
    steps:
    [
        {
            name: "everything",
            image: image,
            environment: environment,
            commands:
            [
                'set -e',
                'wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -',
            ] +
            (if sources != [] then [ ('apt-add-repository "' + source + '"') for source in sources ] else []) +
            (if packages != "" then [ 'apt-get update', 'apt-get -y install ' + packages ] else []) +
            [
                'export LIBRARY=' + library,
                './.drone/drone.sh',
            ]
        }
    ]
};

local macos_pipeline(name, environment, xcode_version = "12.2", osx_version = "catalina", arch = "amd64") =
{
    name: name,
    kind: "pipeline",
    type: "exec",
    trigger: triggers,
    platform: {
        "os": "darwin",
        "arch": arch
    },
    node: {
        "os": osx_version
    },
    steps: [
        {
            name: "everything",
            environment: environment + { "DEVELOPER_DIR": "/Applications/Xcode-" + xcode_version + ".app/Contents/Developer" },
            commands:
            [
                'export LIBRARY=' + library,
                './.drone/drone.sh',
            ]
        }
    ]
};

local windows_pipeline(name, image, environment, arch = "amd64") =
{
    name: name,
    kind: "pipeline",
    type: "docker",
    trigger: triggers,
    platform:
    {
        os: "windows",
        arch: arch
    },
    "steps":
    [
        {
            name: "everything",
            image: image,
            environment: environment,
            commands:
            [
                'cmd /C .drone\\\\drone.bat ' + library,
            ]
        }
    ]
};

[
    linux_pipeline(
        "Linux 14.04 GCC 4.4",
        "cppalliance/droneubuntu1404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-4.4', CXXSTD: '98,0x' },
        "g++-4.4",
        [ "ppa:ubuntu-toolchain-r/test" ],
    ),

    linux_pipeline(
        "Linux 14.04 GCC 4.6",
        "cppalliance/droneubuntu1404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-4.6', CXXSTD: '98,0x' },
        "g++-4.6",
        [ "ppa:ubuntu-toolchain-r/test" ],
    ),

    linux_pipeline(
        "Linux 14.04 GCC 4.7",
        "cppalliance/droneubuntu1404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-4.7', CXXSTD: '98,0x' },
        "g++-4.7",
        [ "ppa:ubuntu-toolchain-r/test" ],
    ),

    linux_pipeline(
        "Linux 14.04 GCC 4.8*",
        "cppalliance/droneubuntu1404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++', CXXSTD: '03,11' },
    ),

    linux_pipeline(
        "Linux 14.04 GCC 4.9",
        "cppalliance/droneubuntu1404:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-4.9', CXXSTD: '03,11' },
        "g++-4.9",
        [ "ppa:ubuntu-toolchain-r/test" ],
    ),

    linux_pipeline(
        "Linux 16.04 GCC 5*",
        "cppalliance/droneubuntu1604:1",
        { TOOLSET: 'gcc', COMPILER: 'g++', CXXSTD: '03,11,14' },
    ),

    linux_pipeline(
        "Linux 18.04 GCC 6",
        "cppalliance/droneubuntu1804:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-6', CXXSTD: '03,11,14' },
        "g++-6",
    ),

    linux_pipeline(
        "Linux 18.04 GCC 7* 32/64",
        "cppalliance/droneubuntu1804:1",
        { TOOLSET: 'gcc', COMPILER: 'g++', CXXSTD: '03,11,14,17', ADDRMD: '32,64' },
    ),

    linux_pipeline(
        "Linux 18.04 GCC 8",
        "cppalliance/droneubuntu1804:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-8', CXXSTD: '03,11,14,17' },
        "g++-8",
    ),

    linux_pipeline(
        "Linux 20.04 GCC 9* 32",
        "cppalliance/droneubuntu2004:1",
        { TOOLSET: 'gcc', COMPILER: 'g++', CXXSTD: '03,11,14,17,2a', ADDRMD: '32' },
    ),

    linux_pipeline(
        "Linux 20.04 GCC 9* 64",
        "cppalliance/droneubuntu2004:1",
        { TOOLSET: 'gcc', COMPILER: 'g++', CXXSTD: '03,11,14,17,2a', ADDRMD: '64' },
    ),

    linux_pipeline(
        "Linux 20.04 GCC 9 ARM64 32/64",
        "cppalliance/droneubuntu2004:multiarch",
        { TOOLSET: 'gcc', COMPILER: 'g++', CXXSTD: '03,11,14,17,2a', ADDRMD: '32,64' },
        arch="arm64",
    ),

    linux_pipeline(
        "Linux 20.04 GCC 10 32 ASAN",
        "cppalliance/droneubuntu2004:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-10', CXXSTD: '03,11,14,17,20', ADDRMD: '32' } + asan,
        "g++-10-multilib",
    ),

    linux_pipeline(
        "Linux 20.04 GCC 10 64 ASAN",
        "cppalliance/droneubuntu2004:1",
        { TOOLSET: 'gcc', COMPILER: 'g++-10', CXXSTD: '03,11,14,17,20', ADDRMD: '64' } + asan,
        "g++-10-multilib",
    ),

    linux_pipeline(
        "Linux 20.04 Clang 13",
        "cppalliance/droneubuntu2004:1",
        { TOOLSET: 'clang', COMPILER: 'clang++-13', CXXSTD: '03,11,14,17,20' },
        "clang-13",
        ["deb http://apt.llvm.org/focal/ llvm-toolchain-focal-13 main"],
    ),

    linux_pipeline(
        "Linux 20.04 Clang 14 UBSAN",
        "cppalliance/droneubuntu2004:1",
        { TOOLSET: 'clang', COMPILER: 'clang++-14', CXXSTD: '03,11,14,17,20' } + ubsan,
        "clang-14",
        ["deb http://apt.llvm.org/focal/ llvm-toolchain-focal-14 main"],
    ),

    linux_pipeline(
        "Linux 20.04 Clang 14 ASAN",
        "cppalliance/droneubuntu2004:1",
        { TOOLSET: 'clang', COMPILER: 'clang++-14', CXXSTD: '03,11,14,17,20' } + asan,
        "clang-14",
        ["deb http://apt.llvm.org/focal/ llvm-toolchain-focal-14 main"],
    ),

    macos_pipeline(
        "MacOS 10.15 Xcode 12.2 UBSAN",
        { TOOLSET: 'clang', COMPILER: 'clang++', CXXSTD: '03,11,14,1z' } + ubsan,
    ),

    macos_pipeline(
        "MacOS 10.15 Xcode 12.2 ASAN",
        { TOOLSET: 'clang', COMPILER: 'clang++', CXXSTD: '03,11,14,1z' } + asan,
    ),

    windows_pipeline(
        "Windows VS2015 msvc-14.0",
        "cppalliance/dronevs2015",
        { TOOLSET: 'msvc-14.0', CXXSTD: '14,latest' },
    ),

    windows_pipeline(
        "Windows VS2017 msvc-14.1",
        "cppalliance/dronevs2017",
        { TOOLSET: 'msvc-14.1', CXXSTD: '14,17,latest' },
    ),

    windows_pipeline(
        "Windows VS2019 msvc-14.2",
        "cppalliance/dronevs2019",
        { TOOLSET: 'msvc-14.2', CXXSTD: '14,17,20,latest' },
    ),

    windows_pipeline(
        "Windows VS2022 msvc-14.3",
        "cppalliance/dronevs2022:1",
        { TOOLSET: 'msvc-14.3', CXXSTD: '14,17,20,latest' },
    ),
]
