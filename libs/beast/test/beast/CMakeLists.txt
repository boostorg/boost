#
# Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/boostorg/beast
#

function(build_individual_tests prefix)
    set(options )
    set(oneValueArgs )
    set(multiValueArgs EXCLUDE LIBS EXTRA)
    cmake_parse_arguments(bit "${options}"
            "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    file(GLOB test_srcs "*.cpp")
    set(individual_tests)
    foreach(test_src IN LISTS test_srcs)
        get_filename_component(test_suffix "${test_src}" NAME_WE)
        if(NOT test_suffix IN_LIST bit_EXCLUDE)
            set(test_name "${prefix}-${test_suffix}")
            add_executable("${test_name}" EXCLUDE_FROM_ALL "${test_src}")
            foreach(lib IN LISTS bit_LIBS)
                target_link_libraries("${test_name}" "${lib}")
            endforeach()
            list(APPEND individual_tests COMMAND "${test_name}")
        endif()
    endforeach()

    add_custom_target("${prefix}-all" ${individual_tests})
endfunction()

add_definitions (-DBOOST_BEAST_ALLOW_DEPRECATED)

# For buffers_cat
add_definitions (-DBOOST_BEAST_TESTS)

add_subdirectory (_experimental)
add_subdirectory (core)
add_subdirectory (http)
add_subdirectory (ssl)
add_subdirectory (websocket)
add_subdirectory (zlib)

#-------------------------------------------------------------------------------

GroupSources (include/boost/beast beast)
GroupSources (test/extras/include/boost /)
GroupSources (test/beast /)

add_executable (tests-beast
    ${BOOST_BEAST_FILES}
    ${EXTRAS_FILES}
    Jamfile
    core.cpp
    http.cpp
    ssl.cpp
    version.cpp
    websocket.cpp
    zlib.cpp
)

target_link_libraries(tests-beast
    lib-asio
    lib-asio-ssl
    lib-beast
    lib-test)

set_property(TARGET tests-beast PROPERTY FOLDER "tests")

add_custom_target(tests-beast-all
        DEPENDS
            tests-beast-_experimental-all
            tests-beast-core-all
            tests-beast-http-all
            tests-beast-ssl-all
            tests-beast-websocket-all
            tests-beast-zlib-all )