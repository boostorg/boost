# Copyright 2018, 2019 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

# Include BoostTest outside the include guard for it to clear its global variables
include(BoostTest)

if(NOT CMAKE_VERSION VERSION_LESS 3.10)
  include_guard()
endif()

if(BUILD_TESTING AND CMAKE_VERSION VERSION_LESS 3.9)
  message(AUTHOR_WARNING "BoostTestJamfile requires CMake 3.9") # CMAKE_MATCH_x
endif()

include(BoostMessage)

# boost_test_jamfile( FILE jamfile [PREFIX prefix]
#   LINK_LIBRARIES libs...
#   COMPILE_DEFINITIONS defs...
#   COMPILE_OPTIONS opts...
#   COMPILE_FEATURES features...
# )

function(boost_test_jamfile)

  cmake_parse_arguments(_ "" "FILE;PREFIX" "LIBRARIES;LINK_LIBRARIES;COMPILE_DEFINITIONS;COMPILE_OPTIONS;COMPILE_FEATURES" ${ARGN})

  if(__UNPARSED_ARGUMENTS)
    message(AUTHOR_WARNING "boost_test_jamfile: extra arguments ignored: ${__UNPARSED_ARGUMENTS}")
  endif()

  if(__LIBRARIES)
    message(AUTHOR_WARNING "boost_test_jamfile: LIBRARIES is deprecated, use LINK_LIBRARIES")
  endif()

  if(NOT __FILE)
    message(AUTHOR_WARNING "boost_test_jamfile: required argument FILE is missing")
    return()
  endif()

  if(DEFINED BUILD_TESTING AND NOT BUILD_TESTING)
    return()
  endif()

  file(STRINGS "${__FILE}" data)

  set(types "compile|compile-fail|link|link-fail|run|run-fail")

  foreach(line IN LISTS data)

    if(line MATCHES "^[ \t]*(${types})[ \t]+([^ \t]+)[ \t]*(\;[ \t]*)?$")

      boost_test(PREFIX "${__PREFIX}" TYPE "${CMAKE_MATCH_1}"
        SOURCES "${CMAKE_MATCH_2}"
        LINK_LIBRARIES ${__LIBRARIES} ${__LINK_LIBRARIES}
        COMPILE_DEFINITIONS ${__COMPILE_DEFINITIONS}
        COMPILE_OPTIONS ${__COMPILE_OPTIONS}
        COMPILE_FEATURES ${__COMPILE_FEATURES}
      )

    elseif(line MATCHES "^[ \t]*(${types})([ \t]|$)")

      boost_message(VERBOSE "boost_test_jamfile: Jamfile line ignored: ${line}")

    endif()

  endforeach()
endfunction()
