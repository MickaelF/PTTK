include ("${CMAKE_SOURCE_DIR}/parserSrcs.cmake")

set(PROJECT_NAME CmdLogParser)

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_library(PTTK_LIB PTTK)

set(SRCS main.cpp
	parserarguments.cpp
	parserarguments.h
	parserexecution.h 
	parserexecution.cpp
)

add_executable(${PROJECT_NAME} ${SRCS} ${ParserSrc})
target_link_libraries(${PROJECT_NAME} PTTK)
target_include_directories(${PROJECT_NAME} PUBLIC ${ParserIncludeDir})
