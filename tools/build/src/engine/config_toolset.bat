@ECHO OFF

REM ~ Copyright 2002-2018 Rene Rivera.
REM ~ Distributed under the Boost Software License, Version 1.0.
REM ~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

:Start
REM Setup the toolset command and options.
if "_%B2_TOOLSET%_" == "_msvc_" call :Config_MSVC
if "_%B2_TOOLSET%_" == "_vc12_" call :Config_VC12
if "_%B2_TOOLSET%_" == "_vc14_" call :Config_VC14
if "_%B2_TOOLSET%_" == "_vc141_" call :Config_VC141
if "_%B2_TOOLSET%_" == "_vc142_" call :Config_VC142
if "_%B2_TOOLSET%_" == "_vc143_" call :Config_VC143
if "_%B2_TOOLSET%_" == "_borland_" call :Config_BORLAND
if "_%B2_TOOLSET%_" == "_como_" call :Config_COMO
if "_%B2_TOOLSET%_" == "_gcc_" call :Config_GCC
if "_%B2_TOOLSET%_" == "_clang_" call :Config_CLANG
if "_%B2_TOOLSET%_" == "_gcc-nocygwin_" call :Config_GCC_NOCYGWIN
if "_%B2_TOOLSET%_" == "_intel-win32_" call :Config_INTEL_WIN32
if "_%B2_TOOLSET%_" == "_mingw_" call :Config_MINGW
exit /b %errorlevel%

:Call_If_Exists
ECHO Call_If_Exists %*
if EXIST %1 call %*
goto :eof

:Config_MSVC
if not defined CXX ( set "CXX=cl" )
if NOT "_%MSVCDir%_" == "__" (
    set "B2_TOOLSET_ROOT=%MSVCDir%\"
    )
call :Call_If_Exists "%B2_TOOLSET_ROOT%bin\VCVARS32.BAT"
if not "_%B2_TOOLSET_ROOT%_" == "__" (
    set "PATH=%B2_TOOLSET_ROOT%bin;%PATH%"
    )
set "B2_CXX="%CXX%" /nologo /MP /MT /TP /Feb2 /wd4996 /O2 /GL /EHsc"
set "B2_CXX_LINK=/link kernel32.lib advapi32.lib user32.lib"
set "_known_=1"
goto :eof

:Config_VC11
if not defined CXX ( set "CXX=cl" )
if NOT "_%VS110COMNTOOLS%_" == "__" (
    set "B2_TOOLSET_ROOT=%VS110COMNTOOLS%..\..\VC\"
    )
if "_%VCINSTALLDIR%_" == "__" call :Call_If_Exists "%B2_TOOLSET_ROOT%VCVARSALL.BAT" %B2_BUILD_ARGS%
if NOT "_%B2_TOOLSET_ROOT%_" == "__" (
    if "_%VCINSTALLDIR%_" == "__" (
        set "PATH=%B2_TOOLSET_ROOT%bin;%PATH%"
        ) )
set "B2_CXX="%CXX%" /nologo /MP /MT /TP /Feb2 /wd4996 /O2 /GL /EHsc"
set "B2_CXX_LINK=/link kernel32.lib advapi32.lib user32.lib"
set "_known_=1"
goto :eof

:Config_VC12
if not defined CXX ( set "CXX=cl" )
if NOT "_%VS120COMNTOOLS%_" == "__" (
    set "B2_TOOLSET_ROOT=%VS120COMNTOOLS%..\..\VC\"
    )

if "_%B2_ARCH%_" == "__" set B2_ARCH=%PROCESSOR_ARCHITECTURE%
set B2_BUILD_ARGS=%B2_BUILD_ARGS% %B2_ARCH%

if "_%VCINSTALLDIR%_" == "__" call :Call_If_Exists "%B2_TOOLSET_ROOT%VCVARSALL.BAT" %B2_BUILD_ARGS%
if NOT "_%B2_TOOLSET_ROOT%_" == "__" (
    if "_%VCINSTALLDIR%_" == "__" (
        set "PATH=%B2_TOOLSET_ROOT%bin;%PATH%"
        ) )
set "B2_CXX="%CXX%" /nologo /MP /MT /TP /Feb2 /wd4996 /O2 /GL /EHsc"
set "B2_CXX_LINK=/link kernel32.lib advapi32.lib user32.lib"
set "_known_=1"
goto :eof

:Config_VC14
if not defined CXX ( set "CXX=cl" )
if "_%B2_TOOLSET_ROOT%_" == "__" (
    if NOT "_%VS140COMNTOOLS%_" == "__" (
        set "B2_TOOLSET_ROOT=%VS140COMNTOOLS%..\..\VC\"
    ))

if "_%B2_ARCH%_" == "__" set B2_ARCH=%PROCESSOR_ARCHITECTURE%
set B2_BUILD_ARGS=%B2_BUILD_ARGS% %B2_ARCH%

if "_%VCINSTALLDIR%_" == "__" call :Call_If_Exists "%B2_TOOLSET_ROOT%VCVARSALL.BAT" %B2_BUILD_ARGS%
if NOT "_%B2_TOOLSET_ROOT%_" == "__" (
    if "_%VCINSTALLDIR%_" == "__" (
        set "PATH=%B2_TOOLSET_ROOT%bin;%PATH%"
        ) )
set "B2_CXX="%CXX%" /nologo /MP /MT /TP /Feb2 /wd4996 /O2 /GL /EHsc"
set "B2_CXX_LINK=/link kernel32.lib advapi32.lib user32.lib"
set "_known_=1"
goto :eof

:Config_VC141
if not defined CXX ( set "CXX=cl" )
call vswhere_usability_wrapper.cmd
REM Reset ERRORLEVEL since from now on it's all based on ENV vars
ver > nul 2> nul
if "_%B2_TOOLSET_ROOT%_" == "__" (
    if NOT "_%VS150COMNTOOLS%_" == "__" (
        set "B2_TOOLSET_ROOT=%VS150COMNTOOLS%..\..\VC\"
    ))

if "_%B2_ARCH%_" == "__" set B2_ARCH=%PROCESSOR_ARCHITECTURE%
set B2_BUILD_ARGS=%B2_BUILD_ARGS% %B2_ARCH%

REM return to current directory as vsdevcmd_end.bat switches to %USERPROFILE%\Source if it exists.
pushd %CD%
if "_%VSINSTALLDIR%_" == "__" call :Call_If_Exists "%B2_TOOLSET_ROOT%Auxiliary\Build\vcvarsall.bat" %B2_BUILD_ARGS%
popd
set "B2_CXX="%CXX%" /nologo /MP /MT /TP /Feb2 /wd4996 /O2 /GL /EHsc"
set "B2_CXX_LINK=/link kernel32.lib advapi32.lib user32.lib"
set "_known_=1"
goto :eof

:Config_VC142
if not defined CXX ( set "CXX=cl" )
call vswhere_usability_wrapper.cmd
REM Reset ERRORLEVEL since from now on it's all based on ENV vars
ver > nul 2> nul
if "_%B2_TOOLSET_ROOT%_" == "__" (
    if NOT "_%VS160COMNTOOLS%_" == "__" (
        set "B2_TOOLSET_ROOT=%VS160COMNTOOLS%..\..\VC\"
    ))

if "_%B2_ARCH%_" == "__" set B2_ARCH=%PROCESSOR_ARCHITECTURE%
set B2_BUILD_ARGS=%B2_BUILD_ARGS% %B2_ARCH%

REM return to current directory as vsdevcmd_end.bat switches to %USERPROFILE%\Source if it exists.
pushd %CD%
if "_%VSINSTALLDIR%_" == "__" call :Call_If_Exists "%B2_TOOLSET_ROOT%Auxiliary\Build\vcvarsall.bat" %B2_BUILD_ARGS%
popd
set "B2_CXX="%CXX%" /nologo /MP /MT /TP /Feb2 /wd4996 /O2 /GL /EHsc"
set "B2_CXX_LINK=/link kernel32.lib advapi32.lib user32.lib"
set "_known_=1"
goto :eof

:Config_VC143
if not defined CXX ( set "CXX=cl" )
call vswhere_usability_wrapper.cmd
REM Reset ERRORLEVEL since from now on it's all based on ENV vars
ver > nul 2> nul
if "_%B2_TOOLSET_ROOT%_" == "__" (
    if NOT "_%VS170COMNTOOLS%_" == "__" (
        set "B2_TOOLSET_ROOT=%VS170COMNTOOLS%..\..\VC\"
    ))

if "_%B2_ARCH%_" == "__" set B2_ARCH=%PROCESSOR_ARCHITECTURE%
set B2_BUILD_ARGS=%B2_BUILD_ARGS% %B2_ARCH%

REM return to current directory as vsdevcmd_end.bat switches to %USERPROFILE%\Source if it exists.
pushd %CD%
if "_%VSINSTALLDIR%_" == "__" call :Call_If_Exists "%B2_TOOLSET_ROOT%Auxiliary\Build\vcvarsall.bat" %B2_BUILD_ARGS%
popd
@REM set "B2_CXX="%CXX%" /nologo /MP /MT /TP /Feb2 /wd4996 /O2 /GL /EHsc"
set "B2_CXX="%CXX%" /nologo -TP /wd4996 /wd4675 /EHs /GR /Zc:throwingNew /O2 /Ob2 /W3 /MD /Zc:forScope /Zc:wchar_t /Zc:inline /Gw /favor:blend /Feb2"
set "B2_CXX_LINK=/link kernel32.lib advapi32.lib user32.lib"
set "_known_=1"
goto :eof

:Config_VCUNK
if NOT "_%B2_TOOLSET%_" == "_vcunk_" goto Skip_VCUNK
call vswhere_usability_wrapper.cmd
REM Reset ERRORLEVEL since from now on it's all based on ENV vars
ver > nul 2> nul
if "_%B2_TOOLSET_ROOT%_" == "__" (
    if NOT "_%VSUNKCOMNTOOLS%_" == "__" (
        set "B2_TOOLSET_ROOT=%VSUNKCOMNTOOLS%..\..\VC\"
    ))

if "_%B2_ARCH%_" == "__" set B2_ARCH=%PROCESSOR_ARCHITECTURE%
set B2_BUILD_ARGS=%B2_BUILD_ARGS% %B2_ARCH%

REM return to current directory as vsdevcmd_end.bat switches to %USERPROFILE%\Source if it exists.
pushd %CD%
if "_%VSINSTALLDIR%_" == "__" call :Call_If_Exists "%B2_TOOLSET_ROOT%Auxiliary\Build\vcvarsall.bat" %B2_BUILD_ARGS%
popd
set "B2_CXX="%CXX%" /nologo /MP /MT /TP /Feb2 /wd4996 /O2 /GL /EHsc"
set "B2_CXX_LINK=/link kernel32.lib advapi32.lib user32.lib"
set "_known_=1"
goto :eof

:Config_BORLAND
if not defined CXX ( set "CXX=bcc32c" )
if "_%B2_TOOLSET_ROOT%_" == "__" (
    call guess_toolset.bat test_path bcc32c.exe )
if "_%B2_TOOLSET_ROOT%_" == "__" (
    if not errorlevel 1 (
        set "B2_TOOLSET_ROOT=%FOUND_PATH%..\"
        ) )
if not "_%B2_TOOLSET_ROOT%_" == "__" (
    set "PATH=%B2_TOOLSET_ROOT%Bin;%PATH%"
    )
set "B2_CXX="%CXX%" -tC -P -O2 -w- -I"%B2_TOOLSET_ROOT%Include" -L"%B2_TOOLSET_ROOT%Lib" -eb2"
set "_known_=1"
goto :eof

:Config_COMO
if not defined CXX ( set "CXX=como" )
set "B2_CXX="%CXX%" --inlining -o b2.exe"
set "_known_=1"
goto :eof

:Config_GCC
if not defined CXX ( set "CXX=g++" )
set "B2_CXX="%CXX%" -x c++ -std=c++11 -s -O3 -o b2.exe -D_GNU_SOURCE"
set "_known_=1"
goto :eof

:Config_CLANG
if not defined CXX ( set "CXX=clang++" )
set "B2_CXX="%CXX%" -x c++ -std=c++11 -s -O3 -o b2.exe"
set "_known_=1"
goto :eof

:Config_GCC_NOCYGWIN
if not defined CXX ( set "CXX=g++" )
set "B2_CXX="%CXX%" -x c++ -std=c++11 -s -O3 -mno-cygwin -o b2.exe"
set "_known_=1"
goto :eof

:Config_INTEL_WIN32
if not defined CXX ( set "CXX=icl" )
set "B2_CXX="%CXX%" /nologo /MT /O2 /Ob2 /Gy /GF /GA /GB /Feb2"
set "_known_=1"
goto :eof

:Config_MINGW
if not defined CXX ( set "CXX=g++" )
if not "_%B2_TOOLSET_ROOT%_" == "__" (
    set "PATH=%B2_TOOLSET_ROOT%bin;%PATH%"
    )
for /F "delims=" %%I in ("%CXX%") do set "PATH=%PATH%;%%~dpI"
set "B2_CXX="%CXX%" -x c++ -std=c++11 -s -O3 -o b2.exe"
set "_known_=1"
goto :eof
