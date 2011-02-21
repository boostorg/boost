@ECHO OFF

REM Copyright (C) 2009 Vladimir Prus
REM
REM Distributed under the Boost Software License, Version 1.0.
REM (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

ECHO Building Boost.Jam build engine
if exist ".\tools\build\v2\engine\src\bin.ntx86\bjam.exe" del tools\build\v2\engine\src\bin.ntx86\bjam.exe
if exist ".\tools\build\v2\engine\src\bin.ntx86_64\bjam.exe" del tools\build\v2\engine\src\bin.ntx86_64\bjam.exe
cd tools\build\v2\engine\src

call .\build.bat > ..\..\..\bjam.log
@ECHO OFF
cd ..\..\..\..\..

if exist ".\tools\build\v2\engine\src\bin.ntx86\bjam.exe" (
   copy .\tools\build\v2\engine\src\bin.ntx86\bjam.exe . > nul
   goto :bjam_built)

if exist ".\tools\build\v2\engine\src\bin.ntx86_64\bjam.exe" (
   copy .\tools\build\v2\engine\src\bin.ntx86_64\bjam.exe . > nul
   goto :bjam_built)

goto :bjam_failure

:bjam_built

REM Ideally, we should obtain the toolset that build.bat has
REM guessed. However, it uses setlocal at the start and does
REM export BOOST_JAM_TOOLSET, and I don't know how to do that
REM properly. Default to msvc for now.
set toolset=msvc

ECHO using %toolset% ; > project-config.jam

ECHO.
ECHO Bootstrapping is done. To build, run:
ECHO.
ECHO     .\bjam
ECHO.    
ECHO To adjust configuration, edit 'project-config.jam'.
ECHO Further information:
ECHO.
ECHO     - Command line help:
ECHO     .\bjam --help
ECHO.     
ECHO     - Getting started guide: 
ECHO     http://boost.org/more/getting_started/windows.html
ECHO.     
ECHO     - Boost.Build documentation:
ECHO     http://www.boost.org/boost-build2/doc/html/index.html

goto :end

:bjam_failure

ECHO.
ECHO Failed to build Boost.Jam build engine.
ECHO Please consult bjam.log for furter diagnostics.
ECHO.
ECHO You can try to obtain a prebuilt binary from
ECHO.
ECHO    http://sf.net/project/showfiles.php?group_id=7586^&package_id=72941
ECHO.
ECHO Also, you can file an issue at http://svn.boost.org 
ECHO Please attach bjam.log in that case.

goto :end

:end
