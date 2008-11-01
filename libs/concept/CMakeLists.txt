#----------------------------------------------------------------------------
# This file was automatically generated from the original CMakeLists.txt file
# Add a variable to hold the headers for the library
set (lib_headers
    concept
)

# Add a library target to the build system
boost_library_project(
  concept
  #  SRCDIRS 
  #  TESTDIRS
  HEADERS ${lib_headers}
  #  DOCDIRS
  #  DESCRIPTION  "Helps Boost library developers adapt to compiler idiosyncrasies; not intended for library users."
  MODULARIZED 
  #  AUTHORS  "John Maddock <john -at- johnmaddock.co.uk>"
  #  MAINTAINERS 
)


