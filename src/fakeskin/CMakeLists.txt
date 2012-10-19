# Copyright (C) 2010 Ze Ji
# University of Hertfordshire. 
# EU FP7 RoboSkin 
# CopyPolicy: Released under the terms of the GPL v2.0, see GPL.TXT

cmake_minimum_required(VERSION 2.6)

project(FakeSkin)


FIND_PACKAGE(YARP REQUIRED)
INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})
LINK_LIBRARIES(${YARP_LIBRARIES})

set(SOURCES main.cpp FakeSkin.h Tokenizer.h)

ADD_EXECUTABLE(fakeSkin ${SOURCES})
