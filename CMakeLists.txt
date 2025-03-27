cmake_minimum_required(VERSION 3.31)
project(mizukistr)
set(CMAKE_C_STANDARD 23)

option(BUILD_EXAMPLES "Build examples. ")

option(MIZUKI_STRING_NO_ALLOC "Disables all allocation related feature" OFF)

add_library(mizukistr)

target_include_directories(mizukistr PUBLIC include)
if(MIZUKI_STRING_NO_ALLOC)
    add_compile_definitions(MIZUKI_STRING_NO_ALLOC)
endif()
add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

