cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(GetGitVersion)

# Getting our repository version
get_git_version(GIT_VERSION)
string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" VERSION ${GIT_VERSION})

configure_file(def.h.in ${CMAKE_CURRENT_BINARY_DIR}/def.h)

message("-- Version: ${VERSION}")
message("-- Corporation: Monolit")
message(STATUS "Generating build using CMake v${CMAKE_VERSION}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

# Get source files list
file(GLOB SRC_LIST *.cpp)

file(GLOB H_LIST *.h)


add_library(dmsz SHARED ${OS_BUNDLE} ${SRC_LIST} ${H_LIST})
target_link_libraries(dmsz)

