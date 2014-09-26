################################################################
# Project Swift2D
################################################################

PROJECT(Swift2D CXX)

# version number
set(SWIFT2D_MAJOR 0)
set(SWIFT2D_MINOR 1)
set(SWIFT2D_PATCH 0)
set(SWIFT2D_VERSION ${SWIFT2D_MAJOR}.${SWIFT2D_MINOR}.${SWIFT2D_PATCH})

cmake_minimum_required(VERSION 2.8.0)

################################################################
# Configure and find libraries
################################################################

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
CMAKE_POLICY(SET CMP0015 OLD)

# set(BOOST_ROOT "/opt/boost/boost_1_55_0")
Set(BOOST_ROOT "D:\\boost_1_56_0")
Set(BOOST_LIBRARYDIR "D:\\boost_1_56_0\\lib32-msvc-12.0")
Set(Boost_USE_STATIC_LIBS ON)
Set(Boost_USE_MULTITHREADED ON)
Set(Boost_USE_STATIC_RUNTIME OFF)
FIND_PACKAGE(Boost   REQUIRED COMPONENTS thread system filesystem)
FIND_PACKAGE(GLFW3   REQUIRED)
FIND_PACKAGE(OPENGL  REQUIRED)
FIND_PACKAGE(BOX2D   REQUIRED)
FIND_PACKAGE(OPENAL  REQUIRED)
FIND_PACKAGE(SNDFILE REQUIRED)
FIND_PACKAGE(PNG     REQUIRED)
FIND_PACKAGE(CURL    REQUIRED)
FIND_PACKAGE(ZLIB    REQUIRED)

set(LIBS
  wldap32
  ${Boost_LIBRARIES}
  ${OPENGL_LIBRARIES}  
  ${GLFW_LIBRARIES} 
  ${PNG_LIBRARIES}
  ${OPENAL_LIBRARIES}
  ${SNDFILE_LIBRARIES}
  ${ALUT_LIBRARIES}
  ${BOX2D_LIBRARIES}
  ${ZLIB_LIBRARIES}
  ${CURL_LIBRARIES}
  awesomium
  steam_api
  # miniupnpc
  # awesomium-1-7
  # Xxf86vm
)
link_libraries(${LIBS})

set(LIB_PATHS
  ${Boost_LIBRARY_DIRS}
  "$(AWE_DIR)build/lib"
  #${CMAKE_CURRENT_SOURCE_DIR}/third_party/steam/redistributable_bin/linux64
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/steam/redistributable_bin
)
link_directories(${LIB_PATHS})

set(OWN_INCLUDE_PATHS
  #${CMAKE_CURRENT_SOURCE_DIR}/build
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  #${CMAKE_CURRENT_SOURCE_DIR}/third_party
)
include_directories(${OWN_INCLUDE_PATHS})

set(INCLUDE_PATHS
  ${GLFW_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
  ${PNG_INCLUDE_DIRS}
  ${OPENAL_INCLUDE_DIRS}
  ${SNDFILE_INCLUDE_DIRS}
  ${ALUT_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIRS}
  ${BOX2D_INCLUDE_DIRS}
  ${CURL_INCLUDE_DIRS}
  ${ZLIB_INCLUDE_DIRS}
  "$(AWE_DIR)include"
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glew
)
include_directories(SYSTEM ${INCLUDE_PATHS})

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "release")
endif()

IF(CMAKE_COMPILER_IS_GNUCXX)
  IF(CMAKE_BUILD_TYPE MATCHES "release")
    add_definitions(-DOGLPLUS_LOW_PROFILE)
    set(CMAKE_CXX_FLAGS "-s -Ofast --std=c++0x")
  ELSE(CMAKE_BUILD_TYPE MATCHES "release")
    set(CMAKE_CXX_FLAGS "-g -Wall --std=c++0x")
  ENDIF(CMAKE_BUILD_TYPE MATCHES "release")
ELSE(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "/Ox /MP /MD /EHsc")
  add_definitions(-D_WIN32_WINDOWS)
  add_definitions(-DGLEW_STATIC)
  add_definitions(-DWIN32_LEAN_AND_MEAN)
  add_definitions(-DBOOST_ASIO_SEPARATE_COMPILATION)
  add_definitions(-DSWIFT_MAKE_LIBRARY)
  add_definitions(-DCURL_STATICLIB)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

################################################################
# Create libraries
################################################################

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(third_party/glew)
add_subdirectory(third_party/raknet)
add_subdirectory(src)
