SET(EV_INCLUDE_SEARCH_DIRS
    /usr/include
)

SET(EV_LIBRARY_SEARCH_DIRS
    /usr/lib
    /usr/lib/x86_64-linux-gnu
)

message("-- checking for ev")

IF (NOT EV_INCLUDE_DIRS)

    SET(_EV_FOUND_INC_DIRS "")

    FOREACH(_SEARCH_DIR ${EV_INCLUDE_SEARCH_DIRS})
        FIND_PATH(_CUR_SEARCH
                NAMES ev.h
                PATHS ${_SEARCH_DIR}
                NO_DEFAULT_PATH)
        IF (_CUR_SEARCH)
            LIST(APPEND _EV_FOUND_INC_DIRS ${_CUR_SEARCH})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${EV_INCLUDE_SEARCH_DIRS})

    IF (NOT _EV_FOUND_INC_DIRS)
        MESSAGE(FATAL_ERROR "find_ev.cmake: unable to find ev headers")
    ENDIF (NOT _EV_FOUND_INC_DIRS)

    FOREACH(_INC_DIR ${_EV_FOUND_INC_DIRS})
        LIST(APPEND EV_INCLUDE_DIRS ${_INC_DIR})
    ENDFOREACH(_INC_DIR ${_BOOST_FOUND_INC_DIRS})

ENDIF(NOT EV_INCLUDE_DIRS)

IF (        EV_INCLUDE_DIRS
    AND NOT EV_LIBRARIES)

    SET(_EV_FOUND_LIB_DIR "")
    SET(_EV_POSTFIX "")

    FOREACH(_SEARCH_DIR ${EV_LIBRARY_SEARCH_DIRS})
        FIND_PATH(_CUR_SEARCH
                NAMES libev.so
                PATHS ${_SEARCH_DIR}
                NO_DEFAULT_PATH)
        IF (_CUR_SEARCH)
            LIST(APPEND _EV_FOUND_LIB_DIR ${_SEARCH_DIR})
        ENDIF(_CUR_SEARCH)
        SET(_CUR_SEARCH _CUR_SEARCH-NOTFOUND CACHE INTERNAL "internal use")
    ENDFOREACH(_SEARCH_DIR ${EV_LIBRARY_SEARCH_DIRS})

    IF (NOT _EV_FOUND_LIB_DIR)
        MESSAGE(FATAL_ERROR "find_ev.cmake: unable to find ev library")
    ELSE (NOT _EV_FOUND_LIB_DIR)
        message("--  found matching version")
    ENDIF (NOT _EV_FOUND_LIB_DIR)

    FOREACH(_LIB_DIR ${_EV_FOUND_LIB_DIR})
        LIST(APPEND EV_LIBRARIES ${_LIB_DIR}/libev.so)
    ENDFOREACH(_LIB_DIR ${_EV_FOUND_INC_DIRS})


ENDIF(        EV_INCLUDE_DIRS
      AND NOT EV_LIBRARIES)



