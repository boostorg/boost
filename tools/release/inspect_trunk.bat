rem Inspect Trunk
rem Copyright Beman Dawes 2008, 2009

rem Distributed under the Boost Software License, Version 1.0.
rem See http://www.boost.org/LICENSE_1_0.txt

echo Must be run in directory containing svn checkout of trunk

echo Update trunk working copy...
svn up --non-interactive
pushd tools\inspect\build
echo Build inspect program...
bjam
popd
echo Inspect...
dist\bin\inspect >%TEMP%\trunk_inspect.html

echo Create ftp script...
pushd %TEMP%
copy %BOOST_TRUNK%\..\user.txt inspect.ftp
echo dir >>inspect.ftp
echo binary >>inspect.ftp
echo put trunk_inspect.html >>inspect.ftp
echo dir >>inspect.ftp
echo mdelete inspect-trunk.html >>inspect.ftp
echo rename trunk_inspect.html inspect-trunk.html >>inspect.ftp
echo dir >>inspect.ftp
echo bye >>inspect.ftp

echo Run ftp script...
ftp -n -i -s:inspect.ftp boost.cowic.de
popd

echo Update script for next run
copy tools\release\inspect_trunk.bat .

echo Inspect script complete
