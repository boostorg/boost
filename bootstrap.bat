@ECHO OFF

REM Copyright (C) 2009 Vladimir Prus
REM
REM Distributed under the Boost Software License, Version 1.0.
REM (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

REM Process command-line arguments, if any
call :process-args %*
if %ERRORLEVEL% NEQ 0 goto :eof

if "%TOOLSET%"=="" (
    set TOOLSET=msvc)

ECHO Building Boost.Build engine
if exist ".\tools\build\src\engine\bin.ntx86\b2.exe" del tools\build\src\engine\bin.ntx86\b2.exe
if exist ".\tools\build\src\engine\bin.ntx86\bjam.exe" del tools\build\src\engine\bin.ntx86\bjam.exe
if exist ".\tools\build\src\engine\bin.ntx86_64\b2.exe" del tools\build\src\engine\bin.ntx86_64\b2.exe
if exist ".\tools\build\src\engine\bin.ntx86_64\bjam.exe" del tools\build\src\engine\bin.ntx86_64\bjam.exe
pushd tools\build\src\engine

call .\build.bat %TOOLSET% > ..\..\..\..\bootstrap.log
@ECHO OFF

popd

if exist ".\tools\build\src\engine\bin.ntx86\bjam.exe" (
   copy .\tools\build\src\engine\bin.ntx86\b2.exe . > nul
   copy .\tools\build\src\engine\bin.ntx86\bjam.exe . > nul
   goto :bjam_built)

if exist ".\tools\build\src\engine\bin.ntx86_64\bjam.exe" (
   copy .\tools\build\src\engine\bin.ntx86_64\b2.exe . > nul
   copy .\tools\build\src\engine\bin.ntx86_64\bjam.exe . > nul
   goto :bjam_built)

goto :bjam_failure

:bjam_built

ECHO import option ; > project-config.jam
ECHO. >> project-config.jam
ECHO using %TOOLSET% ; >> project-config.jam
ECHO. >> project-config.jam
ECHO option.set keep-going : false ; >> project-config.jam
ECHO. >> project-config.jam

ECHO.
ECHO Bootstrapping is done. To build, run:
ECHO.
ECHO     .\b2
ECHO.    
ECHO To adjust configuration, edit 'project-config.jam'.
ECHO Further information:
ECHO.
ECHO     - Command line help:
ECHO     .\b2 --help
ECHO.     
ECHO     - Getting started guide: 
ECHO     http://boost.org/more/getting_started/windows.html
ECHO.     
ECHO     - Boost.Build documentation:
ECHO     http://www.boost.org/build/doc/html/index.html

goto :eof

:bjam_failure

ECHO.
ECHO Failed to build Boost.Build engine.
ECHO Please consult bootstrap.log for furter diagnostics.
ECHO.
ECHO You can try to obtain a prebuilt binary from
ECHO.
ECHO    http://sf.net/project/showfiles.php?group_id=7586^&package_id=72941
ECHO.
ECHO Also, you can file an issue at http://svn.boost.org 
ECHO Please attach bootstrap.log in that case.

goto :eof

:process-args
if "%~1"=="" exit /b 0
if "%1"=="--with-toolset" (
    if "%2"=="" (
        echo ERROR: Expecting the name of a toolset
        exit /b 1)
    set TOOLSET=%2
    shift
    shift)
goto :process-args
