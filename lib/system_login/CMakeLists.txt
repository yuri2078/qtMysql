cmake_minimum_required(VERSION 3.10)  # 设置CMake的最低版本要求为3.10

project(system_login VERSION 0.1 LANGUAGES CXX)  # 设置项目名称和版本号，使用C++语言

set(CMAKE_AUTOUIC ON)  # 自动处理UI文件
set(CMAKE_AUTOMOC ON)  # 自动处理MOC文件
set(CMAKE_AUTORCC ON)  # 自动处理资源文件

set(CMAKE_CXX_STANDARD 17)  # 设置C++标准为C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 要求使用指定的C++标准

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)  # 查找Qt库，要求包含Widgets和LinguistTools组件
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)  # 使用找到的Qt库

set(TS_FILES init_zh_CN.ts)  # 设置翻译文件的变量

set(PROJECT_SOURCES
    student_login.ui
    student_system_login.cpp
    student_system_login.h
    images.qrc
    ${TS_FILES}
)  # 设置项目源文件的变量

INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
)  # 包含项目源文件目录

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)  # 如果Qt版本大于等于6
    qt_add_executable(system_login
        MANUAL_FINALIZATION
        main.cpp
        ${PROJECT_SOURCES}
    )  # 使用qt_add_executable创建可执行文件

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})  # 创建翻译文件
else()
    if(ANDROID)  # 如果是Android平台
        add_library(system_login SHARED
            ${PROJECT_SOURCES}
        )  # 创建共享库
    else()
        add_executable(system_login
            ${PROJECT_SOURCES}
        )  # 创建可执行文件
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})  # 创建翻译文件
endif()

set_target_properties(system_login PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)  # 设置目标属性，用于生成MacOS应用程序包和Windows可执行文件

install(TARGETS system_login
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)  # 安装目标文件

if(QT_VERSION_MAJOR EQUAL 6)  # 如果Qt版本为6
    qt_finalize_executable(system_login)  # 完成可执行文件的设置
endif()

# 创建共享库
add_library(system_login_lib SHARED ${PROJECT_SOURCES})  

target_link_libraries(system_login_lib PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)  # 链接Qt库
target_link_libraries(system_login PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)  # 链接Qt库

