# Copyright 2019-2021 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

if(CMAKE_SOURCE_DIR STREQUAL Boost_SOURCE_DIR AND WIN32 AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

    set(CMAKE_INSTALL_PREFIX "C:/Boost" CACHE PATH "Installation path prefix, prepended to installation directories" FORCE)

endif()

include(BoostMessage)
include(BoostInstall)

#

set(__boost_incompatible_libraries "")

# Define cache variables if root project

if(CMAKE_SOURCE_DIR STREQUAL Boost_SOURCE_DIR)

  # --with-<library>
  set(BOOST_INCLUDE_LIBRARIES "" CACHE STRING
    "List of libraries to build (default: all but excluded and incompatible)")

  # --without-<library>
  set(BOOST_EXCLUDE_LIBRARIES "" CACHE STRING
    "List of libraries to exclude from build")

  set(BOOST_INCOMPATIBLE_LIBRARIES
    "${__boost_incompatible_libraries}"
    CACHE STRING
    "List of libraries with incompatible CMakeLists.txt files")

  option(BOOST_ENABLE_MPI
    "Build and enable installation of Boost.MPI and its dependents (requires MPI, CMake 3.9)")

  option(BOOST_ENABLE_PYTHON
    "Build and enable installation of Boost.Python and its dependents (requires Python, CMake 3.14)")

  # --layout, --libdir, --cmakedir, --includedir in BoostInstall

  # runtime-link=static|shared

  set(BOOST_RUNTIME_LINK shared CACHE STRING "Runtime library selection for the MS ABI (shared or static)")
  set_property(CACHE BOOST_RUNTIME_LINK PROPERTY STRINGS shared static)

  option(BUILD_TESTING "Build the tests." OFF)
  include(CTest)

  if(NOT TARGET tests)
    add_custom_target(tests)
  endif()

  add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure --no-tests=error -C $<CONFIG>)
  add_dependencies(check tests)

  # link=static|shared
  option(BUILD_SHARED_LIBS "Build shared libraries")

  # --stagedir
  set(BOOST_STAGEDIR "${CMAKE_CURRENT_BINARY_DIR}/stage" CACHE STRING "Build output directory")

  if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BOOST_STAGEDIR}/bin")
  endif()

  if(NOT CMAKE_LIBRARY_OUTPUT_DIRECTORY)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BOOST_STAGEDIR}/lib")
  endif()

  if(NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BOOST_STAGEDIR}/lib")
  endif()

else()

  # add_subdirectory use

  if(NOT DEFINED BOOST_INCOMPATIBLE_LIBRARIES)
    set(BOOST_INCOMPATIBLE_LIBRARIES ${__boost_incompatible_libraries})
  endif()

  if(NOT DEFINED BOOST_ENABLE_MPI)
    set(BOOST_ENABLE_MPI OFF)
  endif()

  if(NOT DEFINED BOOST_ENABLE_PYTHON)
    set(BOOST_ENABLE_PYTHON OFF)
  endif()

  set(BUILD_TESTING OFF)
  set(CMAKE_SKIP_INSTALL_RULES ON)

endif()

if(NOT CMAKE_MSVC_RUNTIME_LIBRARY)

  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

  if(NOT BOOST_RUNTIME_LINK STREQUAL "static")
    string(APPEND CMAKE_MSVC_RUNTIME_LIBRARY "DLL")
  endif()

endif()

# Output configuration status lines

set(_msg "")

if(NOT CMAKE_CONFIGURATION_TYPES AND CMAKE_BUILD_TYPE)
  string(APPEND _msg "${CMAKE_BUILD_TYPE} build, ")
endif()

if(BUILD_SHARED_LIBS)
  string(APPEND _msg "shared libraries, ")
else()
  string(APPEND _msg "static libraries, ")
endif()

if(MSVC)
  if(CMAKE_MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    string(APPEND _msg "static runtime, ")
  elseif(CMAKE_MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    string(APPEND _msg "shared runtime, ")
  endif()
endif()

string(APPEND _msg "MPI ${BOOST_ENABLE_MPI}, Python ${BOOST_ENABLE_PYTHON}, testing ${BUILD_TESTING}")

message(STATUS "Boost: ${_msg}")

unset(_msg)

if(BOOST_INCLUDE_LIBRARIES)
  message(STATUS "Boost: libraries included: ${BOOST_INCLUDE_LIBRARIES}")
endif()

if(BOOST_EXCLUDE_LIBRARIES)
  message(STATUS "Boost: libraries excluded: ${BOOST_EXCLUDE_LIBRARIES}")
endif()

# Define functions

function(__boost_auto_install __boost_lib)
  if(NOT CMAKE_VERSION VERSION_LESS 3.13)

    string(MAKE_C_IDENTIFIER "${__boost_lib}" __boost_lib_target)

    if(TARGET "Boost::${__boost_lib_target}" AND TARGET "boost_${__boost_lib_target}")

      get_target_property(__boost_lib_incdir "boost_${__boost_lib_target}" INTERFACE_INCLUDE_DIRECTORIES)

      if(__boost_lib_incdir STREQUAL "${BOOST_SUPERPROJECT_SOURCE_DIR}/libs/${__boost_lib}/include")

        boost_message(DEBUG "Enabling installation for '${__boost_lib}'")
        boost_install(TARGETS "boost_${__boost_lib_target}" VERSION "${BOOST_SUPERPROJECT_VERSION}" HEADER_DIRECTORY "${BOOST_SUPERPROJECT_SOURCE_DIR}/libs/${__boost_lib}/include")

      else()
        boost_message(DEBUG "Not enabling installation for '${__boost_lib}'; interface include directory '${__boost_lib_incdir}' does not equal '${BOOST_SUPERPROJECT_SOURCE_DIR}/libs/${__boost_lib}/include'")
      endif()

    else()
      boost_message(DEBUG "Not enabling installation for '${__boost_lib}'; targets 'Boost::${__boost_lib_target}' and 'boost_${__boost_lib_target}' weren't found")
    endif()

  endif()
endfunction()

function(__boost_scan_dependencies lib var)

  set(result "")

  if(EXISTS "${BOOST_SUPERPROJECT_SOURCE_DIR}/libs/${lib}/CMakeLists.txt")

    file(STRINGS "${BOOST_SUPERPROJECT_SOURCE_DIR}/libs/${lib}/CMakeLists.txt" data)

    foreach(line IN LISTS data)

      if(line MATCHES "^[ ]*Boost::([A-Za-z0-9_]+)[ ]*$")

        string(REGEX REPLACE "^numeric_" "numeric/" dep ${CMAKE_MATCH_1})
        list(APPEND result ${dep})

      endif()

    endforeach()

  endif()

  set(${var} ${result} PARENT_SCOPE)

endfunction()

#

file(GLOB __boost_libraries RELATIVE "${BOOST_SUPERPROJECT_SOURCE_DIR}/libs" "${BOOST_SUPERPROJECT_SOURCE_DIR}/libs/*/CMakeLists.txt" "${BOOST_SUPERPROJECT_SOURCE_DIR}/libs/numeric/*/CMakeLists.txt")

# Check for mistakes in BOOST_INCLUDE_LIBRARIES

foreach(__boost_included_lib IN LISTS BOOST_INCLUDE_LIBRARIES)

  if(NOT "${__boost_included_lib}/CMakeLists.txt" IN_LIST __boost_libraries)

    message(WARNING "Library '${__boost_included_lib}' given in BOOST_INCLUDE_LIBRARIES has not been found.")

  endif()

endforeach()

# Scan for dependencies

set(__boost_include_libraries ${BOOST_INCLUDE_LIBRARIES})

if(__boost_include_libraries)
  list(REMOVE_DUPLICATES __boost_include_libraries)
endif()

set(__boost_libs_to_scan ${__boost_include_libraries})

while(__boost_libs_to_scan)

  boost_message(DEBUG "Scanning dependencies: ${__boost_libs_to_scan}")

  set(__boost_dependencies "")

  foreach(__boost_lib IN LISTS __boost_libs_to_scan)

    __boost_scan_dependencies(${__boost_lib} __boost_deps)
    list(APPEND __boost_dependencies ${__boost_deps})

  endforeach()

  list(REMOVE_DUPLICATES __boost_dependencies)

  set(__boost_libs_to_scan ${__boost_dependencies})

  if(__boost_libs_to_scan)
    list(REMOVE_ITEM __boost_libs_to_scan ${__boost_include_libraries})
  endif()

  list(APPEND __boost_include_libraries ${__boost_libs_to_scan})

endwhile()

# Installing targets created in other directories requires CMake 3.13
if(CMAKE_VERSION VERSION_LESS 3.13)

  boost_message(STATUS "Boost installation support requires CMake 3.13 (have ${CMAKE_VERSION})")

endif()

set(__boost_mpi_libs mpi graph_parallel property_map_parallel)
set(__boost_python_libs python parameter_python)

foreach(__boost_lib_cml IN LISTS __boost_libraries)

  get_filename_component(__boost_lib "${__boost_lib_cml}" DIRECTORY)

  if(__boost_lib IN_LIST BOOST_INCOMPATIBLE_LIBRARIES)

    boost_message(DEBUG "Skipping incompatible Boost library ${__boost_lib}")

  elseif(__boost_lib IN_LIST BOOST_EXCLUDE_LIBRARIES)

    boost_message(DEBUG "Skipping excluded Boost library ${__boost_lib}")

  elseif(NOT BOOST_ENABLE_MPI AND __boost_lib IN_LIST __boost_mpi_libs)

    boost_message(DEBUG "Skipping Boost library ${__boost_lib}, BOOST_ENABLE_MPI is OFF")

  elseif(NOT BOOST_ENABLE_PYTHON AND __boost_lib IN_LIST __boost_python_libs)

    boost_message(DEBUG "Skipping Boost library ${__boost_lib}, BOOST_ENABLE_PYTHON is OFF")

  elseif(NOT BOOST_INCLUDE_LIBRARIES OR __boost_lib IN_LIST BOOST_INCLUDE_LIBRARIES)

    boost_message(VERBOSE "Adding Boost library ${__boost_lib}")
    add_subdirectory(libs/${__boost_lib})

    __boost_auto_install(${__boost_lib})

  elseif(__boost_lib IN_LIST __boost_include_libraries)

    # Disable tests for dependencies

    set(__boost_build_testing ${BUILD_TESTING})
    set(BUILD_TESTING OFF) # hide cache variable

    boost_message(VERBOSE "Adding Boost dependency ${__boost_lib}")
    add_subdirectory(libs/${__boost_lib})

    __boost_auto_install(${__boost_lib})

    set(BUILD_TESTING ${__boost_build_testing})

  elseif(BUILD_TESTING)

    # Disable tests and installation for libraries neither included nor dependencies

    set(__boost_build_testing ${BUILD_TESTING})
    set(BUILD_TESTING OFF) # hide cache variable

    set(__boost_skip_install ${CMAKE_SKIP_INSTALL_RULES})
    set(CMAKE_SKIP_INSTALL_RULES ON)

    boost_message(DEBUG "Adding Boost library ${__boost_lib} with EXCLUDE_FROM_ALL")
    add_subdirectory(libs/${__boost_lib} EXCLUDE_FROM_ALL)

    set(BUILD_TESTING ${__boost_build_testing})
    set(CMAKE_SKIP_INSTALL_RULES ${__boost_skip_install})

  endif()

endforeach()
