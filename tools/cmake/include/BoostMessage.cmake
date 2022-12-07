# Copyright 2019 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

if(NOT CMAKE_VERSION VERSION_LESS 3.10)
  include_guard()
endif()

# boost_message(
#   [FATAL_ERROR|SEND_ERROR|WARNING|AUTHOR_WARNING|DEPRECATION|NOTICE|STATUS
#     |VERBOSE|DEBUG]
#   messages...)

function(boost_message type)

  if(type STREQUAL "VERBOSE")
    if(Boost_VERBOSE OR Boost_DEBUG)
      set(type STATUS)
    elseif(CMAKE_VERSION VERSION_LESS 3.15)
      return()
    endif()
  endif()

  if(type STREQUAL "DEBUG")
    if(Boost_DEBUG)
      set(type STATUS)
    elseif(CMAKE_VERSION VERSION_LESS 3.15)
      return()
    endif()
  endif()

  if(type STREQUAL "NOTICE" AND CMAKE_VERSION VERSION_LESS 3.15)
    set(type "")
  endif()

  set(m "")
  math(EXPR last "${ARGC}-1")

  foreach(i RANGE 1 ${last})
    string(APPEND m "${ARGV${i}}")
  endforeach()

  message(${type} "${m}")

endfunction()
