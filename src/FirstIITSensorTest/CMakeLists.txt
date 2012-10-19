SET(KEYWORD "iitSensorLog")

PROJECT(${KEYWORD})

FIND_PACKAGE(KASPAR)
FIND_PACKAGE(YARP REQUIRED)
FIND_PACKAGE(GSL)

INCLUDE_DIRECTORIES(${GSL_INCLUDE_DIR})
INCLUDE_DIRECTORIES(${YARP_INCLUDE_DIRS})

SET(USE_KASPAR_MOD TRUE CACHE BOOL "Use separated kaspar_mod lib")

#---------------------------------------
IF(NOT GtkPlus_FOUND)
  FIND_PACKAGE(GtkPlus)
#  FIND_PACKAGE(GTHREAD)
ENDIF (NOT GtkPlus_FOUND)

IF (GtkPlus_FOUND)
  ADD_DEFINITIONS(${GTKPLUS_C_FLAGS})
  LINK_LIBRARIES(${GTKPLUS_LINK_FLAGS})
  INCLUDE_DIRECTORIES(${GTKPLUS_INCLUDE_DIR})

  #---------------------------------------

  #set up gthread
  #ADD_DEFINITIONS(${GTHREAD_C_FLAGS})
  #LINK_LIBRARIES(${GTHREAD_LINK_FLAGS})
  #INCLUDE_DIRECTORIES(${GTHREAD_INCLUDE_DIR})


  #search for includes
  INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/include        
    )

  IF(USE_KASPAR_MOD)
    ADD_DEFINITIONS(-DUSE_KASPAR_MOD)
    FIND_PACKAGE(kasparmod)
    IMPORT_DEVICES("${KASPAR_DIR}/src/FirstIITSensorTest/include/iCub/drivers.h" kasparmod)
  ENDIF (USE_KASPAR_MOD)

  # Search for source code.
  FILE(GLOB folder_source src/*.cpp src/*.cc src/*.c)
  FILE(GLOB folder_header include/iCub/*.h)
  SOURCE_GROUP("Source Files" FILES ${folder_source})
  SOURCE_GROUP("Header Files" FILES ${folder_header})


  LINK_DIRECTORIES(
    ${KASPAR_SOURCE_DIR}
    )

  LINK_LIBRARIES(
    ${GTHREAD_LIBRARIES}	 	
    )
  LINK_LIBRARIES(${YARP_LIBRARIES})

  # Set up our main executable.
  IF (folder_source)
    ADD_EXECUTABLE(${KEYWORD} 
      ${folder_source} 
      ${folder_header}
      )	
    TARGET_LINK_LIBRARIES(${KEYWORD} ${GSL_LIBRARIES})
  ELSE (folder_source)
    MESSAGE(FATAL_ERROR "No source code files found. Please add something")
  ENDIF (folder_source)

  INSTALL_TARGETS(/bin ${KEYWORD})

ELSE (GtkPlus_Found)
  MESSAGE(STATUS "Gtk not found, Skipping iCubSkinGui")
ENDIF (GtkPlus_FOUND)
