@ECHO OFF

REM ~ Copyright 2002-2018 Rene Rivera.
REM ~ Distributed under the Boost Software License, Version 1.0.
REM ~ (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

if "_%1_" == "_yacc_" goto Guess_Yacc
if "_%1_" == "_test_path_" (
    shift
    goto Test_Path)
goto Guess


:Clear_Error
ver >nul
goto :eof


:Test_Path
REM Tests for the given executable file presence in the directories in the PATH
REM environment variable. Additionally sets FOUND_PATH to the path of the
REM found file.
call :Clear_Error
setlocal
set test=%~$PATH:1
endlocal
if not errorlevel 1 set FOUND_PATH=%~dp$PATH:1
goto :eof


:Guess
REM Let vswhere tell us where msvc is at, if available.
call :Clear_Error
call vswhere_usability_wrapper.cmd
call :Clear_Error
if NOT "_%VS170COMNTOOLS%_" == "__" (
    set "B2_TOOLSET=vc143"
    set "B2_TOOLSET_ROOT=%VS170COMNTOOLS%..\..\VC\"
    goto :eof)
if NOT "_%VS160COMNTOOLS%_" == "__" (
    set "B2_TOOLSET=vc142"
    set "B2_TOOLSET_ROOT=%VS160COMNTOOLS%..\..\VC\"
    goto :eof)
if NOT "_%VS150COMNTOOLS%_" == "__" (
    set "B2_TOOLSET=vc141"
    set "B2_TOOLSET_ROOT=%VS150COMNTOOLS%..\..\VC\"
    goto :eof)
REM VSUNKCOMNTOOLS represents unknown but detected version from vswhere
if NOT "_%VSUNKCOMNTOOLS%_" == "__" (
    set "B2_TOOLSET=vcunk"
    set "B2_TOOLSET_ROOT=%VSUNKCOMNTOOLS%..\..\VC\"
    goto :eof)
if EXIST "%VS_ProgramFiles%\Microsoft Visual Studio\2017\Enterprise\VC\Auxiliary\Build\vcvarsall.bat"  (
    set "B2_TOOLSET=vc141"
    set "B2_TOOLSET_ROOT=%VS_ProgramFiles%\Microsoft Visual Studio\2017\Enterprise\VC\"
    exit /b 0)
if EXIST "%VS_ProgramFiles%\Microsoft Visual Studio\2017\Professional\VC\Auxiliary\Build\vcvarsall.bat"  (
    set "B2_TOOLSET=vc141"
    set "B2_TOOLSET_ROOT=%VS_ProgramFiles%\Microsoft Visual Studio\2017\Professional\VC\"
    exit /b 0)
if EXIST "%VS_ProgramFiles%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"  (
    set "B2_TOOLSET=vc141"
    set "B2_TOOLSET_ROOT=%VS_ProgramFiles%\Microsoft Visual Studio\2017\Community\VC\"
    exit /b 0)
if NOT "_%VS140COMNTOOLS%_" == "__" (
    set "B2_TOOLSET=vc14"
    set "B2_TOOLSET_ROOT=%VS140COMNTOOLS%..\..\VC\"
    exit /b 0)
if EXIST "%VS_ProgramFiles%\Microsoft Visual Studio 14.0\VC\VCVARSALL.BAT" (
    set "B2_TOOLSET=vc14"
    set "B2_TOOLSET_ROOT=%VS_ProgramFiles%\Microsoft Visual Studio 14.0\VC\"
    exit /b 0)
if NOT "_%VS120COMNTOOLS%_" == "__" (
    set "B2_TOOLSET=vc12"
    set "B2_TOOLSET_ROOT=%VS120COMNTOOLS%..\..\VC\"
    exit /b 0)
if EXIST "%VS_ProgramFiles%\Microsoft Visual Studio 12.0\VC\VCVARSALL.BAT" (
    set "B2_TOOLSET=vc12"
    set "B2_TOOLSET_ROOT=%VS_ProgramFiles%\Microsoft Visual Studio 12.0\VC\"
    exit /b 0)
call :Test_Path cl.exe
if not errorlevel 1 (
    set "B2_TOOLSET=msvc"
    set "B2_TOOLSET_ROOT=%FOUND_PATH%..\"
    exit /b 0)
call :Test_Path vcvars32.bat
if not errorlevel 1 (
    set "B2_TOOLSET=msvc"
    call "%FOUND_PATH%VCVARS32.BAT"
    set "B2_TOOLSET_ROOT=%MSVCDir%\"
    exit /b 0)
call :Test_Path bcc32c.exe
if not errorlevel 1 (
    set "B2_TOOLSET=borland"
    set "B2_TOOLSET_ROOT=%FOUND_PATH%..\"
    exit /b 0)
call :Test_Path icl.exe
if not errorlevel 1 (
    set "B2_TOOLSET=intel-win32"
    set "B2_TOOLSET_ROOT=%FOUND_PATH%..\"
    exit /b 0)
if EXIST "C:\MinGW\bin\gcc.exe" (
    set "B2_TOOLSET=mingw"
    set "B2_TOOLSET_ROOT=C:\MinGW\"
    exit /b 0)
REM Could not find a suitable toolset
exit /b 1
