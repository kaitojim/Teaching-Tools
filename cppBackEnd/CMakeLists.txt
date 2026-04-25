cmake_minimum_required(VERSION 4.0)
project(testing)

# Set c++ standard
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

# Set output folder
set(CMAKE_BINARY_DIR ..)

enable_testing()

add_subdirectory("./src")
add_subdirectory("./header")

add_executable(${PROJECT_NAME} tests.cpp)
target_link_libraries(elementInterface INTERFACE element)
target_link_libraries(${PROJECT_NAME} elementInterface)

# Testing variable
set(blank       "blank")
# ProcessMode
set(word        "word")
set(sentence    "sentence")
# FunctionMode
set(scramble    "scramble")
set(fill        "fillintheblank")
# AddBlkMode
set(wordChar    "fillintheblank(wordorcharacter)")
set(percent     "fillintheblank(percentage)")
set(idx         "fillintheblank(index)")
set(firstWord   "fillintheblank(firstletterorword)")

# tests for word
add_test(NAME w_fillChar COMMAND ${PROJECT_NAME} 
    ${word} 
    ${fill} 
    ${wordChar} 
    "o" 
    "word, oelephant, ground floor"
)
add_test(NAME w_fillPer COMMAND ${PROJECT_NAME} 
    ${word} 
    ${fill} 
    ${percent} 
    "50"
    "word, elephant, ground floor"
)
add_test(NAME w_fillIdx COMMAND ${PROJECT_NAME} 
    ${word} 
    ${fill} 
    ${idx} 
    1 
    "word, elephant, ground floor"
)
add_test(NAME w_fillFirstWord COMMAND ${PROJECT_NAME} 
    ${word} 
    ${fill} 
    ${firstWord} 
    0 
    "word, elephant, ground floor"
)
add_test(NAME w_scramble COMMAND ${PROJECT_NAME} 
    ${word} 
    ${scramble} 
    ${blank} 
    0 
    "word, elephant, ground floor"
)


# tests for sentence
add_test(NAME s_fillChar COMMAND ${PROJECT_NAME} 
    ${sentence} 
    ${fill} 
    ${wordChar} 
    "was" 
    "I was hungry. She was angry. Was she thirsty?. I was here first!"
)
add_test(NAME s_fillPer COMMAND ${PROJECT_NAME} 
    ${sentence} 
    ${fill} 
    ${percent} 
    "50"
    "I was hungry. She was angry. Was she thirsty?. I was here first!"
)
add_test(NAME s_scramble COMMAND ${PROJECT_NAME} 
    ${sentence} 
    ${scramble} 
    ${blank} 
    0 
    "I was hungry. She was angry. Was she thirsty?. I was here first!"
)