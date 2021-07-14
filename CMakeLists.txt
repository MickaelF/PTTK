cmake_minimum_required(VERSION 3.17)

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_CURRENT_LIST_DIR};")

set(LIB_NAME PTTK)
project(${LIB_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_DEBUG_POSTFIX "d")

option(PTTK_BUILD_CMD_LOG_PARSER "Build the PTTK Log parser command line executable" ON)
option(PTTK_BUILD_LOG_GENERATOR "Build the PTTK Log generator (for parser tests)" OFF)
option(PTTK_BUILD_QT_WIDGET_LOG_PARSER "Build the PTTK Log parser with Qt Widget GUI executable" OFF)

set (PTTK_BUILD_CMD_LOG_PARSER OFF)
set (PTTK_BUILD_LOG_GENERATOR OFF)
set (PTTK_BUILD_QT_WIDGET_LOG_PARSER OFF)

set(PTTK_MAJOR_VERSION 1)
set(PTTK_MINOR_VERSION 1)
set(PTTK_PATCH_VERSION 3)
set(PTTK_VERSION
  ${PTTK_MAJOR_VERSION}.${PTTK_MINOR_VERSION}.${PTTK_PATCH_VERSION})

set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

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

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

# Create the PTTKConfig.cmake and PTTKConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(PTTKConfig.cmake.in
  "${PROJECT_BINARY_DIR}/PTTKConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${PTTK_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(PTTKConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/PTTKConfig.cmake" @ONLY)
# ... for both
configure_file(PTTKConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/PTTKConfigVersion.cmake" @ONLY)

add_library(${LIB_NAME} ${PTTK_SRC})
target_include_directories(${LIB_NAME} PUBLIC
      $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${INSTALL_INCLUDE_DIR}>
  )

install(TARGETS ${LIB_NAME} EXPORT ${LIB_NAME}Targets LIBRARY)
install(DIRECTORY include/ DESTINATION include FILES_MATCHING PATTERN "*.h")
install(EXPORT ${LIB_NAME}Targets
        FILE PTTKTargets.cmake
        EXPORT_LINK_INTERFACE_LIBRARIES
        DESTINATION lib/cmake)
install(FILES
    "${PROJECT_BINARY_DIR}/PTTKConfigVersion.cmake"
    "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/PTTKConfig.cmake"
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