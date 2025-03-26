cmake_minimum_required(VERSION 3.31)
project(mizukistr)
set(CMAKE_C_STANDARD 23)

option(BUILD_EXAMPLES "Build examples. ")

add_library(mizukistr)

target_include_directories(mizukistr PUBLIC include)
add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

