cmake_minimum_required(VERSION 3.16)

project(NetworkSimulator2)

include_directories(/usr/include/x86_64-linux-gnu/qt5)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets Charts)

add_subdirectory(src)



