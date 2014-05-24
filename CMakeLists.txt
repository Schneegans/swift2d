################################################################
# Project Swift2D
################################################################

PROJECT(Swift2D CXX)

# version number
set(SWIFT2D_MAJOR 0)
set(SWIFT2D_MINOR 1)
set(SWIFT2D_PATCH 0)
set(SWIFT2D_VERSION ${SWIFT2D_MAJOR}.${SWIFT2D_MINOR}.${SWIFT2D_PATCH})
set(SWIFT2D_DESCRIPTION "Swift2D - an astonishing virtual reality engine")
set(SWIFT2D_HOMEPAGE "http://www.swift2d.org")
set(SWIFT2D_EXENAME "Swift2D")
set(SWIFT2D_PACKAGENAME "Swift2D")

# We require at least version 2.8.0
cmake_minimum_required(VERSION 2.8.0)

find_package(PkgConfig)

# Location where cmake first looks for modules.
SET(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

################################################################
# Configure and find libraries
################################################################

pkg_check_modules(GLFW REQUIRED glfw3)
pkg_check_modules(GLEW REQUIRED glew)
pkg_check_modules(PNG REQUIRED libpng)
pkg_check_modules(OPENAL REQUIRED openal)
pkg_check_modules(SNDFILE REQUIRED sndfile)
pkg_check_modules(ALUT REQUIRED freealut)
pkg_check_modules(PANGOCAIRO REQUIRED pangocairo)
pkg_check_modules(OPENGL REQUIRED gl)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS thread system)

include(find_ev)

set(LIBS
    ${EV_LIBRARIES}
    ${GLFW_LIBRARIES}
    ${Boost_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${PNG_LIBRARIES}
    ${OPENAL_LIBRARIES}
    ${SNDFILE_LIBRARIES}
    ${ALUT_LIBRARIES}
    ${PANGOCAIRO_LIBRARIES}
    ${OPENGL_LIBRARIES}
)
link_libraries(${LIBS})

set(LIB_PATHS
    ${GLFW_LIBRARY_DIRS}
    ${Boost_LIBRARY_DIRS}
    ${GLEW_LIBRARY_DIRS}
    ${PNG_LIBRARY_DIRS}
    ${OPENAL_LIBRARY_DIRS}
    ${SNDFILE_LIBRARY_DIRS}
    ${ALUT_LIBRARY_DIRS}
    ${PANGOCAIRO_LIBRARY_DIRS}
    ${OPENGL_LIBRARY_DIRS}
)
link_directories(${LIB_PATHS})

set(OWN_INCLUDE_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/build
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/include
)
include_directories(${OWN_INCLUDE_PATHS})

set(INCLUDE_PATHS
    ${EV_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${PNG_INCLUDE_DIRS}
    ${OPENAL_INCLUDE_DIRS}
    ${SNDFILE_INCLUDE_DIRS}
    ${ALUT_INCLUDE_DIRS}
    ${PANGOCAIRO_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)
include_directories(SYSTEM ${INCLUDE_PATHS})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-s -O4 --std=c++0x -fPIC")
set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall --std=c++0x -fPIC")

################################################################
# Create libraries
################################################################

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_subdirectory(src)

################################################################
# Create examples
################################################################

file(GLOB _EXAMPLES ${CMAKE_CURRENT_SOURCE_DIR}/examples/*)
FOREACH(_EXAMPLE ${_EXAMPLES})
    add_subdirectory(${_EXAMPLE})
ENDFOREACH(_EXAMPLE ${_EXAMPLES})

################################################################
# Summary
################################################################

message( "" )
message( "Summary:" )
message( " build type: ${CMAKE_BUILD_TYPE}" )
message( "" )
message( " boost:" )
message( "   library: ${Boost_LIBRARIES}" )
message( "   include: ${Boost_INCLUDE_DIRS}" )
message( "" )
message( " ev:" )
message( "   library: ${EV_LIBRARIES}" )
message( "   include: ${EV_INCLUDE_DIRS}" )
message( "" )
message( " glfw3:" )
message( "   library: ${GLFW_LIBRARIES}" )
message( "   include: ${GLFW_INCLUDE_DIRS}" )
message( "" )
message( " glew:" )
message( "   library: ${GLEW_LIBRARIES}" )
message( "   include: ${GLEW_INCLUDE_DIRS}" )
message( "" )
message( " png:" )
message( "   library: ${PNG_LIBRARIES}" )
message( "   include: ${PNG_INCLUDE_DIRS}" )
message( "" )
message( " openal:" )
message( "   library: ${OPENAL_LIBRARIES}" )
message( "   include: ${OPENAL_INCLUDE_DIRS}" )
message( "" )
message( " sndfile:" )
message( "   library: ${SNDFILE_LIBRARIES}" )
message( "   include: ${SNDFILE_INCLUDE_DIRS}" )
message( "" )
message( " alut:" )
message( "   library: ${ALUT_LIBRARIES}" )
message( "   include: ${ALUT_INCLUDE_DIRS}" )
message( "" )
message( " pangocairo:" )
message( "   library: ${PANGOCAIRO_LIBRARIES}" )
message( "   include: ${PANGOCAIRO_INCLUDE_DIRS}" )
message( "" )
message( " opengl:" )
message( "   library: ${OPENGL_LIBRARIES}" )
message( "   include: ${OPENGL_INCLUDE_DIRS}" )
message( "" )
