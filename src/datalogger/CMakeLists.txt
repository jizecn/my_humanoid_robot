# Copyright: (C) 2010 RoboSkin, Hertfordshire University
# Author: Ze Ji
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.6)

SET(KEYWORD "dataLogger")
PROJECT(${KEYWORD})
#ADD_DEFINITIONS(-DNODEFAULTLIB:library)
#SET(YARP_DIR "/home/ze/programming/roboskin/download/yarp0/trunk/yarp2/")

FIND_PACKAGE(YARP REQUIRED)
#FIND_PACKAGE(KASPAR)
#FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(ACE)
#FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(GSL REQUIRED)

LINK_LIBRARIES(${YARP_LIBRARIES})
LINK_LIBRARIES(${ACE_LIBRARIES})
LINK_LIBRARIES(${GSL_LIBRARY})

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/include)
INCLUDE_DIRECTORIES(${ACE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${GSL_INCLUDE_DIRS})

#ADD_SUBDIRECTORY(src/gui)
#SET(qtproject_UIS  gui/mywindow.ui)
#include_directories(${PROJECT_SOURCE_DIR}/src/include)
#QT4_WRAP_UI(qtproject_UIS_H ${qtproject_UIS})

# Search for source code.
FILE(GLOB folder_source src/*.cpp src/*.cc src/*.c src/behaviour/*.cpp src/behaviour/*.cxx src/behaviour/*.c src/gui/*.cpp src/gui/*.c src/gui/*.cxx src/utils/*.cpp src/utils/*.c src/utils/*.cxx src/include/pugixml/*.cpp)
FILE(GLOB folder_header src/include/*.h src/include/gui/*.h src/include/utils/*.h src/include/pugixml/*.hpp)
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

#set(qtproject_SRCS src/gui/MainFrame.cpp src/include/gui/MainFrame.h)
#qt4_automoc(${qtproject_SRCS})

IF (folder_source)
  ADD_EXECUTABLE(${KEYWORD}
    ${folder_source}
    ${folder_header}
    )

  ELSE (folder_source)
    MESSAGE(FATAL_ERROR "No source code files found. Please add something")
ENDIF (folder_source)

#ADD_EXECUTABLE(kasparGUIYarp src/main.cpp)
#TARGET_LINK_LIBRARIES(${KEYWORD})
