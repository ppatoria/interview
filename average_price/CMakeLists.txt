cmake_minimum_required(VERSION 3.20)
project(average_price VERSION 1.0)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(average_price average_price.cpp)

