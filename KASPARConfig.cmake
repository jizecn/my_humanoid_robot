# Copyright: (C) 2010 RoboSkin Consortium
# Author: Ze Ji. 
# based on the iCub system, with slight modification and simplification
###############
# Original Authors: Paul Fitzpatrick, Giorgio Metta, Lorenzo Natale
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

# If KASPAR_FOUND is set, we have already done all tbhis
IF (NOT KASPAR_FOUND)

#Changing the default CMAKE_INSTALL_PREFIX
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  SET(CMAKE_INSTALL_PREFIX
    "${KASPAR_DIR}" CACHE PATH "KASPAR install prefix" FORCE
    )
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

MESSAGE(STATUS "setting up KASPAR")

# pick up Kaspar's helper modules
SET(KASPAR_MODULE_PATH "${KASPAR_DIR}/conf")
SET(KASPAR_SOURCE_DIR ${KASPAR_DIR})
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${KASPAR_MODULE_PATH})

# some conveniences
INCLUDE(UsePkgConfig)

SET(KASPAR_FOUND TRUE)
ENDIF (NOT KASPAR_FOUND)
