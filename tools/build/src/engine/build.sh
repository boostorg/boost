#!/bin/sh

#~ Copyright 2002-2020 Rene Rivera.
#~ Distributed under the Boost Software License, Version 1.0.
#~ (See accompanying file LICENSE_1_0.txt or copy at
#~ http://www.boost.org/LICENSE_1_0.txt)

FALSE=1
TRUE=0

# Reset the toolset.
B2_TOOLSET=
B2_SETUP=

# Internal options.
B2_VERBOSE_OPT=${B2_VERBOSE_OPT:=${FALSE}}
B2_DEBUG_OPT=${B2_DEBUG_OPT:=${FALSE}}
B2_GUESS_TOOLSET_OPT=${FALSE}
B2_HELP_OPT=${FALSE}
B2_CXX_OPT=
B2_CXXFLAGS_OPT=

# We need to calculate and set SCRIPT_PATH and SCRIPT_DIR to reference this
# script so that we can refer to file relative to it.
SCRIPT_PATH=""
if test "${BASH_SOURCE}" ; then
    SCRIPT_PATH=${BASH_SOURCE}
fi
if test "${SCRIPT_PATH}" = "" ; then
    SCRIPT_PATH=$0
fi
SCRIPT_DIR="$( cd "$( dirname "${SCRIPT_PATH}" )" && pwd )"

# This script needs to operate at engine source directory.
SAVED_PWD="${PWD}"
cd "${SCRIPT_DIR}"

test_true ()
{
    if test $1 -eq ${TRUE} ; then
        return ${TRUE}
    fi
    return ${FALSE}
}

# Run a command, and echo before doing so. Also checks the exit status and quits
# if there was an error.
echo_run ()
{
    if test_true ${B2_VERBOSE_OPT} ; then echo "> $@" ; fi
    $@
    r=$?
    if test $r -ne ${TRUE} ; then
        exit $r
    fi
}

# Print an error message, and exit with a status of 1.
error_exit ()
{
    echo "
${@}

You can specify the toolset as the argument, i.e.:
    ./build.sh [options] gcc

Toolsets supported by this script are:
    acc, clang, como, gcc, intel-darwin, intel-linux, kcc, kylix, mipspro,
    pathscale, pgi, qcc, sun, sunpro, tru64cxx, vacpp

For any toolset you can override the path to the compiler with the '--cxx'
option. You can also use additional flags for the compiler with the
'--cxxflags' option.

A special toolset; cxx, is available which is used as a fallback when a more
specific toolset is not found and the cxx command is detected. The 'cxx'
toolset will use the '--cxx' and '--cxxflags' options, if present.

Options:
    --help                  Show this help message.
    --verbose               Show messages about what this script is doing.
    --debug                 Build b2 with debug information, and no
                            optimizations.
    --guess-toolset         Print the toolset we can detect for building.
    --cxx=CXX               The compiler exec to use instead of the detected
                            compiler exec.
    --cxxflags=CXXFLAGS     The compiler flags to use in addition to the
                            flags for the detected compiler.

" 1>&2
    exit 1
}

# Check that a command is in the PATH.
test_path ()
{
    if `command -v command 1>/dev/null 2>/dev/null`; then
        command -v $1 1>/dev/null 2>/dev/null
    else
        hash $1 1>/dev/null 2>/dev/null
    fi
}

# Check that the OS name, as returned by "uname", is as given.
test_uname ()
{
    if test_path uname; then
        test `uname` = $*
    fi
}

test_compiler ()
{
    EXE="${B2_CXX_OPT:-$1}"
    shift
    CMD="${EXE} $@ ${B2_CXXFLAGS_OPT:-}"
    SETUP=${B2_SETUP:-true}
    if test_true ${B2_VERBOSE_OPT} ; then
        echo "> ${CMD} check_cxx11.cpp"
        ( ${SETUP} ; ${CMD} check_clib.cpp check_cxx11.cpp )
    else
        ( ${SETUP} ; ${CMD} check_clib.cpp check_cxx11.cpp ) 1>/dev/null 2>/dev/null
    fi
    CHECK_RESULT=$?
    if test_true ${CHECK_RESULT} ; then
        B2_CXX=${CMD}
    fi
    rm -rf check_cxx11.o* a.out a.exe 1>/dev/null 2>/dev/null
    return ${CHECK_RESULT}
}

test_toolset ()
{
    if test "${TOOLSET}" = "" ; then return ${TRUE} ; fi
    if test "${TOOLSET}" = "$1" -o "${TOOLSET}" = "$2" -o "${TOOLSET}" = "$3" ; then return ${TRUE} ; fi
    return 1
}

# Check the toolset to bootstrap the build with. The one optional argument to
# the function is a toolset name. This operates as follows based on these
# contextual vars, if set, and if an arg is given:
#
# No vars set:
#   Checks, in some priority order, possible toolset commands. Upon finding the
#   first working command sets B2_TOOLSET to the toolset and B2_CXX to the
#   compile command with any base options.
#
# B2_TOOLSET set:
#   Checks that toolset for possible compile commands and sets B2_CXX to the
#   command that works for the toolset.
#
check_toolset ()
{
    TOOLSET=${B2_TOOLSET%%-[0-9]*}
    TOOLSET_SUFFIX=${B2_TOOLSET##$TOOLSET}

    # Prefer Clang (clang) on macOS..
    if test_toolset clang && test_uname Darwin && test_compiler clang++$TOOLSET_SUFFIX -x c++ -std=c++11 ; then B2_TOOLSET=clang$TOOLSET_SUFFIX ; return ${TRUE} ; fi
    # GCC (gcc)..
    if test_toolset gcc && test_compiler g++$TOOLSET_SUFFIX -x c++ -std=c++11 ; then B2_TOOLSET=gcc$TOOLSET_SUFFIX ; return ${TRUE} ; fi
    if test_toolset gcc && test_compiler g++$TOOLSET_SUFFIX -x c++ -std=c++11 -D_GNU_SOURCE ; then B2_TOOLSET=gcc$TOOLSET_SUFFIX ; return ${TRUE} ; fi
    # GCC (gcc) with -pthread arg (for AIX)..
    if test_toolset gcc && test_compiler g++$TOOLSET_SUFFIX -x c++ -std=c++11 -pthread ; then B2_TOOLSET=gcc$TOOLSET_SUFFIX ; return ${TRUE} ; fi
    # Clang (clang)..
    if test_toolset clang && test_compiler clang++$TOOLSET_SUFFIX -x c++ -std=c++11 ; then B2_TOOLSET=clang$TOOLSET_SUFFIX ; return ${TRUE} ; fi
    # Intel macOS (intel-darwin)
    if test_toolset intel-darwin && test -r "${HOME}/intel/oneapi/setvars.sh" && test_uname Darwin ; then
        B2_SETUP="source ${HOME}/intel/oneapi/setvars.sh"
        if test_toolset intel-darwin && test_compiler icpx -x c++ -std=c++11 ; then B2_TOOLSET=intel-darwin ; return ${TRUE} ; fi
        if test_toolset intel-darwin && test_compiler icc -x c++ -std=c++11 ; then B2_TOOLSET=intel-darwin ; return ${TRUE} ; fi
        B2_SETUP=
    fi
    if test_toolset intel-darwin && test -r "/opt/intel/oneapi/setvars.sh" && test_uname Darwin ; then
        B2_SETUP="source /opt/intel/oneapi/setvars.sh"
        if test_toolset intel-darwin && test_compiler icpx -x c++ -std=c++11 ; then B2_TOOLSET=intel-darwin ; return ${TRUE} ; fi
        if test_toolset intel-darwin && test_compiler icc -x c++ -std=c++11 ; then B2_TOOLSET=intel-darwin ; return ${TRUE} ; fi
        B2_SETUP=
    fi
    # Intel oneAPI (intel-linux)
    if test_toolset intel-linux && test_path icpx ; then
        if test_compiler icpx -x c++ -std=c++11 ; then B2_TOOLSET=intel-linux ; return ${TRUE} ; fi
    fi
    if test_toolset intel-linux && test_path icc ; then
        if test_compiler icc -x c++ -std=c++11 ; then B2_TOOLSET=intel-linux ; return ${TRUE} ; fi
    fi
    if test_toolset intel-linux && test -r "${HOME}/intel/oneapi/setvars.sh" ; then
        B2_SETUP="source ${HOME}/intel/oneapi/setvars.sh"
        if test_toolset intel-linux && test_compiler icpx -x c++ -std=c++11 ; then B2_TOOLSET=intel-linux ; return ${TRUE} ; fi
        if test_toolset intel-linux && test_compiler icc -x c++ -std=c++11 ; then B2_TOOLSET=intel-linux ; return ${TRUE} ; fi
        B2_SETUP=
    fi
    if test_toolset intel-linux && test -r "/opt/intel/oneapi/setvars.sh" ; then
        B2_SETUP="source /opt/intel/oneapi/setvars.sh"
        if test_toolset intel-linux && test_compiler icpx -x c++ -std=c++11 ; then B2_TOOLSET=intel-linux ; return ${TRUE} ; fi
        if test_toolset intel-linux && test_compiler icc -x c++ -std=c++11 ; then B2_TOOLSET=intel-linux ; return ${TRUE} ; fi
        B2_SETUP=
    fi
    # Intel Pro (intel-linux)
    if test_toolset intel-linux && test_path icpc ; then
        if test_compiler icpc -x c++ -std=c++11 ; then B2_TOOLSET=intel-linux ; return ${TRUE} ; fi
    fi
    if test_toolset intel-linux && test -r "/opt/intel/inteloneapi/setvars.sh" ; then
        B2_SETUP="source /opt/intel/inteloneapi/setvars.sh"
        if test_compiler icpc -x c++ -std=c++11 ; then B2_TOOLSET=intel-linux ; return ${TRUE} ; fi
        B2_SETUP=
    fi
    if test_toolset intel-linux && test -r "/opt/intel/cc/9.0/bin/iccvars.sh" ; then
        B2_SETUP="source /opt/intel/cc/9.0/bin/iccvars.sh"
        if test_compiler icpc -x c++ -std=c++11 ; then B2_TOOLSET=intel-linux ; return ${TRUE} ; fi
        B2_SETUP=
    fi
    if test_toolset intel-linux && test -r "/opt/intel_cc_80/bin/iccvars.sh" ; then
        B2_SETUP="source /opt/intel_cc_80/bin/iccvars.sh"
        if test_compiler icpc -x c++ -std=c++11 ; then B2_TOOLSET=intel-linux ; return ${TRUE} ; fi
        B2_SETUP=
    fi
    # Mips Pro (mipspro)
    if test_toolset mipspro && test_uname IRIX && test_compiler CC -FE:template_in_elf_section -ptused ; then B2_TOOLSET=mipspro ; return ${TRUE} ; fi
    if test_toolset mipspro && test_uname IRIX64 && test_compiler CC -FE:template_in_elf_section -ptused ; then B2_TOOLSET=mipspro ; return ${TRUE} ; fi
    # OSF Tru64 C++ (tru64cxx)
    if test_toolset tru64cxx && test_uname OSF1 && test_compiler cc ; then B2_TOOLSET=mipspro ; return ${TRUE} ; fi
    # QNX (qcc)
    if test_toolset qcc && test_uname QNX && test_compiler QCC ; then B2_TOOLSET=qcc ; return ${TRUE} ; fi
    # Linux XL/VA C++ (xlcpp, vacpp)
    if test_toolset xlcpp vacpp && test_uname Linux && test_compiler xlC_r ; then
        if /usr/bin/lscpu | grep Byte | grep Little > /dev/null 2>&1 ; then
            # Little endian linux
            B2_TOOLSET=xlcpp
            return ${TRUE}
        else
            # Big endian linux
            B2_TOOLSET=vacpp
            return ${TRUE}
        fi
    fi
    # AIX VA C++ (vacpp)
    if test_toolset vacpp && test_uname AIX && test_compiler xlC_r ; then B2_TOOLSET=vacpp ; return ${TRUE} ; fi
    # PGI (pgi)
    if test_toolset pgi && test_compiler pgc++ -std=c++11 ; then B2_TOOLSET=pgi ; return ${TRUE} ; fi
    # Pathscale C++ (pathscale)
    if test_toolset pathscale && test_compiler pathCC ; then B2_TOOLSET=pathscale ; return ${TRUE} ; fi
    # Como (como)
    if test_toolset como && test_compiler como ; then B2_TOOLSET=como ; return ${TRUE} ; fi
    # Borland C++ (kylix)
    if test_toolset kylix && test_compiler bc++ -tC -q ; then B2_TOOLSET=kylix ; return ${TRUE} ; fi
    # aCC (acc)
    if test_toolset acc && test_compiler aCC -AA ; then B2_TOOLSET=acc ; return ${TRUE} ; fi
    # Sun Pro C++ (sunpro)
    if test_toolset sunpro && test_compiler /opt/SUNWspro/bin/CC -std=c++11 ; then B2_TOOLSET=sunpro ; return ${TRUE} ; fi
    # Generic (cxx)
    if test_toolset cxx && test_compiler cxx ; then B2_TOOLSET=cxx ; return ${TRUE} ; fi
    if test_toolset cxx && test_compiler cpp ; then B2_TOOLSET=cxx ; return ${TRUE} ; fi
    if test_toolset cxx && test_compiler CC ; then B2_TOOLSET=cxx ; return ${TRUE} ; fi

    # Nothing found.
    if test "${B2_TOOLSET}" = "" ; then
        error_exit "Could not find a suitable toolset."
    fi
    return ${FALSE}
}

# Handle command options and args.
while test $# -gt 0
do
    case "$1" in
        --verbose) B2_VERBOSE_OPT=${TRUE} ;;
        --debug) B2_DEBUG_OPT=${TRUE} ;;
        --guess-toolset) B2_GUESS_TOOLSET_OPT=${TRUE} ;;
        --help) B2_HELP_OPT=${TRUE} ;;
        --cxx=*) B2_CXX_OPT=`expr "x$1" : "x--cxx=\(.*\)"` ;;
        --cxxflags=*) B2_CXXFLAGS_OPT=`expr "x$1" : "x--cxxflags=\(.*\)"` ;;
        -*) ;;
        ?*) B2_TOOLSET=$1 ;;
    esac
    shift
done

# Show some help, if requested.
if test_true ${B2_HELP_OPT} ; then
    error_exit
fi

# If we have a CXX but no B2_TOOLSET specified by the user we assume they meant
# "cxx" as the toolset.
if test "${B2_CXX_OPT}" != "" -a "${B2_TOOLSET}" = "" ; then
    B2_TOOLSET=cxx
fi

# If we have B2_TOOLSET=cxx but no B2_CXX_OPT nor B2_CXXFLAGS_OPT specified by the user
# we assume they meant $CXX and $CXXFLAGS.
if test "${B2_TOOLSET}" = "cxx" -a "${B2_CXX_OPT}" = "" -a "${B2_CXXFLAGS_OPT}" = "" ; then
    B2_CXX_OPT="${CXX}"
    B2_CXXFLAGS_OPT="${CXXFLAGS}"
fi

# Guess toolset, or toolset commands.
check_toolset
TOOLSET_CHECK=$?

# We can bail from the rest of the checks and build if we are just guessing
# the toolset.
if test_true ${B2_GUESS_TOOLSET_OPT} ; then
    echo "${B2_TOOLSET}"
    exit 0
fi

# We need a viable compiler. Check here and give some feedback about it.
if ! test_true ${TOOLSET_CHECK} ; then
    echo "
A C++11 capable compiler is required for building the B2 engine.
Toolset '${B2_TOOLSET}' does not appear to support C++11.
"
    (B2_VERBOSE_OPT=${TRUE} check_toolset)
    error_exit "
** Note, the C++11 capable compiler is _only_ required for building the B2
** engine. The B2 build system allows for using any C++ level and any other
** supported language and resource in your projects.
"
fi

# Set the additional options needed to build the engine based on the toolset.
case "${B2_TOOLSET}" in

    gcc|gcc-*)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:---version}
        B2_CXXFLAGS_RELEASE="-O2 -s"
        B2_CXXFLAGS_DEBUG="-O0 -g"
    ;;

    intel-*)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:---version}
        B2_CXXFLAGS_RELEASE="-O3 -static-intel"
        B2_CXXFLAGS_DEBUG="-O0 -g -static-intel"
    ;;

    vacpp)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:--qversion}
        B2_CXXFLAGS_RELEASE="-O3 -s -qstrict -qinline"
        B2_CXXFLAGS_DEBUG="-g -qNOOPTimize -qnoinline -pg"
    ;;

    xlcpp)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:--qversion}
        B2_CXXFLAGS_RELEASE="-s -O3 -qstrict -qinline"
        B2_CXXFLAGS_DEBUG="-g -qNOOPTimize -qnoinline -pg"
    ;;

    como)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:---version}
        B2_CXXFLAGS_RELEASE="-O3 --inlining"
        B2_CXXFLAGS_DEBUG="-O0 -g --no_inlining --long_long"
    ;;

    kcc)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:---version}
        B2_CXXFLAGS_RELEASE="+K2 -s"
        B2_CXXFLAGS_DEBUG="+K0 -g"
    ;;

    kylix)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:---version}
        B2_CXXFLAGS_RELEASE="-O2 -vi -w-inl -s"
        B2_CXXFLAGS_DEBUG="-Od -v -vi-"
    ;;

    mipspro)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:---version}
        B2_CXXFLAGS_RELEASE="-Ofast -g0 \"-INLINE:none\" -s"
        B2_CXXFLAGS_DEBUG="-O0 -INLINE -g"
    ;;

    pathscale)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:---version}
        B2_CXXFLAGS_RELEASE="-O3 -inline -s"
        B2_CXXFLAGS_DEBUG="-O0 -noinline -ggdb"
    ;;

    pgi)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:---version}
        B2_CXXFLAGS_RELEASE="-fast -s"
        B2_CXXFLAGS_DEBUG="-O0 -gopt"
    ;;

    sun*)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:--V}
        B2_CXXFLAGS_RELEASE="-xO4 -s"
        B2_CXXFLAGS_DEBUG="-g"
    ;;

    clang|clang-*)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:---version}
        B2_CXXFLAGS_RELEASE="-O3 -s"
        B2_CXXFLAGS_DEBUG="-O0 -fno-inline -g"
    ;;

    tru64cxx)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:---version}
        B2_CXXFLAGS_RELEASE="-O5 -inline speed -s"
        B2_CXXFLAGS_DEBUG="-O0 -pg -g"
    ;;

    acc)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:---version}
        B2_CXXFLAGS_RELEASE="-O3 -s"
        B2_CXXFLAGS_DEBUG="+d -g"
    ;;

    qcc)
        CXX_VERSION_OPT=""
        B2_CXXFLAGS_RELEASE="-O3 -Wc,-finline-functions"
        B2_CXXFLAGS_DEBUG="O0 -Wc,-fno-inline -gstabs+"
    ;;

    cxx)
        CXX_VERSION_OPT=${CXX_VERSION_OPT:---version}
    ;;

    *)
        error_exit "Unknown toolset: ${B2_TOOLSET}"
    ;;
esac

build_b2 ()
{
    echo "
###
###
### Using '${B2_TOOLSET}' toolset.
###
###
"
    if test "${CXX_VERSION_OPT}" != ""; then
        echo_run ${B2_CXX} ${CXX_VERSION_OPT}
    fi
echo "
###
###
"
    B2_SOURCES="\
builtins.cpp \
class.cpp \
command.cpp \
compile.cpp \
constants.cpp \
cwd.cpp \
debug.cpp \
debugger.cpp \
execcmd.cpp \
execnt.cpp \
execunix.cpp \
filesys.cpp \
filent.cpp \
fileunix.cpp \
frames.cpp \
function.cpp \
glob.cpp \
hash.cpp \
hcache.cpp \
hdrmacro.cpp \
headers.cpp \
jam_strings.cpp \
jam.cpp \
jamgram.cpp \
lists.cpp \
make.cpp \
make1.cpp \
md5.cpp \
mem.cpp \
modules.cpp \
native.cpp \
object.cpp \
option.cpp \
output.cpp \
parse.cpp \
pathnt.cpp \
pathsys.cpp \
pathunix.cpp \
regexp.cpp \
rules.cpp \
scan.cpp \
search.cpp \
startup.cpp \
subst.cpp \
sysinfo.cpp \
timestamp.cpp \
variable.cpp \
w32_getreg.cpp \
modules/order.cpp \
modules/path.cpp \
modules/property-set.cpp \
modules/regex.cpp \
modules/sequence.cpp \
modules/set.cpp \
"

    if test_true ${B2_DEBUG_OPT} ; then B2_CXXFLAGS="${B2_CXXFLAGS_DEBUG}"
    else B2_CXXFLAGS="${B2_CXXFLAGS_RELEASE} -DNDEBUG"
    fi
    ( B2_VERBOSE_OPT=${TRUE} echo_run ${B2_CXX} ${B2_CXXFLAGS} ${B2_SOURCES} -o b2 )
}

if test_true ${B2_VERBOSE_OPT} ; then
    (
        ${B2_SETUP}
        build_b2
    )
else
    (
        ${B2_SETUP} 1>/dev/null 2>/dev/null
        build_b2
    )
fi
