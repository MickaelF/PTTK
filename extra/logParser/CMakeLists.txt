set(SUB_PROJECT_NAME LogParser)

set(SRCS main.cpp
	logparser.cpp
	logparser.h 
	loglineinfo.h 
	loglineinfo.cpp)

add_executable(${SUB_PROJECT_NAME} ${SRCS})
