#Search for the include file...
FIND_PATH(ZLIB_INCLUDE_DIRS zlib.h DOC "Path to ZLIB include directory."
  PATHS
  /usr/include/
  /usr/include/zlib
  ${CMAKE_MODULE_PATH}/../../third_party/zlib
)

#Search for the library file...
SET(ZLIB_LIB_NAMES zlib zlib.lib)

FIND_LIBRARY(ZLIB_LIBRARIES DOC "Absolute path to ZLIB library."
  NAMES ${ZLIB_LIB_NAMES}
  PATHS
  /usr/local/lib
  /usr/lib
  ${CMAKE_MODULE_PATH}/../../third_party/zlib
)

#did we find it?
IF(ZLIB_LIBRARIES AND ZLIB_INCLUDE_DIRS)
  SET(ZLIB_FOUND TRUE)
  message(STATUS "Found ZLIB: ${ZLIB_LIBRARIES}")
ELSE()
  message(STATUS "ZLIB NOT found!")
ENDIF(ZLIB_LIBRARIES AND ZLIB_INCLUDE_DIRS)
