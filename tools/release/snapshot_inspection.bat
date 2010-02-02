rem Inspect snapshot

rem Copyright 2008 Beman Dawes

rem Distributed under the Boost Software License, Version 1.0.
rem See http://www.boost.org/LICENSE_1_0.txt

pushd posix\tools\inspect\build
bjam
popd
echo inspect...
pushd posix
dist\bin\inspect >..\inspect.html
popd

echo Create ftp script...
copy user.txt inspect.ftp
echo dir >>inspect.ftp
echo binary >>inspect.ftp
echo put inspect.html >>inspect.ftp
echo dir >>inspect.ftp
echo mdelete inspect-snapshot.html >>inspect.ftp
echo rename inspect.html inspect-snapshot.html >>inspect.ftp
echo dir >>inspect.ftp
echo bye >>inspect.ftp

echo Run ftp script...
ftp -n -i -s:inspect.ftp boost.cowic.de

echo Inspect script complete
