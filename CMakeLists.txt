cmake_minimum_required(VERSION 3.17)

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_LIST_DIR};")

set(LIB_NAME PTTK)
project(${LIB_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PTTK_SRC
    src/logger.cpp
    src/stringtools.cpp
    include/pttk/executiontimer.h
    include/pttk/log.h
    include/pttk/logger.h
    include/pttk/macro.h
    include/pttk/stringtools.h
    src/numbergenerator.cpp
    include/pttk/iprogress.h
	include/pttk/file/abstractinidefinition.h
	include/pttk/file/inifile.h
	src/file/abstractreader.cpp
	src/file/plaintextfile.cpp
	src/pttkpath.cpp
	include/pttk/contiguousarray.h
	include/pttk/logpriority.h
)

option(PTTK_BUILD_CMD_LOG_PARSER "Build the PTTK Log parser command line executable" ON)
option(PTTK_BUILD_LOG_GENERATOR "Build the PTTK Log generator (for parser tests)" OFF)
option(PTTK_BUILD_QT_WIDGET_LOG_PARSER "Build the PTTK Log parser with Qt Widget GUI executable" OFF)

set (PTTK_BUILD_CMD_LOG_PARSER OFF)
set (PTTK_BUILD_LOG_GENERATOR OFF)
set (PTTK_BUILD_QT_WIDGET_LOG_PARSER OFF)

add_library(${LIB_NAME} ${PTTK_SRC})
target_include_directories(${LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:/include>
    )

install(TARGETS ${LIB_NAME} EXPORT ${LIB_NAME}_target LIBRARY)
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(EXPORT ${LIB_NAME}_target
        FILE PTTK.cmake
        EXPORT_LINK_INTERFACE_LIBRARIES
        DESTINATION lib/cmake)

if(PTTK_BUILD_CMD_LOG_PARSER) 
    add_subdirectory(extra/logParser/CmdLogParser)
endif()

if(PTTK_BUILD_LOG_GENERATOR) 
    add_subdirectory(extra/logGenerator)
endif()

if(PTTK_BUILD_QT_WIDGET_LOG_PARSER) 
    add_subdirectory(extra/logParser/qtWidgetLogParser)
endif()