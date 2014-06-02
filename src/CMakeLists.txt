# determine source and header files
file(GLOB_RECURSE SWIFT2D_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    swift2d/*.c
    swift2d/*.cpp
    ../include/swift2d/*.h
    ../include/swift2d/*.hpp
    ../third_party/src/raknet/*.h
    ../third_party/src/raknet/*.cpp
)

ADD_LIBRARY( swift2d SHARED
    ${SWIFT2D_SRC}
)

TARGET_LINK_LIBRARIES( swift2d  ${LIBS} )
