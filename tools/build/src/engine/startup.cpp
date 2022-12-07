/*
Copyright 2020-2022 René Ferdinand Rivera Morell
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.txt or https://www.bfgroup.xyz/b2/LICENSE.txt)
*/

#include "startup.h"
#include "rules.h"
#include "frames.h"
#include "object.h"
#include "pathsys.h"
#include "cwd.h"
#include "filesys.h"
#include "output.h"
#include "variable.h"

#include <cstdlib>
#include <string>
#include <algorithm>

namespace
{
    void bind_builtin(
        char const *name_, LIST *(*f)(FRAME *, int flags),
        int flags, char const **args)
    {
        FUNCTION *func;
        OBJECT *name = object_new(name_);
        func = function_builtin(f, flags, args);
        new_rule_body(root_module(), name, func, 1);
        function_free(func);
        object_free(name);
    }
} // namespace

void b2::startup::load_builtins()
{
    {
        char const *args[] = {"dir", "?", 0};
        bind_builtin("boost-build", builtin_boost_build, 0, args);
    }
}

LIST *b2::startup::builtin_boost_build(FRAME *frame, int flags)
{
    // Do nothing, but keep the rule, for backwards compatability.
    // But do record the path passed in as a fallback to the loading.
    b2::jam::variable(".boost-build-dir") = b2::jam::list(lol_get(frame->args, 0));
    return L0;
}

extern char const *saved_argv0;

bool b2::startup::bootstrap(FRAME *frame)
{
    b2::jam::list ARGV = b2::jam::variable{"ARGV"};
    b2::jam::object opt_debug_configuration{"--debug-configuration"};
    b2::jam::variable dot_OPTION__debug_configuration{".OPTION", "debug-configration"};
    for (auto arg: ARGV)
    {
        if (opt_debug_configuration == arg)
        {
            dot_OPTION__debug_configuration = b2::jam::list{b2::jam::object{"true"}};
            break;
        }
    }

    // We use the executable path as a root for searches.
    char *b2_exe_path_pchar = executable_path(saved_argv0);
    const std::string b2_exe_path{b2_exe_path_pchar};
    if (b2_exe_path_pchar)
    {
        std::free(b2_exe_path_pchar);
    }

    /**
     * Search for a boost-build.jam file to load in various locations. This is
     * solely for backwards compatiblity. The boost-build.jam file found is
     * loaded, but the `boost-build` invocation in it is ignored.
     */

    const std::string boost_build_jam{"boost-build.jam"};
    std::string b2_file_path;

    // Attempt to find the `boost-build.jam` boot file in work directory tree.
    // I.e. in current directory and ancestor directories.
    if (b2_file_path.empty())
    {
        std::string work_dir{b2::paths::normalize(b2::cwd_str()) + "/"};
        while (b2_file_path.empty() && !work_dir.empty())
        {
            if (b2::filesys::is_file(work_dir + boost_build_jam))
                b2_file_path = work_dir + boost_build_jam;
            else if (work_dir.length() == 1 && work_dir[0] == '/')
                work_dir.clear();
            else
            {
                auto parent_pos = work_dir.rfind('/', work_dir.length() - 2);
                if (parent_pos != std::string::npos)
                    work_dir.erase(parent_pos + 1);
                else
                    work_dir.clear();
            }
        }
    }

    // Check relative to the executable for portable install location.
    // ~b2(.exe)/.b2/kernel/boost-build.jam
    if (b2_file_path.empty())
    {
        const std::string path{
            b2::paths::normalize(
                b2_exe_path + "/../.b2/kernel/" + boost_build_jam)};
        if (b2::filesys::is_file(path))
            b2_file_path = path;
    }

    // Check relative to the executable for portable install location.
    // /usr/share/b2/src/kernel/boost-build.jam
    if (b2_file_path.empty())
    {
        const std::string path{
            b2::paths::normalize(
                b2_exe_path + "/../../share/b2/src/kernel/" + boost_build_jam)};
        if (b2::filesys::is_file(path))
            b2_file_path = path;
    }

    // Check the BOOST_BUILD_PATH (and BOOST_ROOT) paths.
    // $(BOOST_BUILD_PATH)/boost-build.jam
    // $(BOOST_ROOT)/boost-build.jam
    if (b2_file_path.empty())
    {
        b2::jam::list BOOST_BUILD_PATH = b2::jam::variable{"BOOST_BUILD_PATH"};
        // For back-compat with Boost we also search in the BOOST_ROOT location.
        BOOST_BUILD_PATH.append(b2::jam::list(b2::jam::variable{"BOOST_ROOT"}));
        for (auto search_path: BOOST_BUILD_PATH)
        {
            std::string path = b2::jam::object{search_path};
            path = b2::paths::normalize(path+"/"+boost_build_jam);
            if (b2::filesys::is_file(path))
            {
                b2_file_path = path;
                break;
            }
        }
    }

    // Show where we found it, if asked.
    if (!b2_file_path.empty() && dot_OPTION__debug_configuration)
    {
        out_printf("notice: found boost-build.jam at %s\n", b2_file_path.c_str());
    }

    // Load the boost-build file if we find it for backwards compatability. We ignore
    // the `boost-build ..` invocation it does. Preferring to find our own bootstrap
    // engine file.
    if (!b2_file_path.empty())
    {
        b2::jam::variable dot_boost_build_file{".boost-build-file"};
        dot_boost_build_file = b2_file_path;
        b2::jam::object b2_file_path_sym{b2_file_path};
        parse_file(b2_file_path_sym, frame);
    }

    /**
     * Search for a bootstrap.jam file to load in various locations. The
     * `bootstrap.jam` is the starting point of loading the build system.
     */

    const std::string bootstrap_jam{"bootstrap.jam"};
    std::string bootstrap_file;
    std::string bootstrap_files_searched;

    // Check various locations relative to executable.
    if (bootstrap_file.empty())
    {
        const char * dirs[] = {
            // Check relative to the executable for portable install location.
            ".b2/kernel/",
            // Check relative to the exec for system install location.
            "../share/b2/kernel/",
            // Check relative to the exec for legacy install location.
            "../share/b2/src/kernel/",
            // Check development location relative to executable in src/engine.
            "../kernel/",
            // Check development location relative to executable at root.
            "src/kernel/",
            // Check for special Boost location.
            "tools/build/src/kernel/"
        };
        for (auto dir: dirs)
        {
            const std::string path{
                b2::paths::normalize(
                    b2_exe_path + "/../" + dir + bootstrap_jam)};
            if (b2::filesys::is_file(path))
            {
                bootstrap_file = path;
                break;
            }
            bootstrap_files_searched += "  " + path + "\n";
        }
    }

    // Check the development tree for the bootstrap to support not-installed
    // b2 executable locations. I.e. when building b2 with b2 for engine
    // development.
    if (bootstrap_file.empty())
    {
        std::string work_dir(b2::paths::normalize(b2_exe_path + "/..") + "/");
        while (bootstrap_file.empty() && !work_dir.empty())
        {
            bootstrap_files_searched += "  " +  work_dir + "src/kernel/" + bootstrap_jam + "\n";
            if (b2::filesys::is_file(work_dir + "src/kernel/" + bootstrap_jam))
                bootstrap_file = work_dir + "src/kernel/" + bootstrap_jam;
            else if (work_dir.length() == 1 && work_dir[0] == '/')
                work_dir.clear();
            else
            {
                auto parent_pos = work_dir.rfind('/', work_dir.length() - 2);
                if (parent_pos != std::string::npos)
                    work_dir.erase(parent_pos + 1);
                else
                    work_dir.clear();
            }
        }
    }

    // Last resort, search in the directory referenced by the boost-build rule.
    if (bootstrap_file.empty())
    {
        b2::jam::list dot_boost_build_dir
            = b2::jam::variable(".boost-build-dir");
        if (!dot_boost_build_dir.empty())
        {
            std::string dir = b2::jam::object(*dot_boost_build_dir.begin());
            if (!b2_file_path.empty() && !b2::paths::is_rooted(dir))
                dir = b2_file_path + "/../" + dir;
            const std::string path
                = b2::paths::normalize(dir + "/" + bootstrap_jam);
            bootstrap_files_searched += "  " + path + "\n";
            if (b2::filesys::is_file(path))
                bootstrap_file = path;
        }
    }

    // Failed to find the build files to load.
    if (bootstrap_file.empty())
    {
        err_printf(
            "Unable to load B2\n"
            "-----------------\n"
            "No 'bootstrap.jam' was found by searching for:\n"
            "%s\n"
            "Please consult the documentation at "
            "'https://www.bfgroup.xyz/b2/'.\n\n",
            bootstrap_files_searched.c_str());
        return false;
    }

    // Show where we found the bootstrap, if asked.
    if (!bootstrap_file.empty() && dot_OPTION__debug_configuration)
    {
        out_printf("notice: loading B2 from %s\n", bootstrap_file.c_str());
    }

    // Set the bootstrap=file var as it's used by the build system to refer to
    // the rest of the build system files.
    b2::jam::variable dot_bootstrap_file{".bootstrap-file"};
    dot_bootstrap_file = b2::jam::list{b2::jam::object{bootstrap_file}};

    // Load the build system bootstrap file we found.
    parse_file(b2::jam::object{bootstrap_file}, frame);

    return true;
}
