#Search for the include file...
FIND_PATH(GLFW_INCLUDE_DIRS GLFW/glfw3.h DOC "Path to GLFW include directory."
  PATHS
  /usr/include/
  /usr/include/GLFW
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party
)

#Search for the library file...
SET(GLFW_LIB_NAMES glfw3 GLFW3.lib)

FIND_LIBRARY(GLFW_LIBRARIES DOC "Absolute path to GLFW library."
  NAMES ${GLFW_LIB_NAMES}
  PATHS
  /usr/local/lib
  /usr/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/GLFW
)

#did we find it?
IF(GLFW_LIBRARIES AND GLFW_INCLUDE_DIRS)
  SET(GLFW_FOUND TRUE)
  message(STATUS "Found GLFW3: ${GLFW_LIBRARIES}")
ELSE()
  message(STATUS "GLFW3 NOT found!")
ENDIF(GLFW_LIBRARIES AND GLFW_INCLUDE_DIRS)
