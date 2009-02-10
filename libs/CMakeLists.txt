# Find each subdirectory containing a CMakeLists.txt file, and include
# it. This avoids the need to manually list which libraries in Boost
# have CMakeLists.txt files.

# return a list of directories that we should add_subdirectory()
macro(BOOST_COLLECT_SUBPROJECT_DIRECTORY_NAMES varname filename)
  file(GLOB BOOST_LIBRARY_CMAKE_FILES 
    RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "*/${filename}")
  foreach(BOOST_LIB_CMAKE_FILE ${BOOST_LIBRARY_CMAKE_FILES})
    get_filename_component(BOOST_LIB_DIR ${BOOST_LIB_CMAKE_FILE} PATH)
    set(${varname} ${${varname}} ${BOOST_LIB_DIR})
  endforeach(BOOST_LIB_CMAKE_FILE ${BOOST_LIBRARY_CMAKE_FILES})
endmacro(BOOST_COLLECT_SUBPROJECT_DIRECTORY_NAMES varname)

macro(ADD_SUBDIRECTORIES prefix)
  foreach(subdir ${ARGN})
    message(STATUS "${prefix}${subdir}")
    add_subdirectory(${subdir})
  endforeach(subdir ${ARGN})
endmacro(ADD_SUBDIRECTORIES prefix)

# Find all of the subdirectories with .cmake files in them. These are
# the libraries with dependencies.
boost_collect_subproject_directory_names(BOOST_MODULE_DIRS "module.cmake")
foreach(subdir ${BOOST_MODULE_DIRS})
  include("${CMAKE_CURRENT_SOURCE_DIR}/${subdir}/module.cmake")
endforeach(subdir)

# Find all of the subdirectories with CMakeLists.txt files in
# them. This contains all of the Boost libraries.
boost_collect_subproject_directory_names(BOOST_SUBPROJECT_DIRS "CMakeLists.txt")

# Add all of the Boost projects in reverse topological order, so that
# a library's dependencies show up before the library itself.
set(CPACK_INSTALL_CMAKE_COMPONENTS_ALL)
list(SORT BOOST_SUBPROJECT_DIRS)
topological_sort(BOOST_SUBPROJECT_DIRS BOOST_ _DEPENDS)
add_subdirectories(" + " ${BOOST_SUBPROJECT_DIRS})

# Write out a GraphViz file containing inter-library dependencies. 
set(BOOST_DEPENDENCY_GRAPHVIZ_FILE "${Boost_BINARY_DIR}/dependencies.dot")
file(WRITE ${BOOST_DEPENDENCY_GRAPHVIZ_FILE} "digraph boost {\n")
foreach(SUBDIR ${BOOST_SUBPROJECT_DIRS})
  string(TOUPPER "BOOST_${SUBDIR}_COMPILED_LIB" BOOST_COMPILED_LIB_VAR)
  if (${BOOST_COMPILED_LIB_VAR})
    file(APPEND ${BOOST_DEPENDENCY_GRAPHVIZ_FILE} "  \"${SUBDIR}\" [style=\"filled\" fillcolor=\"#A3A27C\" shape=\"box\"];\n ")
  endif (${BOOST_COMPILED_LIB_VAR})
  string(TOUPPER "BOOST_${SUBDIR}_DEPENDS" DEPENDS_VAR)
  if(DEFINED ${DEPENDS_VAR})
    foreach(DEP ${${DEPENDS_VAR}})
      file(APPEND ${BOOST_DEPENDENCY_GRAPHVIZ_FILE} 
        "  \"${SUBDIR}\" -> \"${DEP}\";\n")
    endforeach()
  endif()
endforeach()
file(APPEND ${BOOST_DEPENDENCY_GRAPHVIZ_FILE} "  \"test\" [style=\"filled\" fillcolor=\"#A3A27C\" shape=\"box\"];\n ")
file(APPEND ${BOOST_DEPENDENCY_GRAPHVIZ_FILE} "}\n")
