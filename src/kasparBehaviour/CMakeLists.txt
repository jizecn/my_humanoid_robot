# Copyright: (C) 2010 RoboSkin, Hertfordshire University
# Author: Ze Ji
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

cmake_minimum_required(VERSION 2.6)

SET(KEYWORD "kasparBehaviour")
PROJECT(${KEYWORD})
#ADD_DEFINITIONS(-DNODEFAULTLIB:library)
#SET(YARP_DIR "/home/ze/programming/roboskin/download/yarp0/trunk/yarp2/")


FIND_PACKAGE(YARP REQUIRED)
FIND_PACKAGE(Qt4 REQUIRED)
FIND_PACKAGE(ACE REQUIRED)
FIND_PACKAGE(SDL REQUIRED)
FIND_PACKAGE(GSL REQUIRED)

LINK_LIBRARIES(${YARP_LIBRARIES})
LINK_LIBRARIES(${ACE_LIBRARIES})
LINK_LIBRARIES(${SDL_LIBRARY})
LINK_LIBRARIES(${SDLMAIN_LIBRARY})
LINK_LIBRARIES(${GSL_LIBRARY})

IF(USE_KASPAR_MOD)
    ADD_DEFINITIONS(-DUSE_KASPAR_MOD)
    FIND_PACKAGE(kasparmod)
    IMPORT_DEVICES("${KASPAR_DIR}/src/kasparBehaviour/src/include/drivers.h" kasparmod)
ENDIF (USE_KASPAR_MOD)

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/include)
INCLUDE_DIRECTORIES(${ACE_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${GSL_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR})



# if defined, should include Tokenizer_T.h, instead of sstring.h only, for using ACE_tokenizer
#INCLUDE (CheckIncludeFiles)
#CHECK_INCLUDE_FILES (malloc.h HAVE_TOKENIZER_T_H)
###### The ABOVE does not work. So, temporarily, use IF(WIN32) as my windows machines uses ACE 5.6 which does not have Tokenizer_T.h. My Ubuntu does. 
IF(WIN32)
ELSE(WIN32)
ADD_DEFINITIONS(-DHAVE_TOKENIZER_T_H)
ENDIF(WIN32)

IF(NOT QT4_FOUND)
#  LINK_LIBRARIES(${GTKPLUS_LINK_FLAGS})
    MESSAGE(FATAL_ERROR "No source code files found. Please add something")
ENDIF(NOT QT4_FOUND)

include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(
   ${QT_INCLUDE_DIR}
   ${QT_QTNETWORK_INCLUDE_DIR}
   ${QT_QTTEST_INCLUDE_DIR}
)

#ADD_SUBDIRECTORY(src/gui)
#SET(qtproject_UIS  gui/mywindow.ui)
#include_directories(${PROJECT_SOURCE_DIR}/src/include)
#QT4_WRAP_UI(qtproject_UIS_H ${qtproject_UIS})

# Search for source code.
FILE(GLOB folder_source src/*.cpp src/*.cc src/*.c src/behaviour/*.cpp src/behaviour/*.cxx src/behaviour/*.c src/gui/*.cpp src/gui/*.c src/gui/*.cxx src/utils/*.cpp src/utils/*.c src/utils/*.cxx src/include/pugixml/*.cpp)
FILE(GLOB folder_header src/include/*.h src/include/gui/*.h src/include/utils/*.h src/include/pugixml/*.hpp src/include/signalprocess/*.h)
SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

#set(qtproject_SRCS src/gui/MainFrame.cpp src/include/gui/MainFrame.h)
#qt4_automoc(${qtproject_SRCS})
SET(qt_SRCS
  src/gui/MainFrame.cpp
  src/gui/ModeRadioButton.cpp
)

SET(qt_MOC_HDRS
src/include/gui/MainFrame.h
src/include/gui/ModeRadioButton.h
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

#ADD_EXECUTABLE(kasparGUIYarp src/main.cpp)
TARGET_LINK_LIBRARIES(${KEYWORD} ${QT_LIBRARIES})
