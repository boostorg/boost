@echo off
setlocal

:: run bootstrap.bat if b2.exe is not available
if not exist "b2.exe" call bootstrap.bat

:: we need to activate emscripten tools-chain before building boost
if "%EMSCRIPTEN%"=="" (
    ECHO.
    ECHO Failed to find Emscripten SDK
    ECHO Please activate Emscripten SDK by calling "emsdk activate ..."
    goto :end
)

:: generate project-config.jam for emscripten build
set toolset=emscripten
set _emcc=%EMSCRIPTEN%\em++.bat
set emcc=%_emcc:\=/%

ECHO import option ; > project-config.jam
ECHO. >> project-config.jam
ECHO using %toolset% : : %emcc% ; >> project-config.jam
ECHO using msvc ; >> project-config.jam
ECHO. >> project-config.jam
ECHO option.set keep-going : false ; >> project-config.jam
ECHO. >> project-config.jam

:: build boost system filesystem only
b2 toolset=emscripten link=static variant=debug,release threading=single -d+2 system filesystem

:end