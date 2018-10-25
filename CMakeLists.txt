# Copyright 2018 Mike Dev
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.5)
# NOTE: Individual boost cmake files might require a higher cmake version

project(boost LANGUAGES CXX)

#=== options ===

# exclude libraries that don't work with the superproject yet
# (e.g. due to missing dependencies or conflicting target definitions)

set(boost_main_excluded_libs dummy1;dummy2;dummy3 CACHE string "List of libraries that will be excluded from cmake build")


#~~~ options ~~~


message("
================================ BOOST WARNING ===================================================
=                                                                                                =
= General cmake support in boost is EXPERIMENTAL at best and                                     =
= only a few libraries can be build via cmake at all.                                            =
=                                                                                                =
= The officially supported way to build and test boost is still b2, as e.g. explained here:      =
= https://www.boost.org/doc/libs/1_68_0/more/getting_started/index.html                          =
=                                                                                                =
==================================================================================================
")

message("Manually excluded libs: ${boost_main_excluded_libs}")

# cmake doesn't require autolinking and currently most cmake files don't produce
# name mangled libraries anyway
message("Deactivating boost auto linking mechanism (-DBOOST_ALL_NO_LIB)")
add_definitions(-DBOOST_ALL_NO_LIB)

# Detect and process all CMakeLists files that reside in the root folder of a library
file(GLOB boost_libs_with_cmake_files libs/*/CMakeLists.txt)

foreach(cmake_file IN ITEMS ${boost_libs_with_cmake_files})

    get_filename_component(dir ${cmake_file} DIRECTORY)
    get_filename_component(lib_name ${dir} NAME)

    if(lib_name IN_LIST boost_main_excluded_libs)
        message("Ignoring boost library: ${dir}")
    else()
        message("Adding boost library: ${dir}")
        add_subdirectory(${dir})
    endif()

endforeach()
