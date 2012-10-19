
cmake_minimum_required(VERSION 2.6)
project(CREATEKAYMAPXML)

FIND_PACKAGE(YARP REQUIRED)
#FIND_PACKAGE(ACE REQUIRED)

INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})

LINK_LIBRARIES(${YARP_LIBRARIES})

set(SOURCES main.cpp createKeymapXML.h createKeymapXML.cpp Tokenizer.h Tokenizer.cpp
pugixml/pugixml.hpp pugixml/pugiconfig.hpp pugixml/pugixml.cpp pugixml/pugixpath.cpp)

ADD_EXECUTABLE(createKeymapXML ${SOURCES})
