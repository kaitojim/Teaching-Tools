cmake_minimum_required(VERSION 4.0)
project(elementProcess)

add_definitions(-DNAPI_VERSION=9)
file(GLOB SOURCE_FILES "cppBackEnd/src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

execute_process(
  COMMAND node -p "require('node-addon-api').include"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE NODE_ADDON_API_DIR
)
string(STRIP ${NODE_ADDON_API_DIR} NODE_ADDON_API_DIR)
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR} ${CMAKE_JS_INC} "./cppBackEnd/header")
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

if(MSVC AND CMAKE_JS_NODELIB_DEF AND CMAKE_JS_NODELIB_TARGET)
  # Generate node.lib
  execute_process(COMMAND ${CMAKE_AR} /def:${CMAKE_JS_NODELIB_DEF} /out:${CMAKE_JS_NODELIB_TARGET} ${CMAKE_STATIC_LINKER_FLAGS})
endif()