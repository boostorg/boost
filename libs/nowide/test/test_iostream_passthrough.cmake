# Copyright 2019 - 2021 Alexander Grund
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

if(NOT DEFINED TEST_BINARY)
  if(CMAKE_ARGC GREATER 3)
    # cmake(0) -P(1) <script>(2) <first arg>(3)
    set(TEST_BINARY ${CMAKE_ARGV3})
  else()
    message(FATAL_ERROR "You need to define TEST_BINARY with the path to the binary")
  endif()
endif()

set(msg "Hello Boost Nowide")

execute_process(
  COMMAND ${CMAKE_COMMAND} -E echo "${msg}" # This will be stdin of the below cmd
  COMMAND "${TEST_BINARY}" passthrough
  RESULT_VARIABLE result
  OUTPUT_VARIABLE stdout
)
if(NOT result EQUAL 0)
  message(FATAL_ERROR "Command ${TEST_BINARY} failed (${result}) with ${stdout}")
endif()
if(NOT stdout MATCHES ".*${msg}")
  message(FATAL_ERROR "Command ${TEST_BINARY} did not output '${msg}' but '${stdout}'")
endif()
message(STATUS "Test OK")
