#!/usr/bin/python

# Copyright 2018 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)

# Test the package module.

import BoostBuild
import os

def setup():
    t = BoostBuild.Tester(["p//install", "p//data"],
                          use_test_config=False)

    t.write("p/jamroot.jam", "")
    t.write("p/jamfile.jam", """\
    import package ;
    exe a : a.cpp ;
    lib b : b.cpp ;
    package.install install Test :
      : a
      : b/<link>static b/<link>shared
      : a.h ;
    package.install-data data : Test : a.txt ;
    """)
    t.write("p/a.cpp", "int main() {}")
    t.write("p/b.cpp", """
    int
    #ifdef _WIN32
    __declspec(dllexport)
    #endif
    must_export_something;
    """)
    t.write("p/a.h", "")
    t.write("p/a.txt", "")
    return t

def test_defaults():
    t = setup()

    # Since the default install location is outside out test area,
    # we don't want to actually execute the build.
    t.run_build_system(["-n", "-d1"])

    installdir = "C:/Test" if os.name == 'nt' else "/usr/local"
    t.expect_output_lines([
        x.replace('/', os.sep) for x in
        ["common.copy %s/bin/%s" % (installdir, t.adjust_name("a.exe")),
         "common.copy %s/lib/%s" % (installdir, t.adjust_name("b.dll")),
         "common.copy %s/lib/%s" % (installdir, t.adjust_name("b.lib")),
         "common.copy %s/include/a.h" % installdir,
         "common.copy %s/share/Test/a.txt" % installdir]])

    t.cleanup()

def test_prefix():
    t = setup()
    # An explicit --prefix on the command should override all of these:
    t.write("project-config.jam", """
    option.set prefix : bad ;
    option.set bindir : bad/bin ;
    option.set libdir : bad/lib ;
    option.set includedir : bad/include ;
    option.set datarootdir : bad/share ;
    """)

    t.run_build_system(["--prefix=installdir"])
    t.expect_addition("installdir/bin/a.exe")
    t.expect_addition("installdir/lib/b.dll")
    t.expect_addition("installdir/lib/b.lib")
    t.expect_addition("installdir/include/a.h")
    t.expect_addition("installdir/share/Test/a.txt")

    t.cleanup()

def test_subdirs():
    t = setup()
    # Command line options override config files
    t.write("project-config.jam", """
    option.set prefix : bad ;
    option.set bindir : bad/bin ;
    option.set libdir : bad/lib ;
    option.set includedir : bad/include ;
    option.set datarootdir : bad/share ;
    """)

    t.run_build_system(["--libdir=installdir/lib64",
                        "--bindir=installdir/binx",
                        "--includedir=installdir/includex",
                        "--datarootdir=installdir/sharex"])
    t.expect_addition("installdir/binx/a.exe")
    t.expect_addition("installdir/lib64/b.dll")
    t.expect_addition("installdir/lib64/b.lib")
    t.expect_addition("installdir/includex/a.h")
    t.expect_addition("installdir/sharex/Test/a.txt")

    t.cleanup()

def test_subdirs_with_prefix():
    t = setup()
    # Command line options override config files
    t.write("project-config.jam", """
    option.set prefix : bad ;
    option.set bindir : bad/bin ;
    option.set libdir : bad/lib ;
    option.set includedir : bad/include ;
    option.set datarootdir : bad/share ;
    """)

    t.run_build_system(["--prefix=bad",
                        "--libdir=installdir/lib64",
                        "--bindir=installdir/binx",
                        "--includedir=installdir/includex",
                        "--datarootdir=installdir/sharex"])
    t.expect_addition("installdir/binx/a.exe")
    t.expect_addition("installdir/lib64/b.dll")
    t.expect_addition("installdir/lib64/b.lib")
    t.expect_addition("installdir/includex/a.h")
    t.expect_addition("installdir/sharex/Test/a.txt")

    t.cleanup()

def test_prefix_config_file():
    t = setup()
    # An explicit --prefix on the command should override all of these:
    t.write("project-config.jam", """
    option.set prefix : installdir ;
    """)

    t.run_build_system()
    t.expect_addition("installdir/bin/a.exe")
    t.expect_addition("installdir/lib/b.dll")
    t.expect_addition("installdir/lib/b.lib")
    t.expect_addition("installdir/include/a.h")
    t.expect_addition("installdir/share/Test/a.txt")

    t.cleanup()

def test_subdirs_config_file():
    t = setup()
    # An explicit --prefix on the command should override all of these:
    t.write("project-config.jam", """
    option.set prefix : installdir ;
    option.set libdir : installdir/lib64 ;
    option.set bindir : installdir/binx ;
    option.set includedir : installdir/includex ;
    option.set datarootdir : installdir/sharex ;
    """)

    t.run_build_system()
    t.expect_addition("installdir/binx/a.exe")
    t.expect_addition("installdir/lib64/b.dll")
    t.expect_addition("installdir/lib64/b.lib")
    t.expect_addition("installdir/includex/a.h")
    t.expect_addition("installdir/sharex/Test/a.txt")

    t.cleanup()

def test_multiple():
    '''If no prefix is specified, we might use several default
    install prefixes.'''
    t = BoostBuild.Tester(use_test_config=False)

    t.write("p/jamroot.jam", "")
    t.write("p/jamfile.jam", """\
    import package ;
    exe a : a.cpp ;
    lib b : b.cpp ;
    package.install installx TestX : <install-default-prefix>xxx
      : a
      : b/<link>static b/<link>shared
      : a.h ;
    package.install instally TestY : <install-default-prefix>yyy
      : a
      : b/<link>static b/<link>shared
      : a.h ;
    """)
    t.write("p/a.cpp", "int main() {}")
    t.write("p/b.cpp", """
    int
    #ifdef _WIN32
    __declspec(dllexport)
    #endif
    must_export_something;
    """)
    t.write("p/a.h", "")
    t.run_build_system(["p//installx", "p//instally"])
    t.expect_addition("p/xxx/bin/a.exe")
    t.expect_addition("p/xxx/lib/b.dll")
    t.expect_addition("p/xxx/lib/b.lib")
    t.expect_addition("p/xxx/include/a.h")
    t.expect_addition("p/yyy/bin/a.exe")
    t.expect_addition("p/yyy/lib/b.dll")
    t.expect_addition("p/yyy/lib/b.lib")
    t.expect_addition("p/yyy/include/a.h")

def test_paths():
    t = BoostBuild.Tester(pass_toolset=False)
    t.write("Jamroot.jam", """\
    import package ;
    import assert ;
    import os ;
    if [ os.name ] = NT
    {
        default-prefix = "/C:/Test" ;
    }
    else
    {
        default-prefix = /usr/local ;
    }
    paths = [ package.paths Test ] ;
    assert.result $(default-prefix) : $(paths).prefix ;
    assert.result $(default-prefix)/lib : $(paths).libdir ;
    assert.result $(default-prefix)/bin : $(paths).bindir ;
    assert.result $(default-prefix)/include : $(paths).includedir ;
    assert.result $(default-prefix)/share : $(paths).datarootdir ;
    package.add-path-option bardir : bar : libdir ;
    assert.result $(default-prefix)/lib/bar : $(paths).get bardir ;
    """)
    t.run_build_system()
    t.cleanup()

test_defaults()
test_prefix()
test_subdirs()
test_subdirs_with_prefix()
test_prefix_config_file()
test_subdirs_config_file()
test_multiple()
test_paths()
