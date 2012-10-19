# Try to find qextserial library
# Once done this will define
#
#  QEXTSERIAL_FOUND - system has qextserial
#  QEXTSERIAL_INCLUDE_DIR - ~ the include directory 
#  QEXTSERIAL_LIBRARY - Link these to use qextserial
#
#  2010, Ze Ji
#
	
#SET(QEXTSERIAL_DIR "D:/yarp_libraries/qextserialport/src")
SET(QEXTSERIAL_DIR $ENV{QEXTSERIAL_DIR})
IF(WIN32)
	# give priority to QEXTSERIAL_DIR which should point to 
	# the correct place, but also look in other locations.
	FIND_LIBRARY(QEXTSERIAL_LIB NAMES qextserialport1 qextserialport1d PATHS ${QEXTSERIAL_DIR}/build
												${QEXTSERIAL_DIR}/src/build
						 						   NO_DEFAULT_PATH)
	FIND_PATH(QEXTSERIAL_INC_DIRS NAMES qextserialport.h PATHS ${QEXTSERIAL_DIR}
						   ${QEXTSERIAL_DIR}/src)
ELSE(WIN32)  
    FIND_PATH(QEXTSERIAL_INC_DIRS NAMES qextserialport.h PATHS ${QEXTSERIAL_DIR}
						   ${QEXTSERIAL_DIR}/src)
    FIND_LIBRARY(QEXTSERIAL_LIB NAMES qextserialport PATHS ${QEXTSERIAL_DIR}/build ${QEXTSERIAL_DIR}/src/build)    
    SET(QEXTSERIAL_INC_DIRS ${QEXTSERIAL_DIR}/src)
ENDIF(WIN32)
    
IF(QEXTSERIAL_LIB)
    SET(QEXTSERIAL_FOUND TRUE)
ELSE(QEXTSERIAL_LIB)
    SET(QEXTSERIAL_FOUND FALSE)
    SET(QEXTSERIAL_INC_DIRS)
    SET(QEXTSERIAL_LIB)
ENDIF(QEXTSERIAL_LIB)
