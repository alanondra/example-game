cmake_minimum_required(VERSION 4.1)

project(ExampleGame)

include(${CMAKE_SOURCE_DIR}/.cmake/cpm.cmake)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_MODULE_EXTENSIONS .cppm)

include(${CMAKE_SOURCE_DIR}/.cmake/output.cmake)

add_subdirectory(projects/common)
add_subdirectory(projects/launcher)
