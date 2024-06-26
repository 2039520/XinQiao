cmake_minimum_required(VERSION 3.5)

project(LogIn VERSION 0.1 LANGUAGES CXX)

set(CODE_DIRECTORY_PATH D:/XinQiao/Code)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)


set(CMAKE_USER_MAKEFILE /CMakeLists.txt.user)

# 包含code目录及其子目录下的所有文件
file(GLOB_RECURSE CODE_FILES "../Code/*.cpp"  "../Code/*.h"  "../Code/*.ui")

# 输出匹配到的文件列表
message("file list:")
foreach(file ${CODE_FILES})
    message("  ${file}")
endforeach()

# 将匹配到的文件添加到源文件列表中
list(APPEND PROJECT_SOURCES ${CODE_FILES})

# 包含自定义控件头文件搜索路径
include_directories(${CODE_DIRECTORY_PATH}/CustomControlH)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(LogIn
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}

    )
endif()

include(GNUInstallDirs)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(LogIn)
endif()
# 将Qt模块的链接库添加到可执行文件中
target_link_libraries(LogIn PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# 链接到 Windows Socket 库
target_link_libraries(LogIn PRIVATE ws2_32.lib)
