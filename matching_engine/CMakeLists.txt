cmake_minimum_required(VERSION 3.12)
project(matching_engine)

# set(CMAKE_CXX_COMPILER clang-18)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)


# find_package(LLVM REQUIRED CONFIG)

# Optionally include directories and definitions for LLVM
# message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
# message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
# include_directories(${LLVM_INCLUDE_DIRS})
# add_definitions(${LLVM_DEFINITIONS})

# Identify the LLVM libraries you need (replace with actual tool names)
# llvm_map_components_to_libnames(llvm_libs irreader  # Example tool, replace with actual ones
#                                                        # Add more tools as needed, separated by spaces
#                                  )

# Add your executable
add_executable(mengine
  ${CMAKE_SOURCE_DIR}/matching_engine.cpp
  ${CMAKE_SOURCE_DIR}/orderbook/orderbook.cpp
)

# Link your executable with the required LLVM libraries
# target_link_libraries(mengine ${llvm_libs})
