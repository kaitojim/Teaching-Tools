cmake_minimum_required(VERSION 4.0)
project(Element)

include_directories(..)
include_directories(.)

add_library(ElementClass 
    element.cpp
    word.cpp
    sentence.cpp
)