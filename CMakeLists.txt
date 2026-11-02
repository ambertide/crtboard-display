cmake_minimum_required(VERSION 3.14)
project(crtboard VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(crtboard OBJECT src/Serial.cpp src/Render.cpp)

# Placeholder to compile on Macos
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_include_directories(crtboard PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
endif()

add_executable(crtboard-display src/crtboard.cpp)
target_link_libraries(crtboard-display PRIVATE crtboard)