# - Try to find the freetype library
# Once done this defines
#
#  LIBUSB_FOUND - system has libusb
#  LIBUSB_INCLUDE_DIR - the libusb include directory
#  LIBUSB_LIBRARIES - Link these to use libusb

# Copyright (c) 2006, 2008  Laurent Montel, <montel@kde.org>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

#SET(LIBUSB_INCLUDE_DIR $ENV{PC_LIBUSB_INCLUDEDIR})
#SET(LIBUSB_LIBRARIES $ENV{PC_LIBUSB_LIBRARY_DIRS}/libusb)

if (LIBUSB_INCLUDE_DIR AND LIBUSB_LIBRARIES)

  # in cache already
  set(LIBUSB_FOUND TRUE)

else (LIBUSB_INCLUDE_DIR AND LIBUSB_LIBRARIES)
  IF (NOT WIN32)
    # use pkg-config to get the directories and then use these values
    # in the FIND_PATH() and FIND_LIBRARY() calls
    find_package(PkgConfig)
    pkg_check_modules(PC_LIBUSB libusb)
  ENDIF(NOT WIN32)

  FIND_PATH(LIBUSB_INCLUDE_DIR usb.h
    PATHS $ENV{PC_LIBUSB_INCLUDEDIR} $ENV{PC_LIBUSB_INCLUDE_DIRS})

  FIND_LIBRARY(LIBUSB_LIBRARIES NAMES libusb 
    PATHS $ENV{PC_LIBUSB_LIBDIR} $ENV{PC_LIBUSB_LIBRARY_DIRS})

  include(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(LIBUSB DEFAULT_MSG LIBUSB_LIBRARIES LIBUSB_INCLUDE_DIR)

  MARK_AS_ADVANCED(LIBUSB_INCLUDE_DIR LIBUSB_LIBRARIES)

endif (LIBUSB_INCLUDE_DIR AND LIBUSB_LIBRARIES)
