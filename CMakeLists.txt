set(LIB_NAME PTTK)

set(PTTK_SRC 
    src/log.cpp
    src/logger.cpp
    src/stringtools.cpp
    include/executiontimer.h
    include/log.h
    include/logger.h
    include/macro.h
    include/stringtools.h)

set(PTTK_INCLUDE_DIR include/)

add_library(${LIB_NAME} PTTK_SRC)

add_subdirectory(extra)