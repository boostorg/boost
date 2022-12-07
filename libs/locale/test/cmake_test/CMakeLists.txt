# Copyright 2022 Alexander Grund
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.5...3.16)

project(cmake_subdir_test LANGUAGES CXX)

# Those 2 should work the same
if(DEFINED BOOST_CI_INSTALL_TEST AND BOOST_CI_INSTALL_TEST)
    message("Using installed Boost")
    find_package(boost_locale REQUIRED)
else()
    message("Building Boost")
    add_subdirectory(../.. boostorg/locale)
    set(deps
      # Primary dependencies
      assert
      config
      function
      iterator
      smart_ptr
      static_assert
      type_traits
      thread
      unordered

      # Secondary dependencies
      algorithm
      align
      array
      atomic
      bind
      chrono
      concept_check
      container
      container_hash
      conversion
      core
      date_time
      detail
      exception
      function_types
      functional
      fusion
      integer
      intrusive
      io
      lexical_cast
      move
      mp11
      mpl
      numeric/conversion
      optional
      predef
      preprocessor
      range
      ratio
      rational
      regex
      system
      throw_exception
      tokenizer
      tuple
      type_index
      typeof
      utility
      variant2
      winapi
    )

    foreach(dep IN LISTS deps)
      add_subdirectory(../../../${dep} boostorg/${dep})
    endforeach()
endif()

add_executable(main main.cpp)
target_link_libraries(main Boost::locale Boost::system)

enable_testing()
add_test(NAME main COMMAND main)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -C $<CONFIG>)
