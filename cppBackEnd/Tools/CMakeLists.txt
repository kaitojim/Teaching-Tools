cmake_minimum_required(VERSION 4.0)
project(Tools)

include_directories(.)
add_subdirectory(Element)

add_library(Teacher_tools
    random.h
    InputOption.h
)
target_link_libraries(Teacher_tools
    PUBLIC ElementClass
)