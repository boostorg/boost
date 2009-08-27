if not %1$==$ goto usage_ok
echo Usage: upload2sourceforge release-folder
echo Example: upload2sourceforge 1.40.0
goto done

:usage_ok
dir boost_*
pause Are these the correct files to upload? [Ctrl-C to interrupt]
rsync -avP -e ssh boost_* beman_dawes,boost@frs.sourceforge.net:/home/frs/project/b/bo/boost/boost/%1/

:done
