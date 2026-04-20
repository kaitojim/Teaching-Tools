cmake_minimum_required(VERSION 4.0)
project(element)

add_library(${PROJECT_NAME} 
    element.cpp
    sentence.cpp
    word.cpp
)