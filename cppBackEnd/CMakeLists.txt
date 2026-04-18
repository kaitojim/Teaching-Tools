cmake_minimum_required(VERSION 4.0)
project(Teaching_tools)

add_subdirectory(Tools)

add_executable(teaching main.cpp)
target_link_libraries(teaching PRIVATE Teacher_tools)