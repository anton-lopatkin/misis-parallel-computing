cmake_minimum_required(VERSION 3.16)

project(lab03)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab03
    src/main.cpp
)

target_include_directories(lab03 PRIVATE include)
