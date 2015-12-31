#
# Try to find pcbusb library and include path.
# Once done this will define
#
# PCBUSB_FOUND
# PCBUSB_INCLUDE_DIR
# PCBUSB_LIBRARY
# 

FIND_PATH( PCBUSB_INCLUDE_DIR PCBUSB.h
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where PCBUSB.h resides")
FIND_LIBRARY( PCBUSB_LIBRARY
        NAMES PCBUSB pcbusb
        PATHS
        /usr/lib64
        /usr/lib
        /usr/local/lib64
        /usr/local/lib
        /sw/lib
        /opt/local/lib
        DOC "The PCBUSB library")
IF (PCBUSB_INCLUDE_DIR AND PCBUSB_LIBRARY)
        SET( PCBUSB_FOUND TRUE )
ENDIF (PCBUSB_INCLUDE_DIR AND PCBUSB_LIBRARY)

IF (PCBUSB_FOUND)
   IF (NOT PCBUSB_FIND_QUIETLY)
      MESSAGE(STATUS "Found PCBUSB: ${PCBUSB_LIBRARY}")
   ENDIF (NOT PCBUSB_FIND_QUIETLY)
ELSE (PCBUSB_FOUND)
   IF (PCBUSB_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find PCBUSB")
   ENDIF (PCBUSB_FIND_REQUIRED)
ENDIF (PCBUSB_FOUND)
