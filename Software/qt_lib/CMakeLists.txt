cmake_minimum_required(VERSION 3.16)

project(qt_lib VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC TRUE)

find_package(Qt6 6.2 COMPONENTS Quick Core  REQUIRED)



qt_add_executable(appqt_lib
    main.cpp
    mygameserver.h
    myquickpainteditem.h
)

qt_add_qml_module(appqt_lib
    URI qt_lib
    VERSION 1.0
    QML_FILES main.qml debugwindow.qml
)

set_target_properties(appqt_lib PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appqt_lib
   PRIVATE Qt6::Quick )

install(TARGETS appqt_lib
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
