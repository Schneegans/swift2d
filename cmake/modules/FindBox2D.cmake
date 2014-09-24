#Search for the include file...
FIND_PATH(BOX2D_INCLUDE_DIRS Box2D/Box2D.h DOC "Path to BOX2D include directory."
  PATHS
  /usr/include/
  /usr/include/box2d
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/box2d
)

#Search for the library file...
SET(BOX2D_LIB_NAMES box2d Box2D.lib)

FIND_LIBRARY(BOX2D_LIBRARIES DOC "Absolute path to BOX2D library."
  NAMES ${BOX2D_LIB_NAMES}
  PATHS
  /usr/local/lib
  /usr/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/box2d
)

#did we find it?
IF(BOX2D_LIBRARIES AND BOX2D_INCLUDE_DIRS)
  SET(BOX2D_FOUND TRUE)
  message(STATUS "Found BOX2D: ${BOX2D_LIBRARIES}")
ELSE()
  message(STATUS "BOX2D NOT found!")
ENDIF(BOX2D_LIBRARIES AND BOX2D_INCLUDE_DIRS)
