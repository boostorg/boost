# Copyright 2018, 2019 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.5...3.16)

project(boost_cmake_test LANGUAGES CXX)

include(CTest)

set(BUILD_TESTING OFF) # hide cache variable

add_subdirectory(../../../../libs/assert EXCLUDE_FROM_ALL boostorg/assert)
add_subdirectory(../../../../libs/config EXCLUDE_FROM_ALL boostorg/config)
add_subdirectory(../../../../libs/core EXCLUDE_FROM_ALL boostorg/core)
add_subdirectory(../../../../libs/static_assert EXCLUDE_FROM_ALL boostorg/static_assert)
add_subdirectory(../../../../libs/throw_exception EXCLUDE_FROM_ALL boostorg/throw_exception)

unset(BUILD_TESTING)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

# boost_test

include(BoostTest)

boost_test(TYPE compile SOURCES compile.cpp)
boost_test(TYPE compile-fail SOURCES compile_fail.cpp)
boost_test(TYPE link SOURCES link.cpp)
boost_test(TYPE link-fail SOURCES link_fail.cpp)
boost_test(TYPE run SOURCES run.cpp)
boost_test(TYPE run-fail SOURCES run_fail.cpp)

boost_test(TYPE run SOURCES arguments.cpp ARGUMENTS pumpkin LINK_LIBRARIES Boost::core)

boost_test(TYPE run NAME return_exit_code_pass SOURCES return_exit_code.cpp COMPILE_DEFINITIONS EXIT_CODE=0)
boost_test(TYPE run-fail NAME return_exit_code_fail SOURCES return_exit_code.cpp COMPILE_DEFINITIONS EXIT_CODE=1)

boost_test(TYPE run SOURCES requires_variadic_templates.cpp COMPILE_FEATURES cxx_variadic_templates)

boost_test(TYPE run SOURCES requires_no_rtti.cpp COMPILE_OPTIONS -fno-rtti LINK_LIBRARIES Boost::config)
boost_test(TYPE run SOURCES requires_no_exceptions.cpp COMPILE_OPTIONS -fno-exceptions LINK_LIBRARIES Boost::config)

boost_test(TYPE compile-fail SOURCES emits_warning.cpp COMPILE_OPTIONS -Wall -Werror)

# boost_test, w/ globals

set(BOOST_TEST_COMPILE_OPTIONS -fno-rtti -fno-exceptions)
set(BOOST_TEST_LINK_LIBRARIES Boost::config)

boost_test(SOURCES requires_no_rtti.cpp PREFIX boost_cmake_test_globals)
boost_test(SOURCES requires_no_exceptions.cpp PREFIX boost_cmake_test_globals)

# boost_test_jamfile

include(BoostTestJamfile)

boost_test_jamfile(FILE Jamfile PREFIX boost_cmake_test_jamfile)
