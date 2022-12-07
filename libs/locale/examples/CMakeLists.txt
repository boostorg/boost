# Copyright 2021-2022 Alexander Grund
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

function(boost_locale_add_example name)
  cmake_parse_arguments(PARSE_ARGV 1 ARG "COMPILE_ONLY" "SRC;WORKING_DIRECTORY" "ARGS")

  if(NOT ARG_SRC)
    set(ARG_SRC ${name}.cpp)
  endif()
  set(name ${PROJECT_NAME}-expl_${name})

  add_executable(${name} ${ARG_SRC})
  add_dependencies(tests ${name})
  target_link_libraries(${name} PRIVATE
    Boost::locale
    Boost::algorithm
  )

  if(ARG_WORKING_DIRECTORY)
    set_target_properties(${name} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${ARG_WORKING_DIRECTORY}")
  endif()

  if(NOT ARG_COMPILE_ONLY)
    add_test(NAME ${name} COMMAND ${name} ${ARG_ARGS})
    if(ARG_WORKING_DIRECTORY)
      set_tests_properties(${name} PROPERTIES WORKING_DIRECTORY "${ARG_WORKING_DIRECTORY}")
    endif()
  endif()
endfunction()

boost_locale_add_example(boundary)
boost_locale_add_example(calendar)
boost_locale_add_example(collate COMPILE_ONLY)
boost_locale_add_example(conversions)
boost_locale_add_example(hello)
boost_locale_add_example(wboundary)
boost_locale_add_example(wconversions)
boost_locale_add_example(whello)

boost_locale_add_example(perf_collate SRC performance/perf_collate.cpp COMPILE_ONLY)
boost_locale_add_example(perf_convert SRC performance/perf_convert.cpp COMPILE_ONLY)
boost_locale_add_example(perf_format SRC performance/perf_format.cpp COMPILE_ONLY)
