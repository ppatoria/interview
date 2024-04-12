cmake_minimum_required(VERSION 3.12)
project(matching_engine)

# Set the C++ compiler to clang++
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable ASan and LSan
# set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
# set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")

# Set the C++ standard library to libc++
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")

# Find ncurses library which provides terminfo
# find_library(NCURSES_LIBRARY NAMES ncurses)
# if(NOT NCURSES_LIBRARY)
#   message(FATAL_ERROR "ncurses library not found")
# endif()

# Find LLVM
# find_package(LLVM REQUIRED CONFIG)

# Add LLVM definitions and include directories
# add_definitions(${LLVM_DEFINITIONS})
# include_directories(${LLVM_INCLUDE_DIRS})

# Include the spdlog and libc++ include directories
include_directories(
  # ${SPDLOG_INCLUDE_DIR}
  # /usr/include/c++/v1
)

# Set the source directory
set(MATCHING_ENGINE ${CMAKE_SOURCE_DIR})
set(ORDERBOOK ${MATCHING_ENGINE}/orderbook)

# Add the executable
add_executable(mengine
  ${MATCHING_ENGINE}/main.cpp
  ${ORDERBOOK}/order.cpp
  ${ORDERBOOK}/pricelevel.cpp
  ${ORDERBOOK}/pricelevels.cpp
  ${ORDERBOOK}/orderbook.cpp
  ${ORDERBOOK}/orderbooks.cpp
)

# Map the LLVM components to their library names
# llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against spdlog, libc++, the LLVM libraries, and ncurses
# target_link_libraries(mengine ${SPDLOG_LIB} c++ c++abi ${llvm_libs} ${NCURSES_LIBRARY})

# Use lld as the linker
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
