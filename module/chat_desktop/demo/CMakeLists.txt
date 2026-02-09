cmake_minimum_required(VERSION 3.25)
project(chatDesktop)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)                                                               

find_package(Qt5 REQUIRED COMPONENTS Widgets)

aux_source_directory(src SRC)
file(GLOB HEADERS include/*.h)
file(GLOB RESOURCES qrc/navlist/*.qrc)
qt_add_resources(QRC_SOURCES ${RESOURCES})

add_executable(${PROJECT_NAME} ${SRC} ${HEADERS} ${QRC_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)