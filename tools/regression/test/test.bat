set TEST_LOCATE_ROOT=d:\temp

echo Begin test processing...
bjam --dump-tests "-sALL_LOCATE_TARGET=%TEST_LOCATE_ROOT%" %* >bjam.log 2>&1
echo Begin log processing...
process_jam_log %TEST_LOCATE_ROOT% <bjam.log
start bjam.log
echo Begin compiler status processing...
compiler_status --locate-root %TEST_LOCATE_ROOT% %BOOST_ROOT% test_status.html test_links.html
start test_status.html
