cmake_minimum_required(VERSION 2.6)

project(PrepareDataForFakeSkin)

FIND_PACKAGE(YARP REQUIRED)
LINK_LIBRARIES(${YARP_LIBRARIES})
INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})

set(SOURCES main.cpp)

ADD_EXECUTABLE(prepareDataForFakeSkin ${SOURCES})
