#!/usr/bin/python
#
# Copyright 2017 Steven Watanabe
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.txt or copy at
# https://www.bfgroup.xyz/b2/LICENSE.txt)

# validates a toolset using a mock of the compiler

import BoostBuild
import os
import re
import sys

renames = {"debug": "variant=debug", "release": "variant=release"}

def set_default_target_os(os):
    global removed
    global default_target_os
    default_target_os = os
    removed = set()
    removed.add("target-os=" + default_target_os)

def adjust_property(property):
    global renames
    if property in renames:
        return renames[property]
    else:
        return property

def adjust_properties(properties):
    global removed
    return [adjust_property(p) for p in properties if p not in removed]

def has_property(name, properties):
    return name in [re.sub("=.*", "", p) for p in properties]

def get_property(name, properties):
    for m in [re.match("(.*)=(.*)", p) for p in properties]:
        if m and m.group(1) == name:
            return m.group(2)

def get_target_os(properties):
    return get_property("target-os", properties) or default_target_os

def expand_properties(properties, toolset):
    result = properties[:]
    if not has_property("variant", properties):
        result += ["variant=debug"]
    if not has_property("threading", properties):
        result += ["threading=single"]
    if not has_property("exception-handling", properties):
        result += ["exception-handling=on"]
    if not has_property("link", properties):
        result += ["link=shared"]
    if not has_property("rtti", properties):
        result += ["rtti=on"]
    if not has_property("runtime-link", properties):
        result += ["runtime-link=shared"]
    if toolset == "msvc" and "runtime-link=shared" in result:
        result.remove("threading=" + get_property("threading", result))
        result += ["threading=multi"]
    if not has_property("strip", properties):
        result += ["strip=off"]
    if not has_property("target-os", properties):
        result += ["target-os=" + default_target_os]
    if not has_property("windows-api", properties):
        result += ["windows-api=desktop"]
    return result

def compute_path(properties, target_type):
    path = ""
    if "variant=release" in properties:
        path += "/release"
    else:
        path += "/debug"
    if has_property("address-model", properties):
        path += "/address-model-" + get_property("address-model", properties)
    if has_property("architecture", properties):
        path += "/architecture-" + get_property("architecture", properties)
    if "cxxstd=latest" in properties:
        path += "/cxxstd-latest-iso"
    if "exception-handling=off" in properties:
        path += "/exception-handling-off"
    if "link=static" in properties:
        path += "/link-static"
    if "rtti=off" in properties:
        path += "/rtti-off"
    if "runtime-link=static" in properties: # and target_type in ["exe"]:
        path += "/runtime-link-static"
    if "strip=on" in properties and target_type in ["dll", "exe", "obj2"]:
        path += "/strip-on"
    if get_target_os(properties) != default_target_os:
        path += "/target-os-" + get_target_os(properties)
    if "threading=multi" in properties:
      if "runtime-link=static" not in properties: # TODO: I don't think this it's intended to work this way though
        path += "/threading-multi"
    if not "windows-api=desktop" in properties:
        path += "/windows-api-" + get_property("windows-api", properties)
    return path

def test_toolset(toolset, version, property_sets):
    t = BoostBuild.Tester()

    t.set_tree("toolset-mock")

    # Build necessary tools
    t.run_build_system(["-sPYTHON_CMD=%s" % sys.executable], subdir="src")
    set_default_target_os(t.read("src/bin/target-os.txt").strip())

    for properties in property_sets:
        t.set_toolset(toolset + "-" + version, get_target_os(properties))
        properties = adjust_properties(properties)
        expanded_properties = expand_properties(properties, toolset)
        def path(t):
            return toolset.split("-")[0] + "-*" + version + compute_path(expanded_properties, t)
        os.environ["B2_PROPERTIES"] = " ".join(expanded_properties)
        t.run_build_system(["--user-config=", "-sPYTHON_CMD=%s" % sys.executable] + properties)
        t.expect_addition("bin/%s/lib.obj" % (path("obj")))
        if "link=static" not in properties:
            if get_target_os(properties) in ["cygwin", "windows"] and toolset != "clang-linux":
                t.expect_addition("bin/%s/l1.implib" % (path("dll")))
            t.expect_addition("bin/%s/l1.dll" % (path("dll")))
            t.ignore_addition("bin/%s/*l1.*.rsp" % (path("dll")))
        else:
            t.expect_addition("bin/%s/l1.lib" % (path("lib")))
        t.expect_addition("bin/%s/main.obj" % (path("obj2")))
        t.expect_addition("bin/%s/test.exe" % (path("exe")))
        t.ignore_addition("bin/%s/test.rsp" % (path("exe")))
        t.expect_nothing_more()
        t.rm("bin")

    t.cleanup()
