cmake_minimum_required(VERSION 3.10) #设置cmake最低版本

# 设置项目名称 版本 语言
project(yuri VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON) # 自动编译ui文件
set(CMAKE_AUTOMOC ON) # 自动编译moc文件
set(CMAKE_AUTORCC ON) # 自动编译资源文件
set(CMAKE_CXX_STANDARD 17) # 设置c++ 最低版本
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 要求使用指定版本号
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # 开启生成 compile_commands.json

# 寻找qt依赖库
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS  Widgets LinguistTools )
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS  Widgets LinguistTools)
find_package(Qt6 COMPONENTS Network REQUIRED)
find_package(Qt6 COMPONENTS StateMachine REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Sql)


# 设置翻译文件
set(TS_FILES yuri_zh_CN.ts)
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
)

# 添加两个自项目
add_subdirectory(lib/system_login)
add_subdirectory(lib/my_table)

# 添加项目文件
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

# 如果qt 版本大于等于6
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    # 构建可执行问价
    qt_add_executable(yuri
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
# 如果是安卓
    if(ANDROID)
        add_library(yuri SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(yuri
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

# 链接文件
target_link_libraries(yuri PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(yuri PRIVATE Qt6::Sql)
target_link_libraries(yuri PRIVATE Qt6::Network)
target_link_libraries(yuri PRIVATE Qt6::StateMachine)
target_link_libraries(yuri PRIVATE system_login_lib)

# 设置项目属性
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
