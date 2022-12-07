
set PROCESSORS=%NUMBER_OF_PROCESSORS%

REM Optionally throttle cpus:
REM if %PROCESSORS% GTR 2 set PROCESSORS=2

b2 --user-config=user-config.jam ^
  asio.mode=dflt,nodep,nots,ts,nodep-nots,nodep-ts ^
  variant=release ^
  cxxstd=2a,17,14,11 ^
  -j%PROCESSORS% ^
  -q ^
  libs/beast/test libs/beast/example
