set(PROJECT_NAME LogParser)

project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(PTTK_DIR ${CMAKE_SOURCE_DIR}/include)
message(${PTTK_DIR})

set(SRCS main.cpp
	logparser.cpp
	logparser.h 
	loglineinfo.h 
	loglineinfo.cpp)

add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} PTTK)
target_include_directories(${PROJECT_NAME} PUBLIC ${PTTK_DIR})
