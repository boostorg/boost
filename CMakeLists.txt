# version 3.8 was chosen, because it provides the cxx_std_XX compile features
# https://cmake.org/cmake/help/v3.8/prop_gbl/CMAKE_CXX_KNOWN_FEATURES.html
cmake_minimum_required(VERSION 3.8)

project(boost LANGUAGES CXX)

# Not all boost libraries have native support for cmake yet.
# A quick and dirty way to work around this (at least partially) is
# to resolve missing dependencies via a boost installation known to
# cmake itself
# find_package(Boost REQUIRED COMPONENTS <library list>)

message("================= BOOST WARNING ================================================================== ")
message("=                                                                                                = ")
message("= Cmake support in boost is experimental and only a few libraries can be build via cmake at all. = ")
message("=                                                                                                = ")
message("= The supported way to build and test boost is still b2, as e.g. explained here:                 = ")
message("= https://www.boost.org/doc/libs/1_68_0/more/getting_started/index.html                          = ")
message("=                                                                                                = ")
message("================================================================================================== ")

#Detect and process all CMakeLists files that reside in the root folder of a library
file(GLOB boost_lib_cmake_files libs/*/CMakeLists.txt)

foreach(cmake_file IN ITEMS ${boost_lib_cmake_files})

	 get_filename_component(lib ${cmake_file} DIRECTORY)

	 message("Adding boost library: ${lib}")
	 add_subdirectory(${lib})

endforeach()

#Detect and process all CMakeLists files that reside in the root folder of a tool
file(GLOB boost_tool_cmake_files tools/*/CMakeLists.txt)

foreach(cmake_file IN ITEMS ${boost_tool_cmake_files})

	 get_filename_component(lib ${cmake_file} DIRECTORY)

	 message("Adding boost tool: ${lib}")
	 add_subdirectory(${lib})

endforeach()

