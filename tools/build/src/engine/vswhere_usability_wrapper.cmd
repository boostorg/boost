:: Copyright 2020 - Rene Ferdinand Rivera Morell
:: Copyright 2017 - Refael Ackermann
:: Copyright 2019 - Thomas Kent
:: Distributed under MIT style license
:: See accompanying file LICENSE at https://github.com/node4good/windows-autoconf
:: Forked from version: 1.15.4

@if not defined DEBUG_HELPER @ECHO OFF
setlocal
set "InstallerPath=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer"
if not exist "%InstallerPath%" set "InstallerPath=%ProgramFiles%\Microsoft Visual Studio\Installer"
if not exist "%InstallerPath%" goto :no-vswhere
:: Manipulate %Path% for easier " handeling
set Path=%Path%;%InstallerPath%
where vswhere 2> nul > nul
if errorlevel 1 goto :no-vswhere
set VSWHERE_REQ=-requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64
set VSWHERE_PRP=-property installationPath

REM Visual Studio Unknown Version, Beyond 2022
set VSWHERE_LMT=-version "[18.0,19.0)"
set VSWHERE_PRERELEASE=-prerelease
SET VSWHERE_ARGS=-latest -products * %VSWHERE_REQ% %VSWHERE_PRP% %VSWHERE_LMT% %VSWHERE_PRERELEASE%
for /f "usebackq tokens=*" %%i in (`vswhere %VSWHERE_ARGS%`) do (
    endlocal
	echo Found with vswhere %%i
    @rem comment out setting VCINSTALLDIR for Boost.build
    @rem set "VCINSTALLDIR=%%i\VC\"
    set "VSUNKCOMNTOOLS=%%i\Common7\Tools\"
    exit /B 0
)

REM Visual Studio 2022
set VSWHERE_LMT=-version "[17.0,18.0)"
set VSWHERE_PRERELEASE=-prerelease
SET VSWHERE_ARGS=-latest -products * %VSWHERE_REQ% %VSWHERE_PRP% %VSWHERE_LMT% %VSWHERE_PRERELEASE%
for /f "usebackq tokens=*" %%i in (`vswhere %VSWHERE_ARGS%`) do (
    endlocal
	echo Found with vswhere %%i
    @rem comment out setting VCINSTALLDIR for Boost.build
    @rem set "VCINSTALLDIR=%%i\VC\"
    set "VS170COMNTOOLS=%%i\Common7\Tools\"
    exit /B 0
)

REM Visual Studio 2019 (16.X, toolset 14.2)
set VSWHERE_LMT=-version "[16.0,17.0)"
SET VSWHERE_ARGS=-latest -products * %VSWHERE_REQ% %VSWHERE_PRP% %VSWHERE_LMT% %VSWHERE_PRERELEASE%
for /f "usebackq tokens=*" %%i in (`vswhere %VSWHERE_ARGS%`) do (
    endlocal
	echo Found with vswhere %%i
    @rem comment out setting VCINSTALLDIR for Boost.build
    @rem set "VCINSTALLDIR=%%i\VC\"
    set "VS160COMNTOOLS=%%i\Common7\Tools\"
    exit /B 0
)

REM Visual Studio 2017 (15.X, toolset 14.1)
set VSWHERE_LMT=-version "[15.0,16.0)"
SET VSWHERE_ARGS=-latest -products * %VSWHERE_REQ% %VSWHERE_PRP% %VSWHERE_LMT%
for /f "usebackq tokens=*" %%i in (`vswhere %VSWHERE_ARGS%`) do (
    endlocal
	echo Found with vswhere %%i
    @rem comment out setting VCINSTALLDIR for Boost.build
    @rem set "VCINSTALLDIR=%%i\VC\"
    set "VS150COMNTOOLS=%%i\Common7\Tools\"
    exit /B 0
)

:no-vswhere
endlocal
echo could not find "vswhere"
exit /B 1
