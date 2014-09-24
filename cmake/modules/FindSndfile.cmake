#Search for the include file...
FIND_PATH(SNDFILE_INCLUDE_DIRS sndfile.h DOC "Path to SNDFILE include directory."
  PATHS
  /usr/include/
  /usr/include/sndfile
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/sndfile
)

#Search for the library file...
SET(SNDFILE_LIB_NAMES sndfile SNDFILE32.lib)

FIND_LIBRARY(SNDFILE_LIBRARIES DOC "Absolute path to SNDFILE library."
  NAMES ${SNDFILE_LIB_NAMES}
  PATHS
  /usr/local/lib
  /usr/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/sndfile
)

#did we find it?
IF(SNDFILE_LIBRARIES AND SNDFILE_INCLUDE_DIRS)
  SET(SNDFILE_FOUND TRUE)
  message(STATUS "Found SNDFILE: ${SNDFILE_LIBRARIES}")
ELSE()
  message(STATUS "SNDFILE NOT found!")
ENDIF(SNDFILE_LIBRARIES AND SNDFILE_INCLUDE_DIRS)
