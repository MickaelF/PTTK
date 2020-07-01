set(SUB_PROJECT_NAME LogParser)


set(PTTK_DIR ${CMAKE_SOURCE_DIR}/../../)
add_subdirectory(${PTTK_DIR})

set(SRCS main.cpp
	logparser.cpp
	logparser.h 
	loglineinfo.h 
	loglineinfo.cpp)

add_executable(${SUB_PROJECT_NAME} ${SRCS})
target_link_directories(${SUB_PROJECT_NAME} PTTK)
