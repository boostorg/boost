# Niklaus Giger, 2005-03-15
# Testing whether we may run a test in absolute directories. There are no tests
# for temporary directories as this is implictly tested in a lot of other cases.

# TODO: Move to a separate testing-system test group.

import BoostBuild
import os
import tempfile

# Python 2.7 does not implement os.path.samefile on Windows
import ntpath
if not hasattr(ntpath, "samefile"):
    def samefile(f1, f2):
        try:
            from os.path.nt import _getfinalpathname
            return os.path._getfinalpathname(f1) == os.path._getfinalpathname(f2)
        except (NotImplementedError, ImportError):
            return os.path.abspath(f1) == os.path.abspath(f2)

    ntpath.samefile = samefile

t = BoostBuild.Tester(["-ffile.jam"], workdir=os.getcwd(), pass_d0=False,
    pass_toolset=False)

t.write("file.jam", "EXIT [ PWD ] : 0 ;")

t.run_build_system()
t.fail_test(not os.path.samefile(t.stdout().rstrip("\n"), os.getcwd()))

try:
    t.run_build_system(status=123, subdir="/must/fail/with/absolute/path",
        stderr=None)
except ValueError as e:
    assert "subdir" in str(e), e
else:
    raise ValueError("exception expected")
finally:
    t.cleanup()
