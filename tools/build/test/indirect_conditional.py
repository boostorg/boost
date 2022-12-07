#!/usr/bin/python

# Copyright (C) 2006. Vladimir Prus
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

import BoostBuild

def test_basic():
    t = BoostBuild.Tester(use_test_config=False)

    t.write("jamroot.jam", """\
exe a1 : a1.cpp : <conditional>@a1-rule ;
rule a1-rule ( properties * )
{
    if <variant>debug in $(properties)
    {
        return <define>OK ;
    }
}

exe a2 : a2.cpp : <conditional>@$(__name__).a2-rule
    <variant>debug:<optimization>speed ;
rule a2-rule ( properties * )
{
    if <optimization>speed in $(properties)
    {
        return <define>OK ;
    }
}

exe a3 : a3.cpp :
    <conditional>@$(__name__).a3-rule-1
    <conditional>@$(__name__).a3-rule-2 ;
rule a3-rule-1 ( properties * )
{
    if <optimization>speed in $(properties)
    {
        return <define>OK ;
    }
}
rule a3-rule-2 ( properties * )
{
    if <variant>debug in $(properties)
    {
        return <optimization>speed ;
    }
}
""")

    t.write("a1.cpp", "#ifdef OK\nint main() {}\n#endif\n")
    t.write("a2.cpp", "#ifdef OK\nint main() {}\n#endif\n")
    t.write("a3.cpp", "#ifdef OK\nint main() {}\n#endif\n")

    t.run_build_system()

    t.expect_addition("bin/$toolset/debug*/a1.exe")
    t.expect_addition("bin/$toolset/debug/optimization-speed*/a2.exe")
    t.expect_addition("bin/$toolset/debug/optimization-speed*/a3.exe")

    t.cleanup()

def test_inherit():
    """Tests that paths etc. are handled correctly when an indirect
    conditional is inherited by a subproject."""
    t = BoostBuild.Tester(use_test_config=False)
    t.write("Jamroot.jam", """
import feature ;
import indirect ;
exe d1 : d1.cpp ;
explicit d1 ;
project : requirements <conditional>@c1 ;
build-project subdir ;
feature.feature myrule : : free ;
rule c1 ( properties * )
{
  return <dependency>d1 <include>include <myrule>@parent-generate ;
}
rule parent-generate ( project name : property-set : sources * )
{
  return $(sources) ;
}
rule my-generate ( project name : property-set : sources * )
{
  local r = [ $(property-set).get <myrule> ] ;
  r = [ MATCH @(.*) : $(r) ] ;
  return [ indirect.call
    $(r) $(project) $(name) : $(property-set) : $(sources) ] ;
}
""")
    t.write("d1.cpp", "int main(){}\n")
    t.write("subdir/Jamfile", """
generate srcs : main.cpp : <generating-rule>@my-generate ;
exe main : srcs ;
""")
    t.write("include/a.h", "")
    t.write("subdir/main.cpp", "#include <a.h>\nint main() {}\n")
    t.run_build_system()
    t.expect_addition("bin/$toolset/debug*/d1.obj")
    t.expect_addition("bin/$toolset/debug*/d1.exe")
    t.ignore_addition("bin/*/d1.rsp")
    t.expect_addition("subdir/bin/$toolset/debug*/main.obj")
    t.expect_addition("subdir/bin/$toolset/debug*/main.exe")
    t.ignore_addition("subdir/bin/*/main.rsp")
    t.expect_nothing_more()
    t.cleanup()

def test_glob_in_indirect_conditional():
    """
      Regression test: project-rules.glob rule run from inside an indirect
    conditional should report an error as it depends on the 'currently loaded
    project' concept and indirect conditional rules get called only after all
    the project modules have already finished loading.

    """
    t = BoostBuild.Tester(use_test_config=False)

    t.write("jamroot.jam", """\
use-project /library-example/foo : util/foo ;
build-project app ;
""")
    t.write("app/app.cpp", "int main() {}\n");
    t.write("app/jamfile.jam", "exe app : app.cpp /library-example/foo//bar ;")
    t.write("util/foo/bar.cpp", """\
#ifdef _WIN32
__declspec(dllexport)
#endif
void foo() {}
""")
    t.write("util/foo/jamfile.jam", """\
rule print-my-sources ( properties * )
{
    ECHO My sources: ;
    ECHO [ glob *.cpp ] ;
}
lib bar : bar.cpp : <conditional>@print-my-sources ;
""")

    t.run_build_system(status=1)
    t.expect_output_lines(["My sources:", "bar.cpp"], False)
    t.expect_output_lines("error: Reference to the project currently being "
        "loaded requested when there was no project module being loaded.")

    t.cleanup()


test_basic()
test_inherit()
test_glob_in_indirect_conditional()
