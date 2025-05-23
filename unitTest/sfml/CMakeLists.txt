cmake_minimum_required(VERSION 3.10)  # 设置 CMake 最低版本
project(SFML)  # 项目名称

set(CMAKE_CXX_STANDARD 17)  # 设置 C++ 标准
include_directories(include ../common)

aux_source_directory(src SRC_FILES)
add_executable (SFML ${SRC_FILES})

# 链接 SFML 库
target_link_libraries(SFML sfml-graphics sfml-window sfml-system)