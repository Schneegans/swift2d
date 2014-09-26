#Search for the include file...
FIND_PATH(PNG_INCLUDE_DIRS png.h DOC "Path to PNG include directory."
  PATHS
  /usr/include/
  /usr/include/png
  ${CMAKE_MODULE_PATH}/../../third_party/png
)

#Search for the library file...
SET(PNG_LIB_NAMES png libpng.lib)

FIND_LIBRARY(PNG_LIBRARIES DOC "Absolute path to PNG library."
  NAMES ${PNG_LIB_NAMES}
  PATHS
  /usr/local/lib
  /usr/lib
  ${CMAKE_MODULE_PATH}/../../third_party/png
)

#did we find it?
IF(PNG_LIBRARIES AND PNG_INCLUDE_DIRS)
  SET(PNG_FOUND TRUE)
  message(STATUS "Found PNG: ${PNG_LIBRARIES}")
ELSE()
  message(STATUS "PNG NOT found!")
ENDIF(PNG_LIBRARIES AND PNG_INCLUDE_DIRS)
