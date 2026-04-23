cmake_minimum_required(VERSION 3.16)

project(lab01)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab01
    src/main.cpp
    src/sieve.cpp
    src/timing.cpp
)

target_include_directories(lab01 PRIVATE include)
