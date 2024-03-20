# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/ppatoria/dev/interview/matching_engine/build/thirdparty/SPDLOG/src/SPDLOG"
  "/home/ppatoria/dev/interview/matching_engine/build/thirdparty/SPDLOG/src/SPDLOG-build"
  "/home/ppatoria/dev/interview/matching_engine/build/thirdparty/SPDLOG"
  "/home/ppatoria/dev/interview/matching_engine/build/thirdparty/SPDLOG/tmp"
  "/home/ppatoria/dev/interview/matching_engine/build/thirdparty/SPDLOG/src/SPDLOG-stamp"
  "/home/ppatoria/dev/interview/matching_engine/build/thirdparty/SPDLOG/src"
  "/home/ppatoria/dev/interview/matching_engine/build/thirdparty/SPDLOG/src/SPDLOG-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/ppatoria/dev/interview/matching_engine/build/thirdparty/SPDLOG/src/SPDLOG-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/ppatoria/dev/interview/matching_engine/build/thirdparty/SPDLOG/src/SPDLOG-stamp${cfgdir}") # cfgdir has leading slash
endif()
