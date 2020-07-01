cmake_minimum_required(VERSION 3.17)

set(LIB_NAME PTTK)
project(${LIB_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PTTK_SRC 
    src/log.cpp
    src/logger.cpp
    src/stringtools.cpp
    include/executiontimer.h
    include/log.h
    include/logger.h
    include/macro.h
    include/stringtools.h)

set(PTTK_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/ PARENT_SCOPE)

add_library(${LIB_NAME} ${PTTK_SRC})
target_include_directories(${LIB_NAME} PUBLIC ./include/)