cmake_minimum_required(VERSION 3.12)
project(matching_engine)

include(spdlog.cmake)

set(CMAKE_CXX_COMPILER clang-18)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Find LLVM
find_package(LLVM REQUIRED CONFIG)

# Add LLVM definitions and include directories
add_definitions(${LLVM_DEFINITIONS})

include_directories(
  ${LLVM_INCLUDE_DIRS}
  ${SPDLOG_INCLUDE_DIR})
message("SPDLOG_INCLUDE_DIR: ${SPDLOG_INCLUDE_DIR}")

set(MATCHING_ENGINE ${CMAKE_SOURCE_DIR})
set(ORDERBOOK ${MATCHING_ENGINE}/orderbook)
add_executable(mengine
  ${MATCHING_ENGINE}/main.cpp
  ${ORDERBOOK}/order.cpp
  ${ORDERBOOK}/pricelevel.cpp
  # ${ORDERBOOK}/ordermetadata.cpp
  ${ORDERBOOK}/orderbook.cpp
  # ${ORDERBOOK}/orderbookhandler.cpp
  # ${ORDERBOOK}/debug.cpp
)

# Map the LLVM components to their library names
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against spdlog, the C++ standard library, and the LLVM libraries
target_link_libraries(mengine ${SPDLOG_LIB} "-lstdc++" ${llvm_libs})
set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld")
