@ECHO OFF

REM Copyright (C) 2009 Vladimir Prus
REM Copyright 2019-2020 Rene Rivera
REM
REM Distributed under the Boost Software License, Version 1.0.
REM (See accompanying file LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)


:b2_build
ECHO Building the B2 engine..
pushd src\engine
call .\build.bat %*
@ECHO OFF
popd
if exist ".\src\engine\b2.exe" (
    copy .\src\engine\b2.exe . > nul
    goto :b2_built)
goto :b2_failure


:b2_built
ECHO.
ECHO Building is done. To install, run:
ECHO.
ECHO     .\b2 --prefix=DIR install
ECHO.
goto :end


:b2_failure
ECHO.
ECHO Failed to build the B2 engine.
ECHO.
goto :end


:end
exit /b %ERRORLEVEL%
