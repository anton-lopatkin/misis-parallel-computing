cmake_minimum_required(VERSION 3.16)

project(lab05)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(lab05
    src/main.cpp
    src/plain.cpp
    src/scalar.cpp
    src/vec.cpp
    src/vec512.cpp
)

target_include_directories(lab05 PRIVATE include)

target_compile_options(lab05 PRIVATE
    $<$<CONFIG:Debug>:-O0 -g -mavx2 -mavx512f -mavx512bw>
    $<$<CONFIG:Release>:-O3 -march=native>
)
