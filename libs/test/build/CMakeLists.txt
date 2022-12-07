# Copyright 2014, Raffi Enficiaud

# Use, modification, and distribution are subject to the
# Boost Software License, Version 1.0. (See accompanying file
# LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# See http://www.boost.org/libs/test for the library home page.

cmake_minimum_required(VERSION 2.8.11)
project(BoostTest)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_MACOSX_RPATH ON)

add_definitions(-DBOOST_TEST_NO_LIB)

# build type, by default to release (with optimisations)
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

if(NOT WITHOUT_TESTS)
  # ctest sets BUILD_TESTING automatically, but does not seem to serve its purpose.
  include(CTest)
  enable_testing()
endif()


include(CheckCXXCompilerFlag)
include(CheckIncludeFileCXX)

if(NOT MSVC)
  # c++11 options
  check_cxx_compiler_flag(-std=c++11 HAS_CXX11_FLAG)
  check_cxx_compiler_flag(-std=c++0x HAS_CXX0X_FLAG)
  if(HAS_CXX11_FLAG)
    message(STATUS "Compiling with C++11 support")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  elseif(HAS_CXX0X_FLAG)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
  endif()
endif()

if(MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS)
  set(MSVC_Additional_flags "/fp:fast /GF /Oy /GT /Ox /Ob2 /Oi /Os")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${MSVC_Additional_flags}")
endif()


# global path
set(BOOST_TEST_ROOT_DIR ${BoostTest_SOURCE_DIR}/..)
set(BOOST_ROOT_DIR ${BOOST_TEST_ROOT_DIR}/../..)
get_filename_component(BOOST_TEST_ROOT_DIR_ABS ${BoostTest_SOURCE_DIR}/.. ABSOLUTE)
get_filename_component(BOOST_ROOT_DIR_ABS ${BOOST_ROOT_DIR} ABSOLUTE)

# include globs
file(GLOB_RECURSE
     BOOST_UTF_HEADERS
     ${BOOST_TEST_ROOT_DIR}/include/*.hpp
     ${BOOST_TEST_ROOT_DIR}/include/*.ipp)

# organize files
foreach(_h IN LISTS BOOST_UTF_HEADERS)
  get_filename_component(_hh ${_h} ABSOLUTE)
  file(RELATIVE_PATH _v ${BOOST_TEST_ROOT_DIR_ABS}/include/boost/test ${_hh})
  get_filename_component(_v "${_v}" DIRECTORY)
  string(REPLACE "/" "\\" _v "${_v}")
  source_group(${_v} FILES ${_h})
endforeach()

set(BOOST_UTF_SRC
    ${BOOST_TEST_ROOT_DIR}/src/compiler_log_formatter.cpp

    ${BOOST_TEST_ROOT_DIR}/src/debug.cpp
    ${BOOST_TEST_ROOT_DIR}/src/decorator.cpp
    ${BOOST_TEST_ROOT_DIR}/src/execution_monitor.cpp
    ${BOOST_TEST_ROOT_DIR}/src/framework.cpp
    ${BOOST_TEST_ROOT_DIR}/src/junit_log_formatter.cpp
    ${BOOST_TEST_ROOT_DIR}/src/plain_report_formatter.cpp
    ${BOOST_TEST_ROOT_DIR}/src/progress_monitor.cpp
    ${BOOST_TEST_ROOT_DIR}/src/results_collector.cpp
    ${BOOST_TEST_ROOT_DIR}/src/results_reporter.cpp
    ${BOOST_TEST_ROOT_DIR}/src/test_framework_init_observer.cpp
    ${BOOST_TEST_ROOT_DIR}/src/test_tools.cpp
    ${BOOST_TEST_ROOT_DIR}/src/test_tree.cpp
    ${BOOST_TEST_ROOT_DIR}/src/unit_test_log.cpp
    ${BOOST_TEST_ROOT_DIR}/src/unit_test_main.cpp
    ${BOOST_TEST_ROOT_DIR}/src/unit_test_monitor.cpp
    ${BOOST_TEST_ROOT_DIR}/src/unit_test_parameters.cpp
    ${BOOST_TEST_ROOT_DIR}/src/xml_log_formatter.cpp
    ${BOOST_TEST_ROOT_DIR}/src/xml_report_formatter.cpp
)

add_library(boost_test_framework
            STATIC
            ${BOOST_UTF_HEADERS}
            ${BOOST_UTF_SRC})
#target_compile_definitions(boost_test_framework PUBLIC "-DBOOST_TEST_DYN_LINK=0")
target_include_directories(boost_test_framework 
    PUBLIC 
        ${BOOST_TEST_ROOT_DIR}/include/ 
        ${BOOST_ROOT_DIR_ABS}/)
set_target_properties(boost_test_framework PROPERTIES FOLDER "UTF")

add_library(boost_test_framework_shared
            SHARED
            ${BOOST_UTF_HEADERS}
            ${BOOST_UTF_SRC})
target_compile_definitions(boost_test_framework_shared PUBLIC "-DBOOST_TEST_DYN_LINK=1")
target_include_directories(boost_test_framework_shared 
    PUBLIC 
        ${BOOST_TEST_ROOT_DIR}/include/ 
        ${BOOST_ROOT_DIR_ABS}/)
set_target_properties(boost_test_framework_shared PROPERTIES FOLDER "UTF")

####
# Documentation files (files only, no target)
file(GLOB_RECURSE
     BOOST_UTF_DOC_FILES
     ${BOOST_TEST_ROOT_DIR}/doc/*.qbk)
add_custom_target(
  quickbook
  SOURCES ${BOOST_UTF_DOC_FILES})
set_property(TARGET quickbook PROPERTY FOLDER "Documentation/")

# Unit tests
add_subdirectory(${BOOST_TEST_ROOT_DIR}/test tmp_folders_tests)
