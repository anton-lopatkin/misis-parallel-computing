cmake_minimum_required(VERSION 3.16)
project(lab04)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab04
    src/utils.cpp
    src/seq.cpp
    src/rand.cpp
    src/randidx.cpp
    src/main.cpp
)

target_include_directories(lab04 PRIVATE include)
target_compile_options(lab04 PRIVATE -O2 -march=native)
