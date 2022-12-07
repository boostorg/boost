#!/usr/bin/python

# Copyright 2017 Steven Watanabe
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# Tests configure.check-target-builds and friends

import BoostBuild

def test_check_target_builds():
    t = BoostBuild.Tester(use_test_config=0)
    t.write("Jamroot", """
import configure ;
obj pass : pass.cpp ;
obj fail : fail.cpp ;
explicit pass fail ;
obj foo : foo.cpp :
  [ configure.check-target-builds pass : <define>PASS : <define>FAIL ] ;
obj bar : foo.cpp :
  [ configure.check-target-builds fail : <define>FAIL : <define>PASS ] ;
""")
    t.write("pass.cpp", "void f() {}\n")
    t.write("fail.cpp", "#error fail.cpp\n")
    t.write("foo.cpp", """
#ifndef PASS
#error PASS not defined
#endif
#ifdef FAIL
#error FAIL is defined
#endif
""")
    t.run_build_system()
    t.expect_output_lines([
        "    - pass builds              : yes*",
        "    - fail builds              : no*"])
    t.expect_addition("bin/$toolset/debug*/pass.obj")
    t.expect_addition("bin/$toolset/debug*/foo.obj")
    t.expect_addition("bin/$toolset/debug*/bar.obj")
    t.expect_nothing_more()

    # An up-to-date build should use the cache
    t.run_build_system()
    t.expect_output_lines([
        "    - pass builds              : yes (cached)*",
        "    - fail builds              : no  (cached)*"])
    t.expect_nothing_more()

    # -a should re-run everything, including configuration checks
    t.run_build_system(["-a"])
    t.expect_output_lines([
        "    - pass builds              : yes*",
        "    - fail builds              : no*"])
    t.expect_touch("bin/$toolset/debug*/pass.obj")
    t.expect_touch("bin/$toolset/debug*/foo.obj")
    t.expect_touch("bin/$toolset/debug*/bar.obj")
    t.expect_nothing_more()

    # --reconfigure should re-run configuration checks only
    t.run_build_system(["--reconfigure"])
    t.expect_output_lines([
        "    - pass builds              : yes*",
        "    - fail builds              : no*"])
    t.expect_touch("bin/$toolset/debug*/pass.obj")
    t.expect_nothing_more()

    # -a -n should not rebuild configuration checks
    t.run_build_system(["-a", "-n"])
    t.expect_output_lines([
        "    - pass builds              : yes (cached)*",
        "    - fail builds              : no  (cached)*"])
    t.expect_nothing_more()

    # --clean-all should clear all configuration checks
    t.run_build_system(["--clean-all"])
    t.expect_output_lines([
        "    - pass builds              : yes (cached)*",
        "    - fail builds              : no  (cached)*"])
    t.expect_removal("bin/$toolset/debug*/pass.obj")
    t.expect_removal("bin/$toolset/debug*/foo.obj")
    t.expect_removal("bin/$toolset/debug*/bar.obj")
    t.expect_nothing_more()

    # If configuration checks are absent, then --clean-all
    # should create them and then delete them again.  This
    # currently fails because clean cannot remove targets
    # that were created in the same build.
    #t.run_build_system(["--clean-all"])
    #t.expect_output_lines([
    #    "    - pass builds              : yes",
    #    "    - fail builds              : no"])
    #t.expect_nothing_more()

    # Just verify that we're actually in the initial
    # state here.
    t.run_build_system()
    t.expect_output_lines([
        "    - pass builds              : yes*",
        "    - fail builds              : no*"])
    t.expect_addition("bin/$toolset/debug*/pass.obj")
    t.expect_addition("bin/$toolset/debug*/foo.obj")
    t.expect_addition("bin/$toolset/debug*/bar.obj")
    t.expect_nothing_more()

    t.cleanup()

def test_choose():
    t = BoostBuild.Tester(use_test_config=0)
    t.write("Jamroot", """
import configure ;
obj pass : pass.cpp ;
obj fail : fail.cpp ;
explicit pass fail ;
obj foo : foo.cpp :
  [ configure.choose "which one?" : fail <define>FAIL : pass <define>PASS ] ;
""")
    t.write("pass.cpp", "void f() {}\n")
    t.write("fail.cpp", "#error fail.cpp\n")
    t.write("foo.cpp", """
#ifndef PASS
#error PASS not defined
#endif
#ifdef FAIL
#error FAIL is defined
#endif
""")
    t.run_build_system()
    t.expect_output_lines([
        "    - which one?               : pass*"])
    t.expect_addition("bin/$toolset/debug*/pass.obj")
    t.expect_addition("bin/$toolset/debug*/foo.obj")
    t.expect_nothing_more()

    # An up-to-date build should use the cache
    t.run_build_system()
    t.expect_output_lines([
        "    - which one?               : pass (cached)*"])
    t.expect_nothing_more()

    # -a should re-run everything, including configuration checks
    t.run_build_system(["-a"])
    t.expect_output_lines([
        "    - which one?               : pass*"])
    t.expect_touch("bin/$toolset/debug*/pass.obj")
    t.expect_touch("bin/$toolset/debug*/foo.obj")
    t.expect_nothing_more()

    # --reconfigure should re-run configuration checks only
    t.run_build_system(["--reconfigure"])
    t.expect_output_lines([
        "    - which one?               : pass*"])
    t.expect_touch("bin/$toolset/debug*/pass.obj")
    t.expect_nothing_more()

    # -a -n should not rebuild configuration checks
    t.run_build_system(["-a", "-n"])
    t.expect_output_lines([
        "    - which one?               : pass (cached)*"])
    t.expect_nothing_more()

    # --clean-all should clear all configuration checks
    t.run_build_system(["--clean-all"])
    t.expect_output_lines([
        "    - which one?               : pass (cached)*"])
    t.expect_removal("bin/$toolset/debug*/pass.obj")
    t.expect_removal("bin/$toolset/debug*/foo.obj")
    t.expect_nothing_more()

    # If configuration checks are absent, then --clean-all
    # should create them and then delete them again.  This
    # currently fails because clean cannot remove targets
    # that were created in the same build.
    #t.run_build_system(["--clean-all"])
    #t.expect_output_lines([
    #    "    - which one?               : pass"])
    #t.expect_nothing_more()

    # Just verify that we're actually in the initial
    # state here.
    t.run_build_system()
    t.expect_output_lines([
        "    - which one?               : pass*"])
    t.expect_addition("bin/$toolset/debug*/pass.obj")
    t.expect_addition("bin/$toolset/debug*/foo.obj")
    t.expect_nothing_more()

    t.cleanup()

def test_translation():
    """Tests scoping for targets, paths, and rules within check-target-builds"""
    t = BoostBuild.Tester(use_test_config=0)
    t.write("Jamroot", "")
    t.write("subdir/Jamfile", """
import configure ;
obj pass : pass.cpp ;
obj fail : fail.cpp ;
explicit pass fail ;
obj foo : :
  [ configure.check-target-builds pass
    : [ configure.check-target-builds fail : <define>FAIL
        : <define>PASS <include>include1 <conditional>@c1 ]
    : <define>FAIL ] ;
obj bar : :
  [ configure.choose "which one?" : pass
     [ configure.choose "Try again?" : pass
        <define>PASS <include>include1 <conditional>@c1 ] ] ;
rule c1 ( properties * )
{
  return <include>include2 <source>foo.cpp ;
}
""")
    t.write("subdir/include1/a.h", "")
    t.write("subdir/include2/b.h", "")
    t.write("subdir/pass.cpp", "void f() {}\n")
    t.write("subdir/fail.cpp", "#error fail.cpp\n")
    t.write("subdir/foo.cpp", """
#include <a.h>
#include <b.h>
#ifndef PASS
#error PASS not defined
#endif
#ifdef FAIL
#error FAIL is defined
#endif
""")
    t.run_build_system(["subdir"])
    t.expect_output_lines([
        "    - pass builds              : yes*",
        "    - fail builds              : no*"])
    t.expect_addition("subdir/bin/$toolset/debug*/pass.obj")
    t.expect_addition("subdir/bin/$toolset/debug*/foo.obj")
    t.expect_addition("subdir/bin/$toolset/debug*/bar.obj")
    t.expect_nothing_more()
    t.cleanup()

def test_choose_none():
    """Tests choose when none of the alternatives match."""
    t = BoostBuild.Tester(use_test_config=0)
    t.write("Jamroot", """
import configure ;
obj fail : fail.cpp ;
explicit pass fail ;
obj foo : foo.cpp :
  [ configure.choose "which one?" : fail <define>FAIL ] ;
""")
    t.write("fail.cpp", "#error fail.cpp\n")
    t.write("foo.cpp", """
#ifdef FAIL
#error FAIL is defined
#endif
""")
    t.run_build_system()
    t.expect_output_lines([
        "    - which one?               : none*"])

    # An up-to-date build should use the cache
    t.run_build_system()
    t.expect_output_lines([
        "    - which one?               : none (cached)*"])
    t.expect_nothing_more()
    t.cleanup()

test_check_target_builds()
test_choose()
test_translation()
test_choose_none()
