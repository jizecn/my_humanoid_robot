# Copyright: (C) 2010 RoboSkin Consortium

#### Fork from ICUB Repository for KASPAR in ROBOSKIN PROJECT at UH. by Ze Ji
#######################
# Authors: Paul Fitzpatrick, Giorgio Metta, Lorenzo Natale
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

PROJECT (KASPAR)

# Check for the minimal set of packages required for the global project.
# If we include them here, sub-projects can be configured a bit faster.

FIND_PACKAGE(YARP REQUIRED)

IF (NOT YARP_HAS_MATH_LIB)
  MESSAGE(SEND_ERROR "libYARP_math not found!")
  MESSAGE(FATAL_ERROR "Missing libYARP_math -- see the manual for instructions")
ENDIF (NOT YARP_HAS_MATH_LIB)

#FIND_PACKAGE(OpenCV QUIET)
#IF (NOT OPENCV_FOUND)
#  MESSAGE(STATUS "Skipping modules using OpenCV (Open Computer Vision)")
#ENDIF (NOT OPENCV_FOUND)

FIND_PACKAGE(GSL QUIET)
IF (NOT GSL_FOUND)
  MESSAGE(STATUS "Skipping modules using GSL (GNU Scientific Library)")
ENDIF (NOT GSL_FOUND)

#FIND_PACKAGE(ODE QUIET)
#IF (NOT ODE_FOUND)
# MESSAGE(STATUS "Skipping modules using ODE (Open Dynamics Engine)")
#ENDIF (NOT ODE_FOUND)

FIND_PACKAGE(SDL QUIET)

# SDL_FOUND variable seems unreliable!
IF (SDL_LIBRARY)
 SET(SDL_FOUND "YES")
ENDIF (SDL_LIBRARY)
IF (NOT SDL_FOUND)
 MESSAGE(STATUS "Skipping modules using SDL (Simple DirectMedia Library)")
ENDIF (NOT SDL_FOUND)

FIND_PACKAGE(Qt4 QUIET)
IF (NOT QT4_FOUND)
MESSAGE(STATUS "Skipping modules using QT4")
ENDIF (NOT QT4_FOUND)

# Formal releases are starting.
# In releases, the "BUILD_FOR_RELEASE" flag is set.
#SET(BUILD_FOR_RELEASE FALSE CACHE BOOL "Is this an official build?")
#MARK_AS_ADVANCED(BUILD_FOR_RELEASE)  # not for general use

# kaspar modules.
IF (USE_KASPAR_MOD)
  ADD_SUBDIRECTORY(modules)
ENDIF (USE_KASPAR_MOD)

ADD_SUBDIRECTORY(TactilePreprocessing)
ADD_SUBDIRECTORY(dynamixel_test)
#ADD_SUBDIRECTORY(fsrtest)
ADD_SUBDIRECTORY(FirstIITSensorTest)

IF (QT4_FOUND AND SDL_FOUND AND GSL_FOUND)
ADD_SUBDIRECTORY(kasparBehaviour)
ADD_SUBDIRECTORY(kasparBehaviour2)
ELSE (QT4_FOUND AND SDL_FOUND AND GSL_FOUND)
MESSAGE("kasparBehaviour modules are not included due to Qt4 not found ")
ENDIF(QT4_FOUND AND SDL_FOUND AND GSL_FOUND)
IF (SDL_FOUND)
ADD_SUBDIRECTORY(testsound)
ELSE (SDL_FOUND)
MESSAGE("kasparBehaviour modules are not included due to Qt4 not found ")
ENDIF(SDL_FOUND)

ADD_SUBDIRECTORY(tactile_saliency)
ADD_SUBDIRECTORY(fakeskin)
ADD_SUBDIRECTORY(datalogger)
ADD_SUBDIRECTORY(actionDecision)
ADD_SUBDIRECTORY(actionExecutor)

IF (QT4_FOUND)
ADD_SUBDIRECTORY(guiManager)
ELSE(QT4_FOUND)
MESSAGE(STATUS "Skipping modules using QT4")
ENDIF (QT4_FOUND)
