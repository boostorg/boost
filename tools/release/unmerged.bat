@echo off
if not %1$==$ goto usage_ok
echo Usage: unmerged library-name [svn-options]
echo Options include --summarize to show paths only. i.e. suppresses line-by-line diffs
goto done

:usage_ok
svn diff %2 %3 %4 %5 %6 http://svn.boost.org/svn/boost/branches/release/boost/%1.hpp ^
  http://svn.boost.org/svn/boost/trunk/boost/%1.hpp
svn diff %2 %3 %4 %5 %6 http://svn.boost.org/svn/boost/branches/release/boost/%1 ^
  http://svn.boost.org/svn/boost/trunk/boost/%1
svn diff %2 %3 %4 %5 %6 http://svn.boost.org/svn/boost/branches/release/libs/%1 ^
  http://svn.boost.org/svn/boost/trunk/libs/%1

:done
