cmake_minimum_required(VERSION 3.17)

set(LIB_NAME PTTK)
project(${LIB_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PTTK_SRC 
    src/logger.cpp
    src/stringtools.cpp
    include/executiontimer.h
    include/log.h
    include/logger.h
    include/macro.h
    include/stringtools.h 
    include/numbergenerator.h 
    src/numbergenerator.cpp)

set(PTTK_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include/ PARENT_SCOPE)

add_library(${LIB_NAME} ${PTTK_SRC})
target_include_directories(${LIB_NAME} PUBLIC ./include/)
option(PTTK_BUILD_CMD_LOG_PARSER "Build the PTTK Log parser command line executable" ON)
option(PTTK_BUILD_LOG_GENERATOR "Build the PTTK Log generator (for parser tests)" ON)
set (PTTK_BUILD_CMD_LOG_PARSER ON)
set (PTTK_BUILD_LOG_GENERATOR ON)

if(PTTK_BUILD_CMD_LOG_PARSER) 
    add_subdirectory(extra/logParser/CmdLogParser)
endif()

if(PTTK_BUILD_LOG_GENERATOR) 
    add_subdirectory(extra/logGenerator)
endif()