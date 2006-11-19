@pushd %~dp0..\..
@call bjam --v2 --clean msvc-7.1
@call bjam --dump-tests --v2 msvc-7.1 > test-cases\general\bjam.log
@popd