cmake_minimum_required(VERSION 3.20)

project(package-browser)

find_package(wxWidgets CONFIG REQUIRED COMPONENTS core base)

add_executable(package-browser main.cpp)

target_link_libraries(package-browser wx::core)
