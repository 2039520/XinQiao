cmake_minimum_required(VERSION 3.2)
project(PdChat)
set(CMAKE_CXX_STANDARD 17)
# 添加 -g 选项以生成调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

add_executable(PdChat main.cpp
        head/main.h
        "head/PdChat/PdChat206.h"
        "head/PdSdk/Pd.h"
        "head/PdSdk/SearchAlgorithm.h"
        "head/PdSdk/SqList.h"
         "head/DataBase/DataBase.cpp" "head/DataBase/DataBase.h" "head/PdSdk/Pd.cpp" "head/PdChat/PdChat206.cpp" "head/FileIO/FileIo.cpp")

target_link_libraries(PdChat mysqlcppconn )
