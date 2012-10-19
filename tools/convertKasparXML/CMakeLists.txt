
cmake_minimum_required(VERSION 2.6)
project(CONVERTKASPARXML_FOR_YARP)
#SET(YARP_DIR "/home/ze/programming/roboskin/download/yarp0/trunk/yarp2/")

#FIND_PACKAGE(YARP REQUIRED)
#FIND_PACKAGE(KASPAR)
#LINK_LIBRARIES(${YARP_LIBRARIES})
#LINK_LIBRARIES(${KASPAR_LIBRARIES})


#IF(USE_KASPAR_MOD)
    #ADD_DEFINITIONS(-DUSE_KASPAR_MOD)
    #FIND_PACKAGE(kasparmod)
    #IMPORT_DEVICES("${KASPAR_DIR}/src/fsrtest/drivers.h" kasparmod)
#ENDIF (USE_KASPAR_MOD)

#INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})
#INCLUDE_DIRECTORIES(${KASPAR_INCLUDE_DIRS})

set(SOURCES main.cpp convertKasparXML.h convertKasparXML.cpp pugixml/pugixml.hpp pugixml/pugiconfig.hpp pugixml/pugixml.cpp pugixml/pugixpath.cpp)

ADD_EXECUTABLE(convertKasparXML ${SOURCES})
