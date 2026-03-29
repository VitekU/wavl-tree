cmake_minimum_required(VERSION 3.10)
project(wavl)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(out tests/main.cpp)
add_executable(speed tests/speed_comparison.cpp)
