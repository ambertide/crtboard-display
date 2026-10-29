cmake_minimum_required(VERSION 3.14)
project(crtboard VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 20)

add_library(crtboard OBJECT src/Serial.cpp)

target_include_directories(crtboard PRIVATE include)

add_executable(crtboard-display src/crtboard.cpp)
target_link_libraries(crtboard-display PRIVATE crtboard)