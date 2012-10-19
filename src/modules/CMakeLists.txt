PROJECT(kasparModule)

FIND_PACKAGE(YARP REQUIRED)

# Prepare a device library called "kasparmod".
# A test executable called "kasparmoddev" will also be produced.
#include(YarpPlugin)

include(${YARP_MODULE_PATH}/YarpDevice.cmake)

#include(${YARP_MODULE_PATH}/YarpPlugin.cmake)

FIND_PACKAGE(FTDI QUIET)
IF (NOT FTDI_FOUND)
  MESSAGE(STATUS "Skipping modules using Ftdi")
ENDIF (NOT FTDI_FOUND)

BEGIN_PLUGIN_LIBRARY(kasparmod)
	IF(NOT FTDI_FOUND)
		#MESSAGE(WARNING "FTDI libary not found, skipping DynamixelAX12FtdiDriver")	
		MESSAGE(STATUS "FTDI libary not found, skipping DynamixelAX12FtdiDriver")
	ELSE(NOT FTDI_FOUND)
		ADD_SUBDIRECTORY(DynamixelAX12FtdiDriver)
	ENDIF(NOT FTDI_FOUND)
  	
	ADD_SUBDIRECTORY(ax12motor)
	ADD_SUBDIRECTORY(fsrTouchSensor)
	#ADD_SUBDIRECTORY(esdCan)
	#ADD_SUBDIRECTORY(fakeCan)
	#ADD_SUBDIRECTORY(skinprototype)
	ADD_SUBDIRECTORY(dynamixelAx12)
	ADD_SUBDIRECTORY(qFsrSensor)
	#ADD_SUBDIRECTORY(UHskinprototype)
	ADD_SUBDIRECTORY(kasparServos)

END_PLUGIN_LIBRARY(kasparmod)

ADD_PLUGIN_LIBRARY_EXECUTABLE(kasparmoddev kasparmod)

IF(MSVC)
    SET_TARGET_PROPERTIES(kasparmoddev PROPERTIES COMPILE_FLAGS /wd4996)
    SET_TARGET_PROPERTIES(kasparmod PROPERTIES COMPILE_FLAGS /wd4996)
ENDIF(MSVC)

#ADD_SUBDIRECTORY(motionControlLib)
IF (USE_KASPAR_MOD)
  INSTALL(TARGETS kasparmoddev DESTINATION bin)
  INSTALL(TARGETS kasparmod DESTINATION lib)
ENDIF (USE_KASPAR_MOD)
