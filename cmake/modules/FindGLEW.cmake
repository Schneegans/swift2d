#Search for the include file...
FIND_PATH(GLEW_INCLUDE_DIRS GL/glew.h DOC "Path to GLEW include directory."
  PATHS
  /usr/include/
  /usr/include/GLEW
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew
)

#Search for the library file...
SET(GLEW_LIB_NAMES GLEW glew32mx.lib glew32mxs.lib)

FIND_LIBRARY(GLEW_LIBRARIES DOC "Absolute path to GLEW library."
  NAMES ${GLEW_LIB_NAMES}
  PATHS
  /usr/local/lib
  /usr/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew
)

#did we find it?
IF(GLEW_LIBRARIES AND GLEW_INCLUDE_DIRS)
  SET(GLEW_FOUND TRUE)
  message(STATUS "Found GLEW: ${GLEW_LIBRARIES}")
ELSE()
  message(STATUS "GLEW NOT found!")
ENDIF(GLEW_LIBRARIES AND GLEW_INCLUDE_DIRS)
