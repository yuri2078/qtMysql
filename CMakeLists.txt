cmake_minimum_required(VERSION 3.10)

project(yuri VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS  Widgets LinguistTools )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS  Widgets LinguistTools)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 COMPONENTS StateMachine REQUIRED)

# find_package(Qt6 REQUIRED COMPONENTS StateMachine)
# target_link_libraries(mytarget PRIVATE Qt6::StateMachine)

find_package(Qt6 REQUIRED COMPONENTS Sql)
# -DCMAKE_TOOLCHAIN_FILE=$path_to_installed_qt/lib/cmake/Qt6/qt.toolchain.cmake
set(TS_FILES yuri_zh_CN.ts)
# add_compile_options(-Wall -Werror -Wstrict-prototypes -Wmissing-prototypes)

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
)

add_subdirectory(lib/system_login)

set(PROJECT_SOURCES
    main.cpp
    ${TS_FILES}
    include/setting.h
    include/mainwindow.h
    include/login.h
    include/myclient.h
    include/mylineedit.h
    include/myserver.h
    include/note.h
    include/qmysql.h

    src/login.cpp
    src/mainwindow.cpp  
    src/myclient.cpp
    src/myserver.cpp
    src/setting.cpp
    src/mylineedit.cpp
    src/note.cpp
    src/qmysql.cpp

    ui/login.ui
    ui/mainwindow.ui

    images.qrc
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(yuri
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET yuri APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(yuri SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(yuri
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(yuri PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(yuri PRIVATE Qt6::Sql)
target_link_libraries(yuri PRIVATE Qt6::Network)
target_link_libraries(yuri PRIVATE Qt6::StateMachine)
target_link_libraries(yuri PRIVATE my_dialog)

set_target_properties(yuri PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS yuri
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(yuri)
endif()
