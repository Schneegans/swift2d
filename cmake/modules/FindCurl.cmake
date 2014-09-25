#Search for the include file...
FIND_PATH(CURL_INCLUDE_DIRS curl/curl.h DOC "Path to CURL include directory."
  PATHS
  /usr/include/
  /usr/include/curl
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/curl
)

#Search for the library file...
SET(CURL_LIB_NAMES curl libcurl.lib)

FIND_LIBRARY(CURL_LIBRARIES DOC "Absolute path to CURL library."
  NAMES ${CURL_LIB_NAMES}
  PATHS
  /usr/local/lib
  /usr/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/curl
)

#did we find it?
IF(CURL_LIBRARIES AND CURL_INCLUDE_DIRS)
  SET(CURL_FOUND TRUE)
  message(STATUS "Found CURL: ${CURL_LIBRARIES}")
ELSE()
  message(STATUS "CURL NOT found!")
ENDIF(CURL_LIBRARIES AND CURL_INCLUDE_DIRS)
