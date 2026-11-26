#!/usr/bin/env python

# Copyright Andrey Semashev 2024
#
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

# This script scans through git submodules, and for each submodule creates
# an entry in `${BOOST_ROOT}/README.md` with links to the relevant resources,
# including documentation, dependency report and CI status badges. The script
# expects all submodules to be initialized and checked out.
#
# `${BOOST_ROOT}/README.md` should have comments in the following format to mark
# the place to insert the generated content:
#
# (empty line)
# [comment]: # (STATUS_BADGES_BEGIN)
# (empty line)
# [comment]: # (STATUS_BADGES_END)
#
# Here, "(empty line)" denotes an empty line. Everything between those two comments
# will be replaced with the generated content.

import os
import io
import sys
import getopt
import subprocess

# Prints help message
def help():
    print("""Options:
    -h            Print this help.
    -r <path>     Boost superproject root directory. If not specified, deduced from git.
    -f <file>     Markdown file to process. By default, "<BOOST_ROOT>/README.md".
    -b <branches> One or multiple git branches to generate badges for. Multiple branches should be separated
                  with commas. If not specified, "master,develop" is the default.
    -H <depth>    Header depth to use in the generated content. Must be an integer from 1 to 6. By default, 2.
""")

# Git submodule description.
#
# The submodule is identified by a path relative to Boost superproject.
# The submodule may have a number of associated URL templates that correspond to one of the supported
# status badges. Each URL template may contain a {branch} placeholder which will be substituted with
# the git branch for which the table is generated.
# Except git_url, which is mandatory, each of the URLs may be None, in which case the default URL
# template will be generated during filesystem scanning, or an empty string, which means the given
# badge should be disabled, even if the scan would procude an URL. The latter is useful when a certain
# CI service is not operational for a given submodule.
class submodule_info:
    def __init__(self,
        path = None,                # Submodule path relative to superproject
        git_url = None,             # Git repository URL template
        docs_url = None,            # Documentation URL template
        docs_badge_url = None,      # Documentation badge URL template
        gha_url = None,             # GitHub Actions URL template
        gha_badge_url = None,       # GitHub Actions badge URL template. May be a list of multiple URLs if there are more than one CI badge.
        drone_url = None,           # Drone URL template
        drone_badge_url = None,     # Drone badge URL template
        circleci_url = None,        # CircleCI URL template
        circleci_badge_url = None,  # CircleCI badge URL template
        matrix_url = None,          # Boost test matrix URL template
        matrix_badge_url = None,    # Boost test matrix badge URL template
        deps_url = None,            # Dependency report URL template
        deps_badge_url = None       # Dependency report badge URL template
    ):
        self.path = path
        self.git_url = git_url
        self.docs_url = docs_url
        self.docs_badge_url = docs_badge_url
        self.gha_url = gha_url
        self.gha_badge_url = gha_badge_url
        self.drone_url = drone_url
        self.drone_badge_url = drone_badge_url
        self.circleci_url = circleci_url
        self.circleci_badge_url = circleci_badge_url
        self.matrix_url = matrix_url
        self.matrix_badge_url = matrix_badge_url
        self.deps_url = deps_url
        self.deps_badge_url = deps_badge_url

# Submodule overrides that need special handling.
#
# Each entry should map the submodule path to a submodule_info instance to use as a starting point
# for initialization during the filesystem scan. Each URL that is left as None by the entry will be
# initialized by the scanner with the default URL. Each URL that is set in the entry will be left
# unmodified by the filesystem scanner.
#
# If an entry maps to None instead of submodule_info then the corresponding submodule will be omitted
# from the resulting table.
submodule_overrides: dict = {
    # Remove "libs/headers" from the table
    "libs/headers": None,

    "libs/beast": submodule_info(gha_badge_url = "https://github.com/boostorg/beast/actions/workflows/build.yml/badge.svg?branch={branch}"),
    "libs/geometry": submodule_info(gha_badge_url = "https://github.com/boostorg/geometry/actions/workflows/minimal.yml/badge.svg?branch={branch}"),
    "libs/hana": submodule_info(gha_badge_url = "https://github.com/boostorg/hana/actions/workflows/tests.yml/badge.svg?branch={branch}"),
    "libs/histogram": submodule_info(gha_badge_url = "https://github.com/boostorg/histogram/actions/workflows/slow.yml/badge.svg?branch={branch}"),
    "libs/mysql": submodule_info(gha_badge_url = [ "https://github.com/boostorg/mysql/actions/workflows/docker-linux.yml/badge.svg?branch={branch}",
                                                   "https://github.com/boostorg/mysql/actions/workflows/docker-windows.yml/badge.svg?branch={branch}" ]),

    "libs/numeric/conversion": submodule_info(git_url = "https://github.com/boostorg/numeric_conversion/tree/{branch}",
        gha_url = "https://github.com/boostorg/numeric_conversion/actions?query=branch%3A{branch}",
        gha_badge_url = "https://github.com/boostorg/numeric_conversion/actions/workflows/ci.yml/badge.svg?branch={branch}"),
    "libs/numeric/interval": submodule_info(git_url = "https://github.com/boostorg/interval/tree/{branch}",
        gha_url = "https://github.com/boostorg/interval/actions?query=branch%3A{branch}",
        gha_badge_url = "https://github.com/boostorg/interval/actions/workflows/ci.yml/badge.svg?branch={branch}"),
    "libs/numeric/odeint": submodule_info(git_url = "https://github.com/boostorg/odeint/tree/{branch}",
        gha_url = "https://github.com/boostorg/odeint/actions?query=branch%3A{branch}",
        gha_badge_url = "https://github.com/boostorg/odeint/actions/workflows/ci.yml/badge.svg?branch={branch}"),
    "libs/numeric/ublas": submodule_info(git_url = "https://github.com/boostorg/ublas/tree/{branch}",
        gha_url = "https://github.com/boostorg/ublas/actions?query=branch%3A{branch}",
        gha_badge_url = [ "https://github.com/boostorg/ublas/actions/workflows/linux.yml/badge.svg?branch={branch}",
                          "https://github.com/boostorg/ublas/actions/workflows/windows.yml/badge.svg?branch={branch}" ]),

    # Boost.Process CircleCI is not operational
    "libs/process": submodule_info(circleci_url = "", circleci_badge_url = ""),

    "libs/python": submodule_info(gha_badge_url = [ "https://github.com/boostorg/python/actions/workflows/test-ubuntu.yml/badge.svg?branch={branch}",
                                                    "https://github.com/boostorg/python/actions/workflows/test-windows.yml/badge.svg?branch={branch}",
                                                    "https://github.com/boostorg/python/actions/workflows/test-osx.yml/badge.svg?branch={branch}" ]),
    "libs/safe_numerics": submodule_info(gha_badge_url = [ "https://github.com/boostorg/safe_numerics/actions/workflows/posix.yml/badge.svg?branch={branch}",
                                                           "https://github.com/boostorg/safe_numerics/actions/workflows/windows.yml/badge.svg?branch={branch}" ]),
    "libs/serialization": submodule_info(gha_badge_url = [ "https://github.com/boostorg/serialization/actions/workflows/posix.yml/badge.svg?branch={branch}",
                                                           "https://github.com/boostorg/serialization/actions/workflows/windows.yml/badge.svg?branch={branch}" ])
}

class status_badges_generator:
    boost_root_dir = None
    submodules: list = []
    branches: list = ["master", "develop"]
    target_file = None
    header_depth: int = 2

    # Parses command line options
    def read_options(self):
        try:
            opts, values = getopt.getopt(sys.argv[1:], "hr:f:b:H:")
        except getopt.GetoptError as e:
            print(e)
            print("Use -h to show the supported options\n")
            sys.exit(-1)

        for opt, val in opts:
            if opt == "-h":
                help()
                sys.exit(0)

            elif opt == "-r":
                self.boost_root_dir = val

            elif opt == "-f":
                self.target_file = val

            elif opt == "-b":
                self.branches = val.split(",")

            elif opt == "-H":
                self.header_depth = int(val)
                if self.header_depth < 1 or self.header_depth > 6:
                    raise RuntimeError("Invalid header depth: " + val)

        if self.boost_root_dir is None:
            result = subprocess.run(["git", "rev-parse", "--show-superproject-working-tree"], stdout = subprocess.PIPE, text = True)
            self.boost_root_dir = result.stdout.strip()
            if self.boost_root_dir == "":
                result = subprocess.run(["git", "rev-parse", "--show-toplevel"], stdout = subprocess.PIPE, text = True)
                self.boost_root_dir = result.stdout.strip()
            print("Detected Boost superproject root: \"%s\"" % self.boost_root_dir)

        if self.target_file is None:
            self.target_file = os.path.join(self.boost_root_dir, "README.md")

    # Collects the list of git submodules
    def collect_submodules(self):
        os.chdir(self.boost_root_dir)
        result = subprocess.run(["git", "submodule", "status", "--recursive"], stdout = subprocess.PIPE, text = True)
        for line in result.stdout.split("\n"):
            words = line.strip().split(" ")
            if len(words) == 3 and (words[1].startswith("libs/") or words[1].startswith("tools/")):
                submodule_path = words[1].replace("\\", "/")
                submodule = submodule_overrides.get(submodule_path, submodule_info())
                if submodule is not None:
                    submodule.path = submodule_path
                    submodule_path_elems = submodule_path.split("/")
                    submodule_name = submodule_path_elems[1]

                    if submodule.git_url is None:
                        submodule.git_url = "https://github.com/boostorg/" + submodule_name + "/tree/{branch}"
                        for subsubmodule in submodule_path_elems[2:]:
                            submodule.git_url += "/" + subsubmodule

                    if submodule.docs_url is None and os.path.isdir(os.path.join(self.boost_root_dir, submodule.path, "doc")):
                        submodule.docs_url = "https://www.boost.org/doc/libs/{branch}/" + submodule.path
                        if submodule.docs_badge_url is None:
                            submodule.docs_badge_url = "https://img.shields.io/badge/docs-{branch}-blue.svg"

                    if submodule.gha_url is None and os.path.isfile(os.path.join(self.boost_root_dir, submodule.path, ".github/workflows/ci.yml")):
                        submodule.gha_url = "https://github.com/boostorg/" + submodule_name + "/actions?query=branch%3A{branch}"
                        if submodule.gha_badge_url is None:
                            submodule.gha_badge_url = "https://github.com/boostorg/" + submodule_name + "/actions/workflows/ci.yml/badge.svg?branch={branch}"

                    if submodule.drone_url is None and os.path.isfile(os.path.join(self.boost_root_dir, submodule.path, ".drone.star")):
                        submodule.drone_url = "https://drone.cpp.al/boostorg/" + submodule_name + "/branches"
                        if submodule.drone_badge_url is None:
                            submodule.drone_badge_url = "https://drone.cpp.al/api/badges/boostorg/" + submodule_name + "/status.svg?ref=refs/heads/{branch}"

                    if submodule.circleci_url is None and os.path.isfile(os.path.join(self.boost_root_dir, submodule.path, ".circleci/config.yml")):
                        submodule.circleci_url = "https://circleci.com/gh/boostorg/" + submodule_name + "/tree/{branch}"
                        if submodule.circleci_badge_url is None:
                            submodule.circleci_badge_url = "https://circleci.com/gh/boostorg/" + submodule_name + "/tree/{branch}.svg?style=shield"

                    if submodule_path_elems[0] == "libs":
                        if submodule.matrix_url is None:
                            submodule.matrix_url = "https://www.boost.org/development/tests/{branch}/developer/" + submodule_name
                            for subsubmodule in submodule_path_elems[2:]:
                                submodule.matrix_url += "-" + subsubmodule
                            submodule.matrix_url += ".html"
                            if submodule.matrix_badge_url is None:
                                submodule.matrix_badge_url = "https://img.shields.io/badge/matrix-{branch}-blue.svg"

                        if submodule.deps_url is None:
                            submodule.deps_url = "https://pdimov.github.io/boostdep-report/{branch}/" + submodule_name
                            for subsubmodule in submodule_path_elems[2:]:
                                submodule.deps_url += "~" + subsubmodule
                            submodule.deps_url += ".html"
                            if submodule.deps_badge_url is None:
                                submodule.deps_badge_url = "https://img.shields.io/badge/deps-{branch}-blue.svg"

                    if submodule.docs_url or submodule.gha_url or submodule.drone_url or submodule.matrix_url or submodule.deps_url:
                        self.submodules.append(submodule)

        self.submodules.sort(key = lambda submodule: submodule.path)

    # Parses Markdown file and inserts status badge tables
    def process_markdown_file(self):
        os.replace(self.target_file, self.target_file + ".bak")
        with open(self.target_file + ".bak", "r") as in_file, open(self.target_file, "w") as out_file:
            skipping_content: bool = False
            last_line_was_empty: bool = True
            for line in in_file.readlines():
                stripped_line = line.strip()
                if last_line_was_empty:
                    if not skipping_content and stripped_line == "[comment]: # (STATUS_BADGES_BEGIN)":
                        out_file.write(line) # preserve the markup in the output file to make the script re-runnable
                        skipping_content = True
                        self.generate_status_badges(out_file)
                    elif skipping_content and stripped_line == "[comment]: # (STATUS_BADGES_END)":
                        out_file.write("\n") # write the empty line that was there in the input file previously
                        skipping_content = False
                if not skipping_content:
                    out_file.write(line)
                last_line_was_empty = stripped_line == ""

    # Writes status badge tables in the output file
    def generate_status_badges(self, out_file):
        for branch in self.branches:
            out_file.write("\n" + ("#" * self.header_depth) + " Build status for `" + branch +
                "` branch\n\nSubmodule | Docs | GHA | Drone | CircleCI | Matrix | Deps |\n--------- | ---- | -------------- | ----- | -------- | ----------- | ---- |\n")

            for submodule in self.submodules:
                row: str = "[`" + submodule.path + "`](" + submodule.git_url.format(branch = branch) + ") | "

                if submodule.docs_url:
                    row += "[![Docs](" + submodule.docs_badge_url.format(branch = branch) + ")](" + submodule.docs_url.format(branch = branch) + ") | "
                else:
                    row += "&mdash; |"

                if submodule.gha_url:
                    if isinstance(submodule.gha_badge_url, list):
                        first: bool = True
                        for badge_url in submodule.gha_badge_url:
                            if first:
                                first = False
                            else:
                                row += "<br/>"
                            row += "[![GHA](" + badge_url.format(branch = branch) + ")](" + submodule.gha_url.format(branch = branch) + ")"
                        row += " | "
                    else:
                        row += "[![GHA](" + submodule.gha_badge_url.format(branch = branch) + ")](" + submodule.gha_url.format(branch = branch) + ") | "
                else:
                    row += "&mdash; |"

                if submodule.drone_url:
                    row += "[![Drone](" + submodule.drone_badge_url.format(branch = branch) + ")](" + submodule.drone_url.format(branch = branch) + ") | "
                else:
                    row += "&mdash; |"

                if submodule.circleci_url:
                    row += "[![CircleCI](" + submodule.circleci_badge_url.format(branch = branch) + ")](" + submodule.circleci_url.format(branch = branch) + ") | "
                else:
                    row += "&mdash; |"

                if submodule.matrix_url:
                    row += "[![Matrix](" + submodule.matrix_badge_url.format(branch = branch) + ")](" + submodule.matrix_url.format(branch = branch) + ") | "
                else:
                    row += "&mdash; |"

                if submodule.deps_url:
                    row += "[![Deps](" + submodule.deps_badge_url.format(branch = branch) + ")](" + submodule.deps_url.format(branch = branch) + ")\n"
                else:
                    row += "&mdash;\n"

                out_file.write(row)

def main():
    app = status_badges_generator()
    app.read_options()
    app.collect_submodules()
    app.process_markdown_file()

if __name__ == "__main__":
    main()
