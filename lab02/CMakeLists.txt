cmake_minimum_required(VERSION 3.16)

project(lab02)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab02
    src/main.cpp
    src/utils.cpp
    src/classic.cpp
    src/transpose.cpp
    src/buffered.cpp
    src/blocked.cpp
)

target_include_directories(lab02 PRIVATE include)

target_compile_options(lab02 PRIVATE
    $<$<CONFIG:Debug>:-O0 -g>
    $<$<CONFIG:Release>:-O3 -march=native>
)
