#Search for the include file...
FIND_PATH(OPENAL_INCLUDE_DIRS AL/al.h DOC "Path to OPENAL include directory."
  PATHS
  /usr/include/
  /usr/include/OpenAL
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/OpenAL
)

#Search for the library file...
SET(OPENAL_LIB_NAMES openal OpenAL32.lib)

FIND_LIBRARY(OPENAL_LIBRARIES DOC "Absolute path to OPENAL library."
  NAMES ${OPENAL_LIB_NAMES}
  PATHS
  /usr/local/lib
  /usr/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/OpenAL
)

#did we find it?
IF(OPENAL_LIBRARIES AND OPENAL_INCLUDE_DIRS)
  SET(OPENAL_FOUND TRUE)
  message(STATUS "Found OPENAL: ${OPENAL_LIBRARIES}")
ELSE()
  message(STATUS "OPENAL NOT found!")
ENDIF(OPENAL_LIBRARIES AND OPENAL_INCLUDE_DIRS)
