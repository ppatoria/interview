cmake_minimum_required(VERSION 3.10)

project(Practice)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define build types with default set to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type: Release, Debug, Valgrind" FORCE)
endif()

# Configure CMake build types
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -funroll-loops")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -funroll-loops")
set(CMAKE_CXX_FLAGS_VALGRIND "-O1 -g -fno-inline -fno-omit-frame-pointer")

# Handle custom Valgrind build type
if(CMAKE_BUILD_TYPE STREQUAL "Valgrind")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_VALGRIND}")
endif()

# Find Google Benchmark library
find_package(benchmark REQUIRED)

# Add executables and link libraries
add_executable(lru_dq lru_dq.cpp)
add_executable(lru_linked_list lru_linked_list.cpp)


# Display build configuration for debugging
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "C++ Flags Release: ${CMAKE_CXX_FLAGS_RELEASE}")
