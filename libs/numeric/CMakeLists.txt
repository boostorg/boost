#----------------------------------------------------------------------------
# This file was automatically generated from the original CMakeLists.txt file
# Add a variable to hold the headers for the library
set (lib_headers
    numeric
)

# Add a library target to the build system
boost_library_project(
  numeric
  #  SRCDIRS 
  TESTDIRS  conversion/test interval/test ublas/test
  HEADERS ${lib_headers}
  #  DOCDIRS 
  #  DESCRIPTION 
  MODULARIZED 
  #  AUTHORS 
  #  MAINTAINERS 
)


