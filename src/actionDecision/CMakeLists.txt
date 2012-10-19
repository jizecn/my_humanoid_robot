# Copyright (C) 2010 Ze Ji
# University of Hertfordshire. 
# EU FP7 RoboSkin 
# CopyPolicy: Released under the terms of the GPL v2.0, see GPL.TXT

cmake_minimum_required(VERSION 2.6)

SET(KEYWORD "actionDecision")
project(${KEYWORD})


FIND_PACKAGE(YARP REQUIRED)
INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include/pugixml)

LINK_LIBRARIES(${YARP_LIBRARIES})

# Search for source code.
FILE(GLOB folder_source src/*.cpp src/*.cc src/*.c include/pugixml/*.cpp)
FILE(GLOB folder_header include/*.h include/gui/*.h include/utils/*.h include/pugixml/*.hpp)
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})


IF (folder_source)
  ADD_EXECUTABLE(${KEYWORD}  ${folder_source} ${folder_header})
ENDIF (folder_source)	
