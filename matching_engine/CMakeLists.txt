cmake_minimum_required(VERSION 3.12)
project(YourProjectName)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(mengine matching_engine.cpp)
