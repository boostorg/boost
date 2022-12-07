@ECHO OFF

REM ~ Copyright 2002-2007 Rene Rivera.
REM ~ Distributed under the Boost Software License, Version 1.0.
REM ~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

setlocal
goto Start


:Set_Error
color 00
goto :eof


:Clear_Error
ver >nul
goto :eof


:Error_Print
REM Output an error message and set the errorlevel to indicate failure.
setlocal
ECHO ###
ECHO ### %1
ECHO ###
ECHO ### You can specify the toolset as the argument, i.e.:
ECHO ###     .\build.bat msvc
ECHO ###
ECHO ### Toolsets supported by this script are: borland, como, gcc,
ECHO ###     gcc-nocygwin, intel-win32, mingw,
ECHO ###     vc12, vc14, vc141, vc142, vc143
ECHO ###
ECHO ### If you have Visual Studio 2017 installed you will need to either update
ECHO ### the Visual Studio 2017 installer or run from VS 2017 Command Prompt
ECHO ### as we where unable to detect your toolset installation.
ECHO ###
call :Set_Error
endlocal
goto :eof


:Test_Option
REM Tests whether the given string is in the form of an option: "--*"
call :Clear_Error
setlocal
set test=%1
if not defined test (
    call :Set_Error
    goto Test_Option_End
)
set test=###%test%###
set test=%test:"###=%
set test=%test:###"=%
set test=%test:###=%
if not "-" == "%test:~1,1%" call :Set_Error
:Test_Option_End
endlocal
goto :eof


:Test_Empty
REM Tests whether the given string is not empty
call :Clear_Error
setlocal
set test=%1
if not defined test (
    call :Clear_Error
    goto Test_Empty_End
)
set test=###%test%###
set test=%test:"###=%
set test=%test:###"=%
set test=%test:###=%
if not "" == "%test%" call :Set_Error
:Test_Empty_End
endlocal
goto :eof


:Guess_Toolset
set local
REM Try and guess the toolset to bootstrap the build with...
REM Sets B2_TOOLSET to the first found toolset.
REM May also set B2_TOOLSET_ROOT to the
REM location of the found toolset.

call :Clear_Error
call :Test_Empty "%ProgramFiles%"
if not errorlevel 1 set "ProgramFiles=C:\Program Files"

REM Visual Studio is by default installed to %ProgramFiles% on 32-bit machines and
REM %ProgramFiles(x86)% on 64-bit machines. Making a common variable for both.
call :Clear_Error
call :Test_Empty "%ProgramFiles(x86)%"
if errorlevel 1 (
    set "VS_ProgramFiles=%ProgramFiles(x86)%"
) else (
    set "VS_ProgramFiles=%ProgramFiles%"
)

call guess_toolset.bat
if errorlevel 1 (
    call :Error_Print "Could not find a suitable toolset.")
goto :eof

endlocal
goto :eof


:Start
set B2_TOOLSET=
set B2_BUILD_ARGS=

REM If no arguments guess the toolset;
REM or if first argument is an option guess the toolset;
REM otherwise the argument is the toolset to use.
call :Clear_Error
call :Test_Empty %1
if not errorlevel 1 (
    call :Guess_Toolset
    if not errorlevel 1 ( goto Setup_Toolset ) else ( goto Finish )
)

call :Clear_Error
call :Test_Option %1
if not errorlevel 1 (
    call :Guess_Toolset
    if not errorlevel 1 ( goto Setup_Toolset ) else ( goto Finish )
)

call :Clear_Error
set B2_TOOLSET=%1
shift
goto Setup_Toolset


:Setup_Toolset
REM Setup the toolset command and options. This bit of code
REM needs to be flexible enough to handle both when
REM the toolset was guessed at and found, or when the toolset
REM was indicated in the command arguments.
REM NOTE: The strange multiple "if ?? == _toolset_" tests are that way
REM because in BAT variables are subsituted only once during a single
REM command. A complete "if ... else ..."
REM is a single command, even though it's in multiple lines here.
:Setup_Args
call :Clear_Error
call :Test_Empty %1
if not errorlevel 1 goto Config_Toolset
call :Clear_Error
call :Test_Option %1
if errorlevel 1 (
    set B2_BUILD_ARGS=%B2_BUILD_ARGS% %1
    shift
    goto Setup_Args
)
:Config_Toolset
call config_toolset.bat
if "_%_known_%_" == "__" (
    call :Error_Print "Unknown toolset: %B2_TOOLSET%"
)
if errorlevel 1 goto Finish

echo ###
echo ### Using '%B2_TOOLSET%' toolset.
echo ###

set B2_SOURCES=
set B2_SOURCES=%B2_SOURCES% builtins.cpp class.cpp
set B2_SOURCES=%B2_SOURCES% command.cpp compile.cpp constants.cpp cwd.cpp
set B2_SOURCES=%B2_SOURCES% debug.cpp debugger.cpp
set B2_SOURCES=%B2_SOURCES% execcmd.cpp execnt.cpp execunix.cpp filent.cpp filesys.cpp fileunix.cpp frames.cpp function.cpp
set B2_SOURCES=%B2_SOURCES% glob.cpp hash.cpp hcache.cpp hdrmacro.cpp headers.cpp jam.cpp
set B2_SOURCES=%B2_SOURCES% jamgram.cpp lists.cpp make.cpp make1.cpp md5.cpp mem.cpp modules.cpp
set B2_SOURCES=%B2_SOURCES% native.cpp object.cpp option.cpp output.cpp parse.cpp pathnt.cpp
set B2_SOURCES=%B2_SOURCES% pathsys.cpp pathunix.cpp regexp.cpp rules.cpp scan.cpp search.cpp jam_strings.cpp
set B2_SOURCES=%B2_SOURCES% startup.cpp subst.cpp sysinfo.cpp
set B2_SOURCES=%B2_SOURCES% timestamp.cpp variable.cpp w32_getreg.cpp
set B2_SOURCES=%B2_SOURCES% modules/order.cpp
set B2_SOURCES=%B2_SOURCES% modules/path.cpp
set B2_SOURCES=%B2_SOURCES% modules/property-set.cpp
set B2_SOURCES=%B2_SOURCES% modules/regex.cpp
set B2_SOURCES=%B2_SOURCES% modules/sequence.cpp
set B2_SOURCES=%B2_SOURCES% modules/set.cpp

set B2_CXXFLAGS=%B2_CXXFLAGS% -DNDEBUG

@echo ON
%B2_CXX% %CXXFLAGS% %B2_CXXFLAGS% %B2_SOURCES% %B2_CXX_LINK%
dir *.exe

:Finish
@exit /b %ERRORLEVEL%
