cmake_minimum_required(VERSION 3.31)

project(Example)

include(${CMAKE_SOURCE_DIR}/.cmake/cpm.cmake)

add_subdirectory(packages/game)
