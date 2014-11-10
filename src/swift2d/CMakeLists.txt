file(GLOB_RECURSE SWIFT2D_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    *.c
    *.cpp
    ../../include/swift2d/*.h
    ../../include/swift2d/*.hpp
)

ADD_LIBRARY(swift2d STATIC ${SWIFT2D_SRC}) 

TARGET_LINK_LIBRARIES(swift2d ${LIBS} glew raknet) 

