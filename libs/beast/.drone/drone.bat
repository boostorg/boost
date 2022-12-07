
@ECHO ON
setlocal enabledelayedexpansion

if "%DRONE_JOB_BUILDTYPE%" == "boost" (

echo "============> INSTALL"

SET DRONE_BUILD_DIR=%CD: =%
choco install --no-progress -y openssl --x64 --version 1.1.1.1000
mklink /D "C:\OpenSSL" "C:\Program Files\OpenSSL-Win64"
SET OPENSSL_ROOT=C:\OpenSSL
SET BOOST_BRANCH=develop
IF "%DRONE_BRANCH%" == "master" SET BOOST_BRANCH=master
cp tools\user-config.jam %USERPROFILE%\user-config.jam
cd ..
SET GET_BOOST=!DRONE_BUILD_DIR!\tools\get-boost.sh
bash -c "$GET_BOOST $DRONE_BRANCH $DRONE_BUILD_DIR"
cd boost-root
call bootstrap.bat
b2 headers

echo "============> SCRIPT"

echo "Running tests"
b2 --debug-configuration variant=%VARIANT% cxxstd=%CXXSTD% define=%DEFINE% address-model=%ADDRESS_MODEL% toolset=%TOOLSET% --verbose-test libs/beast/test -j3
if !errorlevel! neq 0 exit /b !errorlevel!

echo "Running libs/beast/example"
b2 --debug-configuration variant=%VARIANT% cxxstd=%CXXSTD% define=%DEFINE% address-model=%ADDRESS_MODEL% toolset=%TOOLSET% libs/beast/example -j3
if !errorlevel! neq 0 exit /b !errorlevel!

echo "Running run-fat-tests"
b2 --debug-configuration variant=%VARIANT% cxxstd=%CXXSTD% define=%DEFINE% address-model=%ADDRESS_MODEL% toolset=%TOOLSET% --verbose-test libs/beast/test//run-fat-tests -j3
if !errorlevel! neq 0 exit /b !errorlevel!

echo "============> COMPLETED"

)
