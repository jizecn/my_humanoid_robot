# Copyright: (C) 2010 RoboSkin, University of Hertfordshire
# Authors: Ze Ji
# CopyPolicy: Released under the terms of the GNU GPL v2.0.
## Based on the iCub file, originally by Lorenzo Natale

IF(WIN32)
cmake_minimum_required(VERSION 2.6)
#disable annoying warnings about use of deprecated c functions
ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE)
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:libcmt.lib;libcmtd.lib")

ELSE(WIN32)
cmake_minimum_required(VERSION 2.4)
ENDIF(WIN32)

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

PROJECT(KASPAR)

find_package(YARP REQUIRED)

# Pick up our CMake scripts - they are all in the conf subdirectory.

SET(KASPAR_DIR ${PROJECT_SOURCE_DIR} CACHE LOCATION "Where is KASPAR root?")
INCLUDE(${PROJECT_SOURCE_DIR}/KASPARConfig.cmake)
      
SET(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})

SET(BUILD_IN_ROOT TRUE CACHE BOOL "Do you want to build libs and bins in $KASPAR_ROOT/lib and $KASPAR_ROOT/bin?")
SET(USE_KASPAR_MOD FALSE CACHE BOOL "Use separated kaspar_mod lib")
#SET(COMPILE_FOR_TESTING FALSE CACHE BOOL "Set this if compiling for testing purposes")
#MARK_AS_ADVANCED(COMPILE_FOR_TESTING)  # not for general use

IF(BUILD_IN_ROOT)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
MAKE_DIRECTORY(${LIBRARY_OUTPUT_PATH})
MAKE_DIRECTORY(${EXECUTABLE_OUTPUT_PATH})
ENDIF(BUILD_IN_ROOT)

ADD_SUBDIRECTORY(src)
