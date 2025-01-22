cmake_minimum_required(VERSION 3.10)

project(BenchmarkCachePrefetching)

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

# Add executables and link libraries
add_executable(cache_prefetch_benchmark prefetch_benchmark.cpp)
target_link_libraries(cache_prefetch_benchmark PRIVATE benchmark::benchmark pthread)

# Optional: Add include directories
target_include_directories(cache_prefetch_benchmark PRIVATE ${benchmark_INCLUDE_DIRS})

# Function to create custom benchmark targets
function(create_benchmark_target target_name)
    add_custom_target(run_${target_name}
        COMMAND echo 1 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo
        COMMAND sudo cpufreq-set -g performance
        COMMAND taskset -c 2 ./${target_name} --benchmark_min_time=1.0 --benchmark_repetitions=10
        COMMAND echo 0 | sudo tee /sys/devices/system/cpu/intel_pstate/no_turbo
        COMMAND sudo cpufreq-set -g ondemand
        DEPENDS ${target_name}  # Ensure the executable is built first
        COMMENT "Running benchmark for ${target_name} with specific CPU settings"
    )
endfunction()

# Create custom targets for each benchmark executable
create_benchmark_target(cache_prefetch_benchmark)

# Combined target to run all benchmarks
add_custom_target(run_all_benchmarks
    DEPENDS run_cache_benchmark run_cache_benchmark_aligned_data run_cache_benchmark_unrolling_loop
    COMMENT "Running all benchmarks"
)

# Display build configuration for debugging
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "C++ Flags Release: ${CMAKE_CXX_FLAGS_RELEASE}")
