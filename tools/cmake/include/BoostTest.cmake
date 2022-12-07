# Copyright 2018, 2019 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

# Clear global variables on each `include(BoostTest)`

set(BOOST_TEST_LINK_LIBRARIES "")
set(BOOST_TEST_COMPILE_DEFINITIONS "")
set(BOOST_TEST_COMPILE_OPTIONS "")
set(BOOST_TEST_COMPILE_FEATURES "")

# Include guard

if(NOT CMAKE_VERSION VERSION_LESS 3.10)
  include_guard()
endif()

include(BoostMessage)

# Private helper functions

function(__boost_test_list_replace list what with)

  set(result "")

  foreach(x IN LISTS ${list})

    if(x STREQUAL what)
      set(x ${with})
    endif()

    list(APPEND result ${x})

  endforeach()

  set(${list} ${result} PARENT_SCOPE)

endfunction()

# boost_test( [TYPE type] [PREFIX prefix] [NAME name] [IGNORE_TEST_GLOBALS]
#   SOURCES sources...
#   ARGUMENTS args...
#   LINK_LIBRARIES libs...
#   COMPILE_DEFINITIONS defs...
#   COMPILE_OPTIONS opts...
#   COMPILE_FEATURES features...
# )

function(boost_test)

  cmake_parse_arguments(_ "IGNORE_TEST_GLOBALS" "TYPE;PREFIX;NAME" "SOURCES;ARGUMENTS;LIBRARIES;LINK_LIBRARIES;COMPILE_DEFINITIONS;COMPILE_OPTIONS;COMPILE_FEATURES" ${ARGN})

  if(NOT __TYPE)
    set(__TYPE run)
  endif()

  if(NOT __PREFIX)
    set(__PREFIX ${PROJECT_NAME})
  endif()

  if(NOT __NAME)
    list(GET __SOURCES 0 __NAME)
    string(MAKE_C_IDENTIFIER ${__NAME} __NAME)
  endif()

  set(__NAME ${__PREFIX}-${__NAME})

  if(__UNPARSED_ARGUMENTS)
    message(AUTHOR_WARNING "Extra arguments for test '${__NAME}' ignored: ${__UNPARSED_ARGUMENTS}")
  endif()

  if(__LIBRARIES)
    boost_message(VERBOSE "Test '${__NAME}' uses deprecated parameter LIBRARIES; use LINK_LIBRARIES")
  endif()

  if(DEFINED BUILD_TESTING AND NOT BUILD_TESTING)
    return()
  endif()

  if(__IGNORE_TEST_GLOBALS)

    set(BOOST_TEST_LINK_LIBRARIES "")
    set(BOOST_TEST_COMPILE_DEFINITIONS "")
    set(BOOST_TEST_COMPILE_OPTIONS "")
    set(BOOST_TEST_COMPILE_FEATURES "")

  endif()

  list(APPEND BOOST_TEST_LINK_LIBRARIES ${__LIBRARIES} ${__LINK_LIBRARIES})
  list(APPEND BOOST_TEST_COMPILE_DEFINITIONS ${__COMPILE_DEFINITIONS})
  list(APPEND BOOST_TEST_COMPILE_OPTIONS ${__COMPILE_OPTIONS})
  list(APPEND BOOST_TEST_COMPILE_FEATURES ${__COMPILE_FEATURES})

  if(MSVC)

    __boost_test_list_replace(BOOST_TEST_COMPILE_OPTIONS "-fexceptions" "/GX")
    __boost_test_list_replace(BOOST_TEST_COMPILE_OPTIONS "-fno-exceptions" "/GX-")

    __boost_test_list_replace(BOOST_TEST_COMPILE_OPTIONS "-frtti" "/GR")
    __boost_test_list_replace(BOOST_TEST_COMPILE_OPTIONS "-fno-rtti" "/GR-")

    __boost_test_list_replace(BOOST_TEST_COMPILE_OPTIONS "-w" "/W0")
    __boost_test_list_replace(BOOST_TEST_COMPILE_OPTIONS "-Wall" "/W4")
    __boost_test_list_replace(BOOST_TEST_COMPILE_OPTIONS "-Wextra" "")
    __boost_test_list_replace(BOOST_TEST_COMPILE_OPTIONS "-pedantic" "")
    __boost_test_list_replace(BOOST_TEST_COMPILE_OPTIONS "-Wpedantic" "")

    __boost_test_list_replace(BOOST_TEST_COMPILE_OPTIONS "-Werror" "/WX")
    __boost_test_list_replace(BOOST_TEST_COMPILE_OPTIONS "-Wno-error" "/WX-")

  endif()

  foreach(feature IN LISTS BOOST_TEST_COMPILE_FEATURES)
    if(NOT feature IN_LIST CMAKE_CXX_COMPILE_FEATURES)

      boost_message(VERBOSE "Test '${__NAME}' skipped, '${feature}' is not supported")
      return()

    endif()
  endforeach()

  foreach(library IN LISTS BOOST_TEST_LINK_LIBRARIES)

    if(TARGET ${library})
      get_target_property(features ${library} INTERFACE_COMPILE_FEATURES)

      if(features) # need to check because features-NOTFOUND is a valid list
        foreach(feature IN LISTS features)
          if(NOT feature IN_LIST CMAKE_CXX_COMPILE_FEATURES)

            boost_message(VERBOSE "Test '${__NAME}' skipped, '${feature}' required by '${library}' is not supported")
            return()

          endif()
        endforeach()
      endif()
    endif()
  endforeach()

  if(NOT TARGET tests)
    add_custom_target(tests)
  endif()

  if(__TYPE STREQUAL "compile")

    add_library(${__NAME} STATIC EXCLUDE_FROM_ALL ${__SOURCES})
    target_link_libraries(${__NAME} ${BOOST_TEST_LINK_LIBRARIES})
    target_compile_definitions(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_DEFINITIONS})
    target_compile_options(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_OPTIONS})
    target_compile_features(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_FEATURES})

    add_dependencies(tests ${__NAME})

  elseif(__TYPE STREQUAL "compile-fail")

    add_library(${__NAME} STATIC EXCLUDE_FROM_ALL ${__SOURCES})
    target_link_libraries(${__NAME} ${BOOST_TEST_LINK_LIBRARIES})
    target_compile_definitions(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_DEFINITIONS})
    target_compile_options(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_OPTIONS})
    target_compile_features(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_FEATURES})

    add_test(NAME ${__TYPE}-${__NAME} COMMAND "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${__NAME} --config $<CONFIG>)

    set_tests_properties(${__TYPE}-${__NAME} PROPERTIES WILL_FAIL TRUE RUN_SERIAL TRUE)

  elseif(__TYPE STREQUAL "link")

    add_executable(${__NAME} EXCLUDE_FROM_ALL ${__SOURCES})
    target_link_libraries(${__NAME} ${BOOST_TEST_LINK_LIBRARIES})
    target_compile_definitions(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_DEFINITIONS})
    target_compile_options(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_OPTIONS})
    target_compile_features(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_FEATURES})

    add_dependencies(tests ${__NAME})

  elseif(__TYPE STREQUAL "link-fail")

    add_library(compile-${__NAME} OBJECT EXCLUDE_FROM_ALL ${__SOURCES})
    target_link_libraries(compile-${__NAME} ${BOOST_TEST_LINK_LIBRARIES})
    target_compile_definitions(compile-${__NAME} PRIVATE ${BOOST_TEST_COMPILE_DEFINITIONS})
    target_compile_options(compile-${__NAME} PRIVATE ${BOOST_TEST_COMPILE_OPTIONS})
    target_compile_features(compile-${__NAME} PRIVATE ${BOOST_TEST_COMPILE_FEATURES})

    add_dependencies(tests compile-${__NAME})

    add_executable(${__NAME} EXCLUDE_FROM_ALL $<TARGET_OBJECTS:compile-${__NAME}>)
    target_link_libraries(${__NAME} ${BOOST_TEST_LINK_LIBRARIES})
    target_compile_definitions(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_DEFINITIONS})
    target_compile_options(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_OPTIONS})
    target_compile_features(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_FEATURES})

    add_test(NAME ${__TYPE}-${__NAME} COMMAND "${CMAKE_COMMAND}" --build ${CMAKE_BINARY_DIR} --target ${__NAME} --config $<CONFIG>)
    set_tests_properties(${__TYPE}-${__NAME} PROPERTIES WILL_FAIL TRUE RUN_SERIAL TRUE)

  elseif(__TYPE STREQUAL "run" OR __TYPE STREQUAL "run-fail")

    add_executable(${__NAME} EXCLUDE_FROM_ALL ${__SOURCES})
    target_link_libraries(${__NAME} ${BOOST_TEST_LINK_LIBRARIES})
    target_compile_definitions(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_DEFINITIONS})
    target_compile_options(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_OPTIONS})
    target_compile_features(${__NAME} PRIVATE ${BOOST_TEST_COMPILE_FEATURES})

    add_dependencies(tests ${__NAME})

    add_test(NAME ${__TYPE}-${__NAME} COMMAND ${__NAME} ${__ARGUMENTS})

    if(__TYPE STREQUAL "run-fail")
      set_tests_properties(${__TYPE}-${__NAME} PROPERTIES WILL_FAIL TRUE)
    endif()

  else()

    message(AUTHOR_WARNING "Unknown test type '${__TYPE}' for test '${__NAME}'")

  endif()

endfunction(boost_test)
