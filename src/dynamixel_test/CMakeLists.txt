# Copyright: (C) 2009 RobotCub Consortium
# Authors: Paul Fitzpatrick, Giacomo Spigler
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.6)

#SET(YARP_DIR "/home/ze/programming/roboskin/download/yarp0/trunk/yarp2/")
FIND_PACKAGE(KASPAR)
FIND_PACKAGE(YARP REQUIRED)


IF(USE_KASPAR_MOD)
    ADD_DEFINITIONS(-DUSE_KASPAR_MOD)
    FIND_PACKAGE(kasparmod)
    IMPORT_DEVICES("${KASPAR_DIR}/src/dynamixel_test/drivers.h" kasparmod)
ENDIF (USE_KASPAR_MOD)

LINK_DIRECTORIES(${KASPAR_SOURCE_DIR})

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR})
INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})

LINK_LIBRARIES(${YARP_LIBRARIES})

ADD_EXECUTABLE(motortest motortest.cpp)

TARGET_LINK_LIBRARIES(motortest ${KASPAR_LIBRARIES})
