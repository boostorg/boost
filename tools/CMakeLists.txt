#
# Copyright Troy D. Straszheim
#
# Distributed under the Boost Software License, Version 1.0.
# See http://www.boost.org/LICENSE_1_0.txt
#
# return a list of directories that we should add_subdirectory()
macro(boost_collect_lib_dependencies varname filename)
  #message(STATUS "boost_collect_lib_dependencies.... ${Boost_SOURCE_DIR}/libs")
  file(GLOB BOOST_LIBRARY_CMAKE_FILES 
       RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${Boost_SOURCE_DIR}/libs/*/${filename}")
  foreach(BOOST_LIB_CMAKE_FILE ${BOOST_LIBRARY_CMAKE_FILES})
    #message(STATUS "-- BOOST_LIB_CMAKE_FILE: ${BOOST_LIB_CMAKE_FILE}")
    get_filename_component(BOOST_LIB_DIR ${BOOST_LIB_CMAKE_FILE} PATH)
    set(${varname} ${${varname}} ${BOOST_LIB_DIR})
  endforeach(BOOST_LIB_CMAKE_FILE ${BOOST_LIBRARY_CMAKE_FILES})
endmacro(boost_collect_lib_dependencies varname)


# Find all of the subdirectories with .cmake files in them. These are
# the libraries with dependencies.
boost_collect_lib_dependencies(BOOST_MODULE_DIRS "module.cmake")
foreach(subdir ${BOOST_MODULE_DIRS})
#  message(STATUS "${Boost_SOURCE_DIR}/libs/${subdir}/module.cmake")
  include("${Boost_SOURCE_DIR}/libs/${subdir}/module.cmake")
endforeach(subdir)


###############################################################################
# This macro is an internal utility macro 
# TODO: Document this if it stays around
#
#
# example usage:
#  boost_tool_dependencies( BOOST_DEPENDS test)
#
macro(boost_tool_dependencies)
  parse_arguments(BOOST_TEST 
    "BOOST_DEPENDS"
    ""
    ${ARGN}
  )
    set (THIS_TEST_DEPENDS_ALL "")
    # message (STATUS "BOOST_TEST_BOOST_DEPENDS: ${BOOST_TEST_BOOST_DEPENDS}")
    foreach(libname ${BOOST_TEST_BOOST_DEPENDS})
       # message(STATUS "libname: ${libname}")
        string(TOUPPER "BOOST_${libname}_DEPENDS" THIS_PROJECT_DEPENDS)
      #  message(STATUS "${THIS_PROJECT_DEPENDS}: ${${THIS_PROJECT_DEPENDS}}")
       # set(THIS_TEST_DEPENDS_ALL ${libname} ${${THIS_PROJECT_DEPENDS}} )
      #  message(STATUS "${THIS_TEST_DEPENDS_ALL}: ${${THIS_TEST_DEPENDS_ALL}}")
        
       list(FIND THIS_TEST_DEPENDS_ALL ${libname} DEPDEP_INDEX)
       if (DEPDEP_INDEX EQUAL -1)
         list(APPEND THIS_TEST_DEPENDS_ALL ${libname})
         set(ADDED_DEPS TRUE)
       endif()
        string(TOUPPER "BOOST_${libname}_DEPENDS" THIS_PROJECT_DEPENDS)
       # message(STATUS "${additional_lib}: ===> ${${THIS_PROJECT_DEPENDS}}")
        set(ADDED_DEPS TRUE)
        while (ADDED_DEPS)
          set(ADDED_DEPS FALSE)
          foreach(DEP ${THIS_TEST_DEPENDS_ALL})
            string(TOUPPER "BOOST_${DEP}_DEPENDS" DEP_DEPENDS)
            foreach(DEPDEP ${${DEP_DEPENDS}})
              list(FIND THIS_TEST_DEPENDS_ALL ${DEPDEP} DEPDEP_INDEX)
              if (DEPDEP_INDEX EQUAL -1)
                list(APPEND THIS_TEST_DEPENDS_ALL ${DEPDEP})
                set(ADDED_DEPS TRUE)
              endif()
            endforeach()
          endforeach()
        endwhile()
      # message(STATUS "->  Dependencies for ${libname}")
      # message(STATUS "->  THIS_TEST_DEPENDS_ALL: ${THIS_TEST_DEPENDS_ALL}")  
        
    endforeach(libname ${BOOST_TEST_BOOST_DEPENDS})
   foreach (include ${THIS_TEST_DEPENDS_ALL})
      #message(STATUS "include: ${include}")
      include_directories("${Boost_SOURCE_DIR}/libs/${include}/include")
    endforeach (include ${includes})

endmacro(boost_tool_dependencies)
#
###############################################################################

# add_subdirectory(quickbook)
# add_subdirectory(wave)
add_subdirectory(bcp)
add_subdirectory(inspect)