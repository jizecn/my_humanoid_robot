# Copyright: (C) 2010 RoboSkin, Hertfordshire University
# Author: Ze Ji
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.6)

SET(KEYWORD "guiManager")
PROJECT(${KEYWORD})

FIND_PACKAGE(YARP REQUIRED)
FIND_PACKAGE(Qt4 REQUIRED)

LINK_LIBRARIES(${YARP_LIBRARIES})

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

IF(NOT QT4_FOUND)
   MESSAGE(FATAL_ERROR "No source code files found. Please add something")
ENDIF(NOT QT4_FOUND)

include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
   ${QT_INCLUDE_DIR}
   ${QT_QTNETWORK_INCLUDE_DIR}
   ${QT_QTTEST_INCLUDE_DIR}
)

# Search for source code.
FILE(GLOB folder_source src/*.cpp src/*.cc src/*.c src/gui/*.cpp src/gui/*.c src/gui/*.cxx src/utils/*.cpp src/utils/*.c src/utils/*.cxx include/pugixml/*.cpp)
FILE(GLOB folder_header include/*.h include/gui/*.h include/utils/*.h include/pugixml/*.hpp)
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

#set(qtproject_SRCS src/gui/MainFrame.cpp src/include/gui/MainFrame.h)
#qt4_automoc(${qtproject_SRCS})
SET(qt_SRCS
  src/gui/MainFrame.cpp
)

SET(qt_MOC_HDRS
include/gui/MainFrame.h
)
QT4_WRAP_CPP(qt_MOC_SRCS ${qt_MOC_HDRS})

IF (folder_source)
  ADD_EXECUTABLE(${KEYWORD}
	${qt_MOC_SRCS}
    ${folder_source}
    ${folder_header}
    )

  ELSE (folder_source)
    MESSAGE(FATAL_ERROR "No source code files found. Please add something")
  ENDIF (folder_source)

TARGET_LINK_LIBRARIES(${KEYWORD} ${QT_LIBRARIES})
