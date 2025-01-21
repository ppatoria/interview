cmake_minimum_required(VERSION 3.10)

project(BenchmarkExample)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define build types with default set to Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type: Release, Debug, Valgrind" FORCE)
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

# Add executable and link libraries
add_executable(cache_benchmark benchmark.cpp)
target_link_libraries(cache_benchmark PRIVATE benchmark::benchmark pthread)

add_executable(cache_benchmark_aligned_data benchmark_aligned_data.cpp)
target_link_libraries(cache_benchmark_aligned_data PRIVATE benchmark::benchmark pthread)

add_executable(cache_benchmark_unrolling_loop unrolling_loop.cpp)
target_link_libraries(cache_benchmark_unrolling_loop PRIVATE benchmark::benchmark pthread)

# Optional: Add include directories
target_include_directories(cache_benchmark PRIVATE ${benchmark_INCLUDE_DIRS})

# Display build configuration for debugging
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS}")
