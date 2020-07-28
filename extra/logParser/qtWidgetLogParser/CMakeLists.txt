include ("${CMAKE_SOURCE_DIR}/parserSrcs.cmake")
include ("${CMAKE_SOURCE_DIR}/generatorSrcs.cmake")

set(PROJECT_NAME QtWidgetLogParser)
project(${PROJECT_NAME})

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ui)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)
find_library(PTTK_LIB PTTK)

set(SRCS_FILES
    mainwindow.h
    mainwindow.cpp
    main.cpp
    logviewerwidget.h
    logviewerwidget.cpp
    loggeneratordialog.h
    loggeneratordialog.cpp
    progressdialog.h
    progressdialog.cpp
    prioritycombobox.h
    prioritycombobox.cpp
    qtparserinifile.h
    qtparserinifile.cpp
    explorerpathwidget.h
    explorerpathwidget.cpp
)

set(RSC_FILES
    resources.qrc
)

set(UI_FILES
    ui/MainWindow.ui
    ui/LogGeneratorDialog.ui
    ui/ProgressDialog.ui
)

add_executable(${PROJECT_NAME} ${SRCS_FILES} ${RSC_FILES} ${UI_FILES} ${ParserSrc} ${GeneratorSrcs})
target_link_libraries(${PROJECT_NAME} Qt::Widgets Qt::Core PTTK)
target_include_directories(${PROJECT_NAME} PUBLIC ${GeneratorIncludeDir} ${ParserIncludeDir})
