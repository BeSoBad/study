cmake_minimum_required(VERSION 2.8)
project(NM_LAB_1)
include_directories("include")

set(SOURCE_EXE_MAIN main.cpp)
set(SOURCE_LIB_MATRIX matrix.cpp)
set(SOURCE_LIB_VECTOR vector.cpp)
set(SOURCE_LIB_OPS operations.cpp)

add_library(matrix STATIC ${SOURCE_LIB_MATRIX})
add_library(vector STATIC ${SOURCE_LIB_VECTOR})
add_library(operations STATIC ${SOURCE_LIB_OPS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_executable(server ${SOURCE_EXE_MAIN})
target_link_libraries(main matrix vector operations)
