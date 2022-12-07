@REM Copyright 2022 Peter Dimov
@REM Distributed under the Boost Software License, Version 1.0.
@REM https://www.boost.org/LICENSE_1_0.txt

@ECHO ON

set LIBRARY=%1
set DRONE_BUILD_DIR=%CD%

set BOOST_BRANCH=develop
if "%DRONE_BRANCH%" == "master" set BOOST_BRANCH=master
cd ..
git clone -b %BOOST_BRANCH% --depth 1 https://github.com/boostorg/boost.git boost-root
cd boost-root
git submodule update --init tools/boostdep
xcopy /s /e /q %DRONE_BUILD_DIR% libs\%LIBRARY%\
python tools/boostdep/depinst/depinst.py %LIBRARY%
cmd /c bootstrap
b2 -d0 headers

if not "%CXXSTD%" == "" set CXXSTD=cxxstd=%CXXSTD%
if not "%ADDRMD%" == "" set ADDRMD=address-model=%ADDRMD%
b2 -j3 libs/%LIBRARY%/test toolset=%TOOLSET% %CXXSTD% %ADDRMD% variant=debug,release embed-manifest-via=linker
