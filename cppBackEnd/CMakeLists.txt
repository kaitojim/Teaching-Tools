cmake_minimum_required(VERSION 4.0)
project(test)
enable_testing()

add_subdirectory("./cppBackEnd/src")

add_executable(${PROJECT_NAME} test.cpp)
target_link_libraries(element)