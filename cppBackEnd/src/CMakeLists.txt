cmake_minimum_required(VERSION 40)
project(element)

file(GLOB SOURCE_FILES "cppBackEnd/src/*.cpp")
add_library(${PROJECT_NAME} ${SOURCE_FILES})