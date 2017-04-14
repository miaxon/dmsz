cmake_minimum_required(VERSION 2.8.0)
project(dmsz)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

add_subdirectory(lib)
add_subdirectory(ini)
add_subdirectory(logger)
add_subdirectory(bin)
add_subdirectory(logbin)
