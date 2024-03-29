
# Copyright: (C) 2009 RobotCub Consortium
# Authors: Lorenzo Natale and Vadim Tikhanoff
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

# Options:
# USE_ODE_DOUBLE  -use double precision ode libraries
# ODE_STATIC      -link against static libraries
#
# On exit create the following variables:
# ODE_INCLUDE_DIRS  - Directories to include to use ODE
# ODE_LIBRARIES     - Default library to link against to use ODE
# ODE_FOUND         - If false, library not found


# Dec 2009, reorganized to handle new windows ode versions.

# find (all) header files for include directories:
IF (NOT ODE_DIR)
	SET (ODE_ENV_DIR $ENV{ODE_DIR})
	IF (ODE_ENV_DIR)
		FIND_PATH(ODE_DIR include/ode/ode.h "$ENV{ODE_DIR}")
	ELSE (ODE_ENV_DIR)
		FIND_PATH(ODE_DIR include/ode/ode.h ${CMAKE_PROJECT_DIR})
	ENDIF (ODE_ENV_DIR)
ENDIF (NOT ODE_DIR)

FIND_PATH(ODE_INCLUDE_DIRS ode/ode.h "${ODE_DIR}/include")
MARK_AS_ADVANCED(ODE_INCLUDE_DIRS)

SET (ODE_SYSTEM_LIBS)
SET (ODE_SYSTEM_LIBS_FOUND FALSE)
SET (ODE_FOUND FALSE)

IF (MSVC)
  SET(ODE_STATIC TRUE CACHE BOOL "Link ODE as static library? Warning in windows, to coexist with YARP you need to set this to FALSE")

  #search static and dynamic libs, single and double precision
  FIND_LIBRARY(ODE_MAIN_STATIC_DOUBLE_DB 
                NAMES ode ode_doubled 
                PATHS "${ODE_DIR}/lib/debuglib" "${ODE_DIR}/lib/debugdoublelib")
  
  FIND_LIBRARY(ODE_MAIN_STATIC_DOUBLE_RE 
               NAMES ode ode_double 
               PATHS "${ODE_DIR}/lib/releaselib" "${ODE_DIR}/lib/releasedoublelib")

  FIND_LIBRARY(ODE_MAIN_DLL_DOUBLE_DB 
               NAMES ode ode_doubled 
               PATHS "${ODE_DIR}/lib/debugdll"	"${ODE_DIR}/lib/debugdoubledll")

  FIND_LIBRARY(ODE_MAIN_DLL_DOUBLE_RE 
                NAMES ode ode_double 
                PATHS "${ODE_DIR}/lib/releasedll" "${ODE_DIR}/lib/releasedoubledll")

  FIND_LIBRARY(ODE_MAIN_STATIC_SINGLE_DB 
                NAMES ode ode_singled 
                PATHS "${ODE_DIR}/lib/debuglib" "${ODE_DIR}/lib/debugsinglelib")

  FIND_LIBRARY(ODE_MAIN_STATIC_SINGLE_RE 
                NAMES ode ode_single 
                PATHS "${ODE_DIR}/lib/releaselib" "${ODE_DIR}/lib/releasesinglelib")

  FIND_LIBRARY(ODE_MAIN_DLL_SINGLE_DB 
                NAMES ode ode_singled 
                PATHS "${ODE_DIR}/lib/debugdll" "${ODE_DIR}/lib/debugsingledll")

  FIND_LIBRARY(ODE_MAIN_DLL_SINGLE_RE 
                NAMES ode ode_single 
                PATHS "${ODE_DIR}/lib/releasedll" "${ODE_DIR}/lib/releasesingledll")
 
  MARK_AS_ADVANCED(ODE_MAIN_STATIC_SINGLE_DB 
                   ODE_MAIN_STATIC_SINGLE_RE
                   ODE_MAIN_STATIC_DOUBLE_DB
				   ODE_MAIN_STATIC_DOUBLE_RE)
  MARK_AS_ADVANCED(ODE_MAIN_DLL_DOUBLE_DB 
                   ODE_MAIN_DLL_DOUBLE_RE
                   ODE_MAIN_DLL_SINGLE_DB
				   ODE_MAIN_DLL_SINGLE_RE)

  SET (ODE_SYSTEM_LIBS opengl32 glu32)
  SET (ODE_SYSTEM_LIBS_FOUND TRUE)

#  IF (ODE_MAIN_STATIC_DB OR ODE_MAIN_LIBRARY_DB OR ODE_MAIN_STATIC_RE OR ODE_MAIN_LIBRARY_RE)
#	MARK_AS_ADVANCED(CLEAR ODE_STATIC)
#  ELSE (ODE_MAIN_STATIC_DB OR ODE_MAIN_LIBRARY_DB OR ODE_MAIN_STATIC_RE OR ODE_MAIN_LIBRARY_RE)
#	MARK_AS_ADVANCED(ODE_STATIC)
#  ENDIF (ODE_MAIN_STATIC_DB OR ODE_MAIN_LIBRARY_DB OR ODE_MAIN_STATIC_RE OR ODE_MAIN_LIBRARY_RE)

  IF(ODE_STATIC AND NOT USE_ODE_DOUBLE)
	IF (ODE_MAIN_STATIC_SINGLE_RE AND ODE_MAIN_STATIC_SINGLE_DB AND ODE_SYSTEM_LIBS_FOUND AND ODE_INCLUDE_DIRS)
	  SET (ODE_FOUND TRUE)
	  SET (ODE_LIBRARIES "${ODE_SYSTEM_LIBS};optimized;${ODE_MAIN_STATIC_SINGLE_RE};debug;${ODE_MAIN_STATIC_SINGLE_DB};")
	ENDIF (ODE_MAIN_STATIC_SINGLE_RE AND ODE_MAIN_STATIC_SINGLE_DB AND ODE_SYSTEM_LIBS_FOUND AND ODE_INCLUDE_DIRS)
  ENDIF(ODE_STATIC AND NOT USE_ODE_DOUBLE)
  
  IF (ODE_STATIC AND USE_ODE_DOUBLE)
  	IF (ODE_MAIN_STATIC_DOUBLE_RE AND ODE_MAIN_STATIC_DOUBLE_DB AND ODE_SYSTEM_LIBS_FOUND AND ODE_INCLUDE_DIRS)
	  SET (ODE_FOUND TRUE)
	  SET (ODE_LIBRARIES "${ODE_SYSTEM_LIBS};optimized;${ODE_MAIN_STATIC_DOUBLE_RE};debug;${ODE_MAIN_STATIC_DOUBLE_DB};")
	ENDIF (ODE_MAIN_STATIC_DOUBLE_RE AND ODE_MAIN_STATIC_DOUBLE_DB AND ODE_SYSTEM_LIBS_FOUND AND ODE_INCLUDE_DIRS)
  ENDIF(ODE_STATIC AND USE_ODE_DOUBLE)
  
  IF(NOT ODE_STATIC AND NOT USE_ODE_DOUBLE)
	IF (ODE_MAIN_DLL_SINGLE_RE AND ODE_MAIN_DLL_SINGLE_DB AND ODE_SYSTEM_LIBS_FOUND AND ODE_INCLUDE_DIRS)
	  SET (ODE_FOUND TRUE)
	  SET (ODE_LIBRARIES "${ODE_SYSTEM_LIBS};optimized;${ODE_MAIN_DLL_SINGLE_RE};debug;${ODE_MAIN_DLL_SINGLE_DB};")
	ENDIF (ODE_MAIN_DLL_SINGLE_RE AND ODE_MAIN_DLL_SINGLE_DB AND ODE_SYSTEM_LIBS_FOUND AND ODE_INCLUDE_DIRS)
  ENDIF(NOT ODE_STATIC AND NOT USE_ODE_DOUBLE)
  
  IF(NOT ODE_STATIC AND USE_ODE_DOUBLE)
	IF (ODE_MAIN_DLL_DOUBLE_RE AND ODE_MAIN_DLL_DOUBLE_DB AND ODE_SYSTEM_LIBS_FOUND AND ODE_INCLUDE_DIRS)
	  SET (ODE_FOUND TRUE)
	  SET (ODE_LIBRARIES "${ODE_SYSTEM_LIBS};optimized;${ODE_MAIN_DLL_DOUBLE_RE};debug;${ODE_MAIN_DLL_DOUBLE_DB};")
	ENDIF (ODE_MAIN_DLL_DOUBLE_RE AND ODE_MAIN_DLL_DOUBLE_DB AND ODE_SYSTEM_LIBS_FOUND AND ODE_INCLUDE_DIRS)
  ENDIF(NOT ODE_STATIC AND USE_ODE_DOUBLE)

ELSE (MSVC) 

  FIND_LIBRARY(ODE_MAIN_LIBRARY ode libode "${ODE_DIR}/lib" "${ODE_DIR}/ode/src" "${ODE_DIR}/ode/src/.libs")
  MARK_AS_ADVANCED(ODE_MAIN_LIBRARY)

  FIND_PACKAGE(OpenGL)
  IF (NOT OPENGL_FOUND)
	MESSAGE(STATUS "Cannot find OpenGL, but ODE needs it")
  ENDIF (NOT OPENGL_FOUND)
  SET (ODE_SYSTEM_LIBS ${OPENGL_LIBRARIES})
  SET (ODE_SYSTEM_LIBS_FOUND ${OPENGL_FOUND})

  IF (ODE_MAIN_LIBRARY AND ODE_SYSTEM_LIBS_FOUND AND ODE_INCLUDE_DIRS)
	SET (ODE_FOUND TRUE)
	SET (ODE_LIBRARIES ${ODE_MAIN_LIBRARY} ${ODE_SYSTEM_LIBS})
	#MESSAGE (STATUS "ODE libraries: ${ODE_LIBRARIES}")
  ENDIF (ODE_MAIN_LIBRARY AND ODE_SYSTEM_LIBS_FOUND AND ODE_INCLUDE_DIRS)
  
ENDIF (MSVC)

IF (ODE_FOUND)
    IF (NOT ODE_FIND_QUIETLY)
        MESSAGE(STATUS "Found ODE: ${ODE_LIBRARIES}")
    ENDIF (NOT ODE_FIND_QUIETLY)
ELSE (ODE_FOUND)
    IF (ODE_FIND_REQUIRED OR ODE_DIR)
	IF (ODE_DIR)
	        MESSAGE(STATUS "ODE library not found, check ODE_DIR")
	ELSE (ODE_DIR)
	        MESSAGE(FATAL_ERROR "ODE library not found, check ODE_DIR")
	ENDIF (ODE_DIR)
    ENDIF (ODE_FIND_REQUIRED OR ODE_DIR)
ENDIF (ODE_FOUND)




