cmake_minimum_required(VERSION 3.5)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})
option(USE_SCINTILLA "Use the scintilla component to add syntax highlighting." ON)

set(CURRENT_PROJECT qtafbclient)

project(${CURRENT_PROJECT})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5WebSockets REQUIRED)

set(mainwindow_ui "mainwindow.ui")
if(USE_SCINTILLA)
	find_package(QScintilla REQUIRED)
	message(STATUS "Using QScintilla!")
	add_definitions(-DUSE_SCINTILLA)
else()
    message(STATUS "QScintilla disabled!")
    file(READ ${mainwindow_ui} ui_content)
    
    set(mainwindow_ui "${CMAKE_BINARY_DIR}/mainwindow.ui")
    
    string(REGEX REPLACE "<customwidgets>.*</customwidgets>" "" ui_content ${ui_content})
    string(REPLACE "QsciScintilla" "QPlainTextEdit" ui_content ${ui_content})
    file(WRITE ${mainwindow_ui} ${ui_content})
endif()

message(STATUS "UI file: ${mainwindow_ui}")

include_directories(${QScintilla_INCLUDE_DIRS})

qt5_wrap_ui(ui_mainwindow ${mainwindow_ui})

set(CURRENT_PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    qafbwebsocketclient.cpp
    ${ui_mainwindow}
)

add_executable(${CURRENT_PROJECT}
    ${CURRENT_PROJECT_SOURCES}
)

target_link_libraries(${CURRENT_PROJECT}
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    Qt5::WebSockets
)

if(USE_SCINTILLA)
	target_link_libraries(${CURRENT_PROJECT} QScintilla::QScintilla)
endif()
