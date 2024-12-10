# [WARNING] DO NOT MODIFY THIS FILE
cmake_minimum_required(VERSION 3.10)
project(bplustree)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUNIX -Wall -g")

include_directories(${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})


# do not add/remove c++ files
file(GLOB SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(b+tree ${SRCS})