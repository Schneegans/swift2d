################################################################
# Project Swift2D
################################################################

project(Swift2D CXX)

# version number
set(SWIFT2D_MAJOR 0)
set(SWIFT2D_MINOR 1)
set(SWIFT2D_PATCH 0)
set(SWIFT2D_VERSION ${SWIFT2D_MAJOR}.${SWIFT2D_MINOR}.${SWIFT2D_PATCH})

cmake_minimum_required(VERSION 2.8.0)
cmake_policy(SET CMP0015 OLD)

################################################################
# Configure and find libraries
################################################################

set(BOOST_ROOT "D:\\boost_1_56_0")
set(BOOST_LIBRARYDIR "D:\\boost_1_56_0\\lib32-msvc-12.0")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS thread system filesystem)

set(SWIFT_INCLUDE_DIRS
  "third_party/include"
  "$(AWE_DIR)include"
  ${CMAKE_CURRENT_SOURCE_DIR}/src/raknet
  ${CMAKE_CURRENT_SOURCE_DIR}/src/glew
)

macro(SwiftLoadSystemLib LIB_NAME)
  find_package(${LIB_NAME} REQUIRED)
  list(APPEND SWIFT_INCLUDE_DIRS ${${LIB_NAME}_INCLUDE_DIRS})
endmacro()

SwiftLoadSystemLib("OPENGL")
SwiftLoadSystemLib("Boost")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(SYSTEM ${SWIFT_INCLUDE_DIRS})

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

IF(CMAKE_COMPILER_IS_GNUCXX)
  IF(CMAKE_BUILD_TYPE MATCHES "Release")
    add_definitions(-DOGLPLUS_LOW_PROFILE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s -Ofast --std=c++0x")
  ELSE(CMAKE_BUILD_TYPE MATCHES "release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall --std=c++0x")
  ENDIF(CMAKE_BUILD_TYPE MATCHES "release")
ELSE(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W1 /Ox /MP /MD /EHsc")
  ADD_DEFINITIONS(-D_WIN32_WINDOWS)
  ADD_DEFINITIONS(-DGLEW_STATIC)
  ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)
  ADD_DEFINITIONS(-DBOOST_ASIO_SEPARATE_COMPILATION)
  ADD_DEFINITIONS(-DSWIFT_MAKE_LIBRARY)
  ADD_DEFINITIONS(-DCURL_STATICLIB)
  ADD_DEFINITIONS(-DNOMINMAX)
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

################################################################
# Create libraries
################################################################

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)

add_subdirectory(src)
