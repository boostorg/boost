##########################################################################
# Boost Build Slave Support                                              #
##########################################################################
# Copyright (C) 2008 Troy D. Straszheim                                  #
#                                                                        #
# Distributed under the Boost Software License, Version 1.0.             #
# See accompanying file LICENSE_1_0.txt or copy at                       #
#   http://www.boost.org/LICENSE_1_0.txt                                 #
##########################################################################
#
# Quick configuration of build slaves. 
# 
# 1. Copy this file to your (empty, newly created) build directory
# 2. Customize below where you see CUSTOMIZE
# 3. Use this file to populate your build directory.  From the build
#    directory execute:
#     
#      cmake -C path/to/this/file ../path/to/source/directory   
# 
#    e.g.
#  
#      cmake -C BuildSlave.cmake ../src
#

message (STATUS "Reading initial cache for build slaves.")

# 
#  CUSTOMIZE
#
set(BOOST_BUILD_SLAVE_CONTACT_INFO "buildmeister@example.com"
    CACHE STRING "who to contact with questions" FORCE)

set(BOOST_BUILD_SLAVE_HOSTNAME "descriptive.name.of.host.example.com"
    CACHE STRING "descriptive hostname" FORCE)

# 
#  CUSTOMIZE: Either set this to the path of an existing file 
#  (relative to build directory) or create the file slave-description.txt
#
set(BOOST_BUILD_SLAVE_DETAILS_FILE "slave-description.txt"
    CACHE STRING "file containing details about the build/platform" FORCE)

#
#   Below this line oughtn't require customization.
#
if(EXISTS ${BOOST_BUILD_SLAVE_DETAILS_FILE})
  message(STATUS "Will take build details from ${BOOST_BUILD_SLAVE_DETAILS_FILE}")
else(EXISTS ${BOOST_BUILD_SLAVE_DETAILS_FILE})
  message(FATAL_ERROR "Please configure BOOST_BUILD_SLAVE_DETAILS_FILE (${BOOST_BUILD_SLAVE_DETAILS_FILE}) and create this file")	    
endif(EXISTS ${BOOST_BUILD_SLAVE_DETAILS_FILE})

if(WIN32)
  set(CMAKE_GENERATOR "NMake Makefiles" CACHE INTERNAL "generator" FORCE)
  set(CMAKE_MAKE_PROGRAM "nmake" CACHE INTERNAL "nmake" FORCE)
endif(WIN32)

set(BUILD_TESTING ON
    CACHE BOOL "build testing" FORCE)

set(BOOST_BUILD_SLAVE ON 
    CACHE BOOL "build slave mode" FORCE)



