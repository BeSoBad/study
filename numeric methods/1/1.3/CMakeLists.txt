cmake_minimum_required(VERSION 2.8)
project(NM_LAB_1.3)
include_directories("include/")

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE RelWithDebInfo)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g -O0")

set(SOURCE_EXE_MAIN main.cpp)
set(SOURCE_LIB_MATRIX include/matrix.cpp)
set(SOURCE_LIB_VECTOR include/vector.cpp)
set(SOURCE_LIB_OPS include/operations.cpp)

add_library(matrix STATIC ${SOURCE_LIB_MATRIX})
add_library(vector STATIC ${SOURCE_LIB_VECTOR})
add_library(operations STATIC ${SOURCE_LIB_OPS})

add_executable(main ${SOURCE_EXE_MAIN})
target_link_libraries(main matrix vector operations)
