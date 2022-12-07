# Copyright 2018-2021 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.5...3.20)

project(boost_add_sibdir_test LANGUAGES CXX)

include(CTest)

set(BOOST_INCLUDE_LIBRARIES timer)

add_subdirectory(../../../.. deps/boost)

add_executable(main main.cpp)
target_link_libraries(main Boost::timer)

add_test(NAME main COMMAND main)
