cmake_minimum_required(VERSION 3.14)
project(RISCV)

set(CMAKE_CXX_STANDARD 14)

add_executable(code main.cpp operator.hpp RISCV.hpp)
