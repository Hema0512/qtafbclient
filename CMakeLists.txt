cmake_minimum_required(VERSION 3.5)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})

set(CURRENT_PROJECT qtafbclient)

project(${CURRENT_PROJECT})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebSockets REQUIRED)
find_package(QScintilla REQUIRED)

include_directories(${QScintilla_INCLUDE_DIRS})

set(CURRENT_PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    qafbwebsocketclient.cpp
)

add_executable(${CURRENT_PROJECT}
    ${CURRENT_PROJECT_SOURCES}
)

target_link_libraries(${CURRENT_PROJECT}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::WebSockets
    QScintilla::QScintilla
)
