# Copyright (C) 2010 Ze Ji
# University of Hertfordshire. 
# EU FP7 RoboSkin 
# CopyPolicy: Released under the terms of the GPL v2.0, see GPL.TXT

cmake_minimum_required(VERSION 2.6)

SET(KEYWORD "testsound")
project(${KEYWORD})

FIND_PACKAGE(YARP REQUIRED)
FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(ACE REQUIRED)

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})
LINK_LIBRARIES(${YARP_LIBRARIES})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${ACE_INCLUDE_DIRS})
LINK_LIBRARIES(${ACE_LIBRARIES})
LINK_LIBRARIES(${SDL_LIBRARY})
LINK_LIBRARIES(${SDLMAIN_LIBRARY})

INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})


# Search for source code.
FILE(GLOB folder_source src/*.cpp src/*.cc src/*.c)
FILE(GLOB folder_header include/*.h include/utils/*.h)
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

ADD_EXECUTABLE(${KEYWORD}  ${folder_source} ${folder_header})
TARGET_LINK_LIBRARIES(${KEYWORD} ${SDL_LIBRARY} ${SDLMAIN_LIBRARY})