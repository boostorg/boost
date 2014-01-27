rem Copyright Beman Dawes 2005

rem Distributed under the Boost Software License, Version 1.0.
rem See http://www.boost.org/LICENSE_1_0.txt

echo Begin build...
pushd ..\build
b2 variant=release install >build.log 2>&1
start build.log
popd

echo Begin test processing...
b2 --dump-tests %* >test.log 2>&1
echo Begin log processing...
..\build\bin\process_jam_log --v2 <test.log
start test.log
echo Begin compiler status processing...
..\build\bin\compiler_status --v2 . test_status.html test_links.html
start test_status.html
