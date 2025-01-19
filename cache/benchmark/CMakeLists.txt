# cmake_minimum_required(VERSION 3.10)

# project(BenchmarkExample)

# # Set C++ Standard
# set(CMAKE_CXX_STANDARD 20)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Find Google Benchmark library
# find_package(benchmark REQUIRED)

# # Add executable and link the benchmark library
# add_executable(cache_benchmark benchmark.cpp)
# target_link_libraries(cache_benchmark PRIVATE benchmark::benchmark)

cmake_minimum_required(VERSION 3.10)

project(BenchmarkExample)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Google Benchmark library
find_package(benchmark REQUIRED)

# Add executable
add_executable(cache_benchmark benchmark.cpp)

# Link libraries
target_link_libraries(cache_benchmark PRIVATE benchmark::benchmark pthread)

# Optional: Add include directories
target_include_directories(cache_benchmark PRIVATE ${benchmark_INCLUDE_DIRS})
