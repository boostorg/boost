# Copyright 2018 Mike Dev
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.5)
# NOTE: Individual boost cmake files might require a higher cmake version

project(boost LANGUAGES CXX)

#=== options ===

# Some libraries' cmake files don't work well with this cmake file, e.g. because
# - they are generally not designed to support the add_subdirectory workflow at all
# - they define targets with conflicting names (e.g. check)
# - require some additional (internal or external) dependencies
#
# Those libraries can be excluded here
set(BOOST_MAIN_IGNORE_LIBS callable_traits;compute;gil;hana;yap;safe_numerics;beast CACHE string "List of libraries that will be excluded from cmake build")


#~~~ options ~~~

message("
================================ BOOST WARNING ===================================================
=                                                                                                =
= General cmake support in boost is EXPERIMENTAL at best and                                     =
= only a few libraries can be build via cmake at all.                                            =
=                                                                                                =
= The officially supported way to build and test boost is still b2, as e.g. explained here:      =
= https://www.boost.org/doc/libs/1_69_0/more/getting_started/index.html                          =
=                                                                                                =
==================================================================================================
")

message("Excluded libs (BOOST_MAIN_IGNORE_LIBS): ${BOOST_MAIN_IGNORE_LIBS}")

# cmake doesn't require autolinking and currently most cmake files don't produce
# name mangled libraries anyway
message("Deactivating boost auto linking mechanism (-DBOOST_ALL_NO_LIB)")
add_definitions(-DBOOST_ALL_NO_LIB)

# Detect and process all CMakeLists files that reside in the root folder of a library
file(GLOB boost_libs_with_cmake_files libs/*/CMakeLists.txt)

foreach(cmake_file IN LISTS boost_libs_with_cmake_files)

    get_filename_component(dir ${cmake_file} DIRECTORY)
    get_filename_component(lib_name ${dir} NAME)

    if(lib_name IN_LIST BOOST_MAIN_IGNORE_LIBS)
        message("Ignoring boost library: ${dir}")
    else()
        message("Adding boost library: ${dir}")
        add_subdirectory(${dir})
    endif()

endforeach()
