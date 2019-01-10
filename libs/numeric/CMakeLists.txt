# Copyright 2018 Mike Dev
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

file(GLOB boost_numeric_libs_with_cmake_files */CMakeLists.txt)

foreach(cmake_file IN LISTS boost_numeric_libs_with_cmake_files)

    get_filename_component(dir ${cmake_file} DIRECTORY)
    get_filename_component(lib_name ${dir} NAME)

    if(lib_name IN_LIST BOOST_MAIN_IGNORE_LIBS OR "numeric/${lib_name}" IN_LIST BOOST_MAIN_IGNORE_LIBS)
        message("Ignoring boost library: ${dir}")
    else()
        message("Adding boost library: ${dir}")
        add_subdirectory(${dir})
    endif()

endforeach()

