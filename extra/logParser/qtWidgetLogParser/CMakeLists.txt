set(PROJECT_NAME QtWidgetLogParser)
project(${PROJECT_NAME})

set(GeneratorIncludeDir ${CMAKE_SOURCE_DIR}/extra/logGenerator/)
set(ParserIncludeDir ${CMAKE_SOURCE_DIR}/extra/logParser/)
set(GeneratorSrc
    ${CMAKE_SOURCE_DIR}/extra/logGenerator/loggenerator.cpp)
set(ParserSrc
    ${CMAKE_SOURCE_DIR}/extra/logParser/loglineinfo.cpp
    ${CMAKE_SOURCE_DIR}/extra/logParser/logparser.cpp)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Core LinguistTools REQUIRED)
find_library(PTTK_LIB PTTK)

set(SRCS_FILES
    mainwindow.h
    mainwindow.cpp
    main.cpp
    logviewermodel.h
    logviewermodel.cpp
    loggeneratordialog.h
    loggeneratordialog.cpp
    progressdialog.h
    progressdialog.cpp
    qtparserinifile.h
    qtparserinifile.cpp
    explorerpathwidget.h
    explorerpathwidget.cpp
	startupdialog.h
	startupdialog.cpp
	logviewerwidget.h
	logviewerwidget.cpp
	logstyledelegate.h
	logstyledelegate.cpp
	prioritylabelfactory.cpp
	prioritylabelfactory.h
    logviewerhorizontalheader.cpp
    logviewerhorizontalheader.h
    logfilterproxymodel.h
    logfilterproxymodel.cpp
    style.h
    filenamesmenu.h
    filenamesmenu.cpp
	aboutdialog.cpp
	aboutdialog.h
)

set(RSC_FILES
    resources.qrc
)

set(UI_FILES
    ui/MainWindow.ui
    ui/LogGeneratorDialog.ui
    ui/ProgressDialog.ui
	ui/StartUpDialog.ui
)
qt5_add_translation(QM_FILES resources/french.ts resources/english.ts)
add_executable(${PROJECT_NAME} ${SRCS_FILES} ${RSC_FILES} ${UI_FILES} ${ParserSrc} ${GeneratorSrc} ${QM_FILES})
target_link_libraries(${PROJECT_NAME} Qt::Widgets Qt::Core PTTK)
target_include_directories(${PROJECT_NAME} PUBLIC ${PTTK_INCLUDE_DIRS} ${GeneratorIncludeDir} ${ParserIncludeDir})
