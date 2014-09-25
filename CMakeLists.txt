################################################################
# Project Swift2D
################################################################

PROJECT(Swift2D CXX)

# version number
set(SWIFT2D_MAJOR 0)
set(SWIFT2D_MINOR 1)
set(SWIFT2D_PATCH 0)
set(SWIFT2D_VERSION ${SWIFT2D_MAJOR}.${SWIFT2D_MINOR}.${SWIFT2D_PATCH})
set(SWIFT2D_DESCRIPTION "Swift2D - an astonishing 2D game engine")
set(SWIFT2D_HOMEPAGE "http://www.swift2d.org")
set(SWIFT2D_EXENAME "Swift2D")
set(SWIFT2D_PACKAGENAME "Swift2D")

cmake_minimum_required(VERSION 2.8.0)

# find_package(PkgConfig)

SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

################################################################
# Configure and find libraries
################################################################

find_package(GLFW3 REQUIRED)
find_package(OPENGL REQUIRED)
find_package(BOX2D REQUIRED)
find_package(OPENAL REQUIRED)
find_package(SNDFILE REQUIRED)
find_package(PNG REQUIRED)
find_package(CURL REQUIRED)
find_package(ZLIB REQUIRED)

# set(BOOST_ROOT "/opt/boost/boost_1_55_0")
set(BOOST_ROOT "D:\\boost_1_56_0")
set(BOOST_LIBRARYDIR "D:\\boost_1_56_0\\lib32-msvc-12.0")
set(Boost_USE_STATIC_LIBS ON)
# set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
# set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS thread system filesystem)

# pkg_check_modules(CURL    REQUIRED libcurl)
# pkg_check_modules(PNG     REQUIRED libpng)
# pkg_check_modules(SNDFILE REQUIRED sndfile)
# pkg_check_modules(GLEW    REQUIRED glew)
# pkg_check_modules(OPENAL  REQUIRED openal)
# pkg_check_modules(ALUT    REQUIRED freealut)
# pkg_check_modules(OPENGL  REQUIRED gl)
# pkg_check_modules(BOX2D   REQUIRED box2d)

cmake_policy(SET CMP0015 OLD)

set(LIBS
  ${GLFW_LIBRARIES}
  ${Boost_LIBRARIES}
  ${PNG_LIBRARIES}
  ${OPENAL_LIBRARIES}
  ${SNDFILE_LIBRARIES}
  ${ALUT_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${BOX2D_LIBRARIES}
  ${CURL_LIBRARIES}
  ${ZLIB_LIBRARIES}
  awesomium
  steam_api
  wldap32
  # miniupnpc
  # awesomium-1-7
  # Xxf86vm
)
link_libraries(${LIBS})

set(LIB_PATHS
  ${Boost_LIBRARY_DIRS}
  "$(AWE_DIR)build/lib"
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/steam/redistributable_bin/linux64
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party/steam/redistributable_bin
)
link_directories(${LIB_PATHS})

set(OWN_INCLUDE_PATHS
  ${CMAKE_CURRENT_SOURCE_DIR}/build
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/third_party
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

# IF(CMAKE_BUILD_TYPE MATCHES "release")
#   add_definitions(-DOGLPLUS_LOW_PROFILE)
#   set(CMAKE_CXX_FLAGS "-s -Ofast --std=c++0x")
# ELSE(CMAKE_BUILD_TYPE MATCHES "release")
#   set(CMAKE_CXX_FLAGS "-g -Wall --std=c++0x")
# ENDIF(CMAKE_BUILD_TYPE MATCHES "release")

set(CMAKE_CXX_FLAGS_RELEASE "/Ox /MP /MD /w /EHsc")
set(CMAKE_CXX_FLAGS_DEBUG "/Ox /MP /MD /w /EHsc")
add_definitions(-D_WIN32_WINDOWS)
add_definitions(-DGLEW_STATIC)
add_definitions(-DCURL_STATICLIB)

################################################################
# Create libraries
################################################################

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_subdirectory(third_party/glew)
add_subdirectory(third_party/raknet)
add_subdirectory(src)

################################################################
# Create examples
################################################################

add_subdirectory(examples/network)
