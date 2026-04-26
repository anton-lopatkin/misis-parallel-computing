cmake_minimum_required(VERSION 3.16)

project(lab07)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab07
    src/main.cpp
    src/naive.cpp
    src/avx512.cpp
)

target_include_directories(lab07 PRIVATE include)

target_compile_options(lab07 PRIVATE -O3 -march=native)
