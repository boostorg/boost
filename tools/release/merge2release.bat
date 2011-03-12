rem @echo off
if not %1$==$ goto usage_ok
echo Usage: merge2release library-name [svn-options]
echo Options include --dry-run
goto done

:usage_ok
pushd %BOOST_RELEASE%
pushd boost
svn merge %2 %3 %4 %5 %6 https://svn.boost.org/svn/boost/branches/release/boost/%1.hpp ^
  https://svn.boost.org/svn/boost/trunk/boost/%1.hpp
pushd %1
svn merge %2 %3 %4 %5 %6 https://svn.boost.org/svn/boost/branches/release/boost/%1 ^
  https://svn.boost.org/svn/boost/trunk/boost/%1
popd
popd
pushd libs\%1  
svn merge %2 %3 %4 %5 %6 https://svn.boost.org/svn/boost/branches/release/libs/%1 ^
  https://svn.boost.org/svn/boost/trunk/libs/%1
popd 
popd 

:done
