# Copyright 2018, 2019 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.5...3.16)

project(boost_fetch_test LANGUAGES CXX)

include("${CMAKE_CURRENT_SOURCE_DIR}/../../include/BoostFetch.cmake")

include(CTest)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C $<CONFIG>)

set(BUILD_TESTING OFF) # hide cache variable

boost_fetch(boostorg/assert TAG develop EXCLUDE_FROM_ALL)
boost_fetch(boostorg/config TAG develop EXCLUDE_FROM_ALL)
boost_fetch(boostorg/core TAG develop EXCLUDE_FROM_ALL)
boost_fetch(boostorg/static_assert TAG develop EXCLUDE_FROM_ALL)
boost_fetch(boostorg/throw_exception TAG develop EXCLUDE_FROM_ALL)

unset(BUILD_TESTING)

add_executable(main main.cpp)
target_link_libraries(main Boost::core)

add_test(NAME main COMMAND main)
