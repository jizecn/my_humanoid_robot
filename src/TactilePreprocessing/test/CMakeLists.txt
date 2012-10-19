# Copyright: (C) 2010 RoboSkin (EU FP7), Hertfordshire University
# Author: Ze Ji
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.6)

SET(KEYWORD "TactilePreprocessingTest")
PROJECT(${KEYWORD})

FIND_PACKAGE(YARP REQUIRED)

#optionally: use cmake find scripts provided by YARP and iCub
set(CMAKE_MODULE_PATH ${YARP_MODULE_PATH} ${CMAKE_MODULE_PATH})

FIND_PACKAGE(GSL REQUIRED)

LINK_LIBRARIES(${YARP_LIBRARIES})
LINK_LIBRARIES(${GSL_LIBRARIES})

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/../src/include)
INCLUDE_DIRECTORIES(${GSL_INCLUDE_DIRS})

# Search for source code.
FILE(GLOB folder_source test1.cpp ../src/TactileClassification.cpp ../src/TactileClassificationRawData.cpp ../src/TactileData.cpp ../src/svm.cpp)
FILE(GLOB folder_header ../src/include/*.h ../src/include/math/*.h)
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

ADD_EXECUTABLE(${KEYWORD} ${folder_source} ${folder_header})

target_link_libraries(${KEYWORD} ${YARP_LIBRARIES})