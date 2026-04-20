cmake_minimum_required(VERSION 4.0)
project(testing)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory("./src")
add_subdirectory("./header")

add_executable(${PROJECT_NAME} tests.cpp)
target_link_libraries(elementInterface INTERFACE element)
target_link_libraries(${PROJECT_NAME} elementInterface)