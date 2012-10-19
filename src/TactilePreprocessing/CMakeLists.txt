# Copyright: (C) 2010 RoboSkin (EU FP7), Hertfordshire University
# Author: Ze Ji
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.6)

SET(KEYWORD "TactilePreprocessing")
PROJECT(${KEYWORD})

FIND_PACKAGE(YARP REQUIRED)

#optionally: use cmake find scripts provided by YARP and iCub
set(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})

FIND_PACKAGE(GSL REQUIRED)

LINK_LIBRARIES(${YARP_LIBRARIES})
LINK_LIBRARIES(${GSL_LIBRARIES})

#include_directories(${PROJECT_SOURCE_DIR}/include)

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/include)
INCLUDE_DIRECTORIES(${GSL_INCLUDE_DIRS})

# Search for source code.
FILE(GLOB folder_source src/*.cpp src/*.cc src/*.c)
FILE(GLOB folder_header src/include/*.h src/include/math/*.h)
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

ADD_EXECUTABLE(${KEYWORD} ${folder_source} ${folder_header})

target_link_libraries(${KEYWORD}  ${YARP_LIBRARIES})

#if(WIN32)
#	INSTALL(TARGETS ${KEYWORD} RUNTIME DESTINATION bin/release)	
#else(WIN32)
#	INSTALL(TARGETS ${KEYWORD} RUNTIME DESTINATION bin)
#endif(WIN32)
