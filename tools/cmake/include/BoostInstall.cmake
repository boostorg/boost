# Copyright 2019, 2020, 2021 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt

if(NOT CMAKE_VERSION VERSION_LESS 3.10)
  include_guard()
endif()

include(BoostMessage)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Variables

if(WIN32)
  set(__boost_default_layout "versioned")
else()
  set(__boost_default_layout "system")
endif()

set(__boost_default_cmakedir "${CMAKE_INSTALL_LIBDIR}/cmake")
set(__boost_default_include_subdir "/boost-${PROJECT_VERSION_MAJOR}_${PROJECT_VERSION_MINOR}")

# Define cache variables when Boost is the root project

if(CMAKE_SOURCE_DIR STREQUAL "${BOOST_SUPERPROJECT_SOURCE_DIR}")

  set(BOOST_INSTALL_LAYOUT "${__boost_default_layout}" CACHE STRING "Installation layout (versioned, tagged, or system)")
  set_property(CACHE BOOST_INSTALL_LAYOUT PROPERTY STRINGS versioned tagged system)

  set(BOOST_INSTALL_CMAKEDIR "${__boost_default_cmakedir}" CACHE STRING "Installation directory for CMake configuration files")
  set(BOOST_INSTALL_INCLUDE_SUBDIR "${__boost_default_include_subdir}" CACHE STRING "Header subdirectory when layout is versioned")

else()

  # add_subdirectory use

  if(NOT DEFINED BOOST_INSTALL_LAYOUT)
    set(BOOST_INSTALL_LAYOUT "${__boost_default_layout}")
  endif()

  if(NOT DEFINED BOOST_INSTALL_CMAKEDIR)
    set(BOOST_INSTALL_CMAKEDIR "${__boost_default_cmakedir}")
  endif()

  if(NOT DEFINED BOOST_INSTALL_INCLUDE_SUBDIR)
    set(BOOST_INSTALL_INCLUDE_SUBDIR "${__boost_default_include_subdir}")
  endif()

endif()

if(BOOST_INSTALL_LAYOUT STREQUAL "versioned")
  string(APPEND CMAKE_INSTALL_INCLUDEDIR "${BOOST_INSTALL_INCLUDE_SUBDIR}")
endif()

#

if(CMAKE_SOURCE_DIR STREQUAL "${BOOST_SUPERPROJECT_SOURCE_DIR}" AND NOT __boost_install_status_message_guard)
  message(STATUS "Boost: using ${BOOST_INSTALL_LAYOUT} layout: ${CMAKE_INSTALL_INCLUDEDIR}, ${CMAKE_INSTALL_BINDIR}, ${CMAKE_INSTALL_LIBDIR}, ${BOOST_INSTALL_CMAKEDIR}")
  set(__boost_install_status_message_guard TRUE)
endif()

#

function(__boost_install_set_output_name LIB TYPE VERSION)

  set(name_debug ${LIB})
  set(name_release ${LIB})

  # prefix
  if(WIN32 AND TYPE STREQUAL "STATIC_LIBRARY")
    set_target_properties(${LIB} PROPERTIES PREFIX "lib")
  endif()

  # toolset
  if(BOOST_INSTALL_LAYOUT STREQUAL versioned)

    string(TOLOWER ${CMAKE_CXX_COMPILER_ID} toolset)

    if(toolset STREQUAL "msvc")

      set(toolset "vc")

      if(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)[.]([0-9]+)")

        if(CMAKE_MATCH_1 GREATER 18)
          math(EXPR major ${CMAKE_MATCH_1}-5)
        else()
          math(EXPR major ${CMAKE_MATCH_1}-6)
        endif()

        math(EXPR minor ${CMAKE_MATCH_2}/10)

        string(APPEND toolset ${major}${minor})

      endif()

    else()

      if(toolset STREQUAL "gnu")

        set(toolset "gcc")

      elseif(toolset STREQUAL "clang")

        if(MSVC)
          set(toolset "clangw")
        endif()

      endif()

      if(CMAKE_CXX_COMPILER_VERSION MATCHES "^([0-9]+)[.]")
        string(APPEND toolset ${CMAKE_MATCH_1})
      endif()

    endif()

    string(APPEND name_debug "-${toolset}")
    string(APPEND name_release "-${toolset}")

  endif()

  if(BOOST_INSTALL_LAYOUT STREQUAL versioned OR BOOST_INSTALL_LAYOUT STREQUAL tagged)

    # threading
    string(APPEND name_debug "-mt")
    string(APPEND name_release "-mt")

    # ABI tag

    if(MSVC)

      get_target_property(MSVC_RUNTIME_LIBRARY ${LIB} MSVC_RUNTIME_LIBRARY)

      if(MSVC_RUNTIME_LIBRARY STREQUAL "MultiThreaded$<$<CONFIG:Debug>:Debug>")

        string(APPEND name_debug "-sgd")
        string(APPEND name_release "-s")

      else()

        string(APPEND name_debug "-gd")

      endif()

    else()

      string(APPEND name_debug "-d")

    endif()

    # Arch and model
    math(EXPR bits ${CMAKE_SIZEOF_VOID_P}*8)

    string(APPEND name_debug "-x${bits}") # x86 only for now
    string(APPEND name_release "-x${bits}")

  endif()

  if(BOOST_INSTALL_LAYOUT STREQUAL versioned)

    string(REGEX REPLACE "^([0-9]+)[.]([0-9]+).*" "\\1_\\2" __ver ${VERSION})

    string(APPEND name_debug "-${__ver}")
    string(APPEND name_release "-${__ver}")

  endif()

  set_target_properties(${LIB} PROPERTIES OUTPUT_NAME_DEBUG ${name_debug})
  set_target_properties(${LIB} PROPERTIES OUTPUT_NAME ${name_release})

  if(TYPE STREQUAL "STATIC_LIBRARY")

    set_target_properties(${LIB} PROPERTIES COMPILE_PDB_NAME_DEBUG "${name_debug}")
    set_target_properties(${LIB} PROPERTIES COMPILE_PDB_NAME "${name_release}")

  endif()

endfunction()

function(__boost_install_update_include_directory lib incdir prop)

  get_target_property(value ${lib} ${prop})

  if(value STREQUAL incdir)

    set_target_properties(${lib} PROPERTIES ${prop} "$<BUILD_INTERFACE:${incdir}>;$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

  endif()

endfunction()

# Installs a single target
# boost_install_target(TARGET target VERSION version [HEADER_DIRECTORY directory])

function(boost_install_target)

  cmake_parse_arguments(_ "" "TARGET;VERSION;HEADER_DIRECTORY" "" ${ARGN})

  if(NOT __TARGET)

    message(SEND_ERROR "boost_install_target: TARGET not given.")
    return()

  endif()

  if(NOT __VERSION)

    message(SEND_ERROR "boost_install_target: VERSION not given, but is required for installation.")
    return()

  endif()

  set(LIB ${__TARGET})

  if(NOT __HEADER_DIRECTORY)

    set(__HEADER_DIRECTORY "${PROJECT_SOURCE_DIR}/include")

  endif()

  get_target_property(TYPE ${LIB} TYPE)

  __boost_install_update_include_directory(${LIB} "${__HEADER_DIRECTORY}" INTERFACE_INCLUDE_DIRECTORIES)

  if(TYPE STREQUAL "STATIC_LIBRARY" OR TYPE STREQUAL "SHARED_LIBRARY")

    __boost_install_update_include_directory(${LIB} "${__HEADER_DIRECTORY}" INCLUDE_DIRECTORIES)

    get_target_property(OUTPUT_NAME ${LIB} OUTPUT_NAME)

    if(NOT OUTPUT_NAME)
      __boost_install_set_output_name(${LIB} ${TYPE} ${__VERSION})
    endif()

  endif()

  if(TYPE STREQUAL "SHARED_LIBRARY" OR TYPE STREQUAL "EXECUTABLE")

    get_target_property(VERSION ${LIB} VERSION)

    if(NOT VERSION)
      set_target_properties(${LIB} PROPERTIES VERSION ${__VERSION})
    endif()

  endif()

  if(LIB MATCHES "^boost_(.*)$")
    set_target_properties(${LIB} PROPERTIES EXPORT_NAME ${CMAKE_MATCH_1})
  endif()

  if(CMAKE_SKIP_INSTALL_RULES)

    boost_message(DEBUG "boost_install_target: not installing target '${__TARGET}' due to CMAKE_SKIP_INSTALL_RULES=${CMAKE_SKIP_INSTALL_RULES}")
    return()

  endif()

  set(CONFIG_INSTALL_DIR "${BOOST_INSTALL_CMAKEDIR}/${LIB}-${__VERSION}")

  install(TARGETS ${LIB} EXPORT ${LIB}-targets
    # explicit destination specification required for 3.13, 3.14 no longer needs it
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    PRIVATE_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
  )

  if(MSVC)
    if(TYPE STREQUAL "SHARED_LIBRARY")
      install(FILES $<TARGET_PDB_FILE:${LIB}> DESTINATION ${CMAKE_INSTALL_BINDIR} OPTIONAL)
    endif()

    if(TYPE STREQUAL "STATIC_LIBRARY" AND NOT CMAKE_VERSION VERSION_LESS 3.15)
      install(FILES "$<TARGET_FILE_DIR:${LIB}>/$<TARGET_FILE_PREFIX:${LIB}>$<TARGET_FILE_BASE_NAME:${LIB}>.pdb" DESTINATION ${CMAKE_INSTALL_LIBDIR} OPTIONAL)
    endif()
  endif()

  install(EXPORT ${LIB}-targets DESTINATION "${CONFIG_INSTALL_DIR}" NAMESPACE Boost:: FILE ${LIB}-targets.cmake)

  set(CONFIG_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/tmpinst/${LIB}-config.cmake")
  set(CONFIG_FILE_CONTENTS "# Generated by BoostInstall.cmake for ${LIB}-${__VERSION}\n\n")

  get_target_property(INTERFACE_LINK_LIBRARIES ${LIB} INTERFACE_LINK_LIBRARIES)

  set(LINK_LIBRARIES "")

  if(TYPE STREQUAL "STATIC_LIBRARY" OR TYPE STREQUAL "SHARED_LIBRARY")
    get_target_property(LINK_LIBRARIES ${LIB} LINK_LIBRARIES)
  endif()

  if(INTERFACE_LINK_LIBRARIES OR LINK_LIBRARIES)

    string(APPEND CONFIG_FILE_CONTENTS "include(CMakeFindDependencyMacro)\n\n")

    set(link_libraries ${INTERFACE_LINK_LIBRARIES} ${LINK_LIBRARIES})
    list(REMOVE_DUPLICATES link_libraries)

    set(python_components "")
    set(icu_components "")

    foreach(dep IN LISTS link_libraries)

      if(dep MATCHES "^Boost::(.*)$")

        string(APPEND CONFIG_FILE_CONTENTS "find_dependency(boost_${CMAKE_MATCH_1} ${__VERSION} EXACT)\n")

      elseif(dep STREQUAL "Threads::Threads")

        string(APPEND CONFIG_FILE_CONTENTS "set(THREADS_PREFER_PTHREAD_FLAG ON)\n")
        string(APPEND CONFIG_FILE_CONTENTS "find_dependency(Threads)\n")

      elseif(dep STREQUAL "ZLIB::ZLIB")

        string(APPEND CONFIG_FILE_CONTENTS "find_dependency(ZLIB)\n")

      elseif(dep STREQUAL "BZip2::BZip2")

        string(APPEND CONFIG_FILE_CONTENTS "find_dependency(BZip2)\n")

      elseif(dep STREQUAL "LibLZMA::LibLZMA")

        string(APPEND CONFIG_FILE_CONTENTS "find_dependency(LibLZMA)\n")

      elseif(dep STREQUAL "zstd::libzstd_shared" OR dep STREQUAL "zstd::libzstd_static")

        string(APPEND CONFIG_FILE_CONTENTS "find_dependency(zstd)\n")

      elseif(dep STREQUAL "MPI::MPI_CXX")

        # COMPONENTS requires 3.9, but the imported target also requires 3.9
        string(APPEND CONFIG_FILE_CONTENTS "find_dependency(MPI COMPONENTS CXX)\n")

      elseif(dep STREQUAL "Iconv::Iconv")

        string(APPEND CONFIG_FILE_CONTENTS "find_dependency(Iconv)\n")

      elseif(dep STREQUAL "Python::Module")

        string(APPEND python_components " Development")

      elseif(dep STREQUAL "Python::NumPy")

        string(APPEND python_components " NumPy")

      elseif(dep STREQUAL "ICU::data")

        string(APPEND icu_components " data")

      elseif(dep STREQUAL "ICU::i18n")

        string(APPEND icu_components " i18n")

      elseif(dep STREQUAL "ICU::uc")

        string(APPEND icu_components " uc")

      endif()

    endforeach()

    if(python_components)

        string(APPEND CONFIG_FILE_CONTENTS "find_dependency(Python COMPONENTS ${python_components})\n")

    endif()

    if(icu_components)

        string(APPEND CONFIG_FILE_CONTENTS "find_dependency(ICU COMPONENTS ${icu_components})\n")

    endif()

    string(APPEND CONFIG_FILE_CONTENTS "\n")

  endif()

  string(APPEND CONFIG_FILE_CONTENTS "include(\"\${CMAKE_CURRENT_LIST_DIR}/${LIB}-targets.cmake\")\n")

  file(WRITE "${CONFIG_FILE_NAME}" "${CONFIG_FILE_CONTENTS}")
  install(FILES "${CONFIG_FILE_NAME}" DESTINATION "${CONFIG_INSTALL_DIR}")

  set(CONFIG_VERSION_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/tmpinst/${LIB}-config-version.cmake")

  if(TYPE STREQUAL "INTERFACE_LIBRARY")

    # Header-only libraries are architecture-independent

    if(NOT CMAKE_VERSION VERSION_LESS 3.14)

      write_basic_package_version_file("${CONFIG_VERSION_FILE_NAME}" COMPATIBILITY SameMajorVersion ARCH_INDEPENDENT)

    else()

      set(OLD_CMAKE_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})
      set(CMAKE_SIZEOF_VOID_P "")

      write_basic_package_version_file("${CONFIG_VERSION_FILE_NAME}" COMPATIBILITY SameMajorVersion)

      set(CMAKE_SIZEOF_VOID_P ${OLD_CMAKE_SIZEOF_VOID_P})

    endif()

  else()

    write_basic_package_version_file("${CONFIG_VERSION_FILE_NAME}" COMPATIBILITY SameMajorVersion)

  endif()

  install(FILES "${CONFIG_VERSION_FILE_NAME}" DESTINATION "${CONFIG_INSTALL_DIR}")

endfunction()

# boost_install([VERSION version] [TARGETS targets...] [HEADER_DIRECTORY directory])

function(boost_install)

  cmake_parse_arguments(_ "" "VERSION;HEADER_DIRECTORY" "TARGETS" ${ARGN})

  if(NOT __VERSION)

    if(NOT PROJECT_VERSION)

      message(AUTHOR_WARNING "boost_install: VERSION not given, PROJECT_VERSION not set, but a version is required for installation.")
      return()

    else()

      boost_message(DEBUG "boost_install: VERSION not given, assuming PROJECT_VERSION ('${PROJECT_VERSION}')")
      set(__VERSION ${PROJECT_VERSION})

    endif()

  endif()

  if(__UNPARSED_ARGUMENTS)

    message(AUTHOR_WARNING "boost_install: extra arguments ignored: ${__UNPARSED_ARGUMENTS}")

  endif()

  if(__HEADER_DIRECTORY AND NOT CMAKE_SKIP_INSTALL_RULES)

    get_filename_component(__HEADER_DIRECTORY "${__HEADER_DIRECTORY}" ABSOLUTE)
    install(DIRECTORY "${__HEADER_DIRECTORY}/" DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

  endif()

  foreach(target IN LISTS __TARGETS)

    boost_install_target(TARGET ${target} VERSION ${__VERSION} HEADER_DIRECTORY ${__HEADER_DIRECTORY})

  endforeach()

endfunction()
