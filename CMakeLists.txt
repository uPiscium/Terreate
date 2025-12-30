cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(Terreate)
option(TERREATE_DEBUG_BUILD "Build with debug flags" OFF)
option(TERREATE_BUILD_TEST "Build tests" ON)

include(utils)

setup_build_flags()
add_subdirectory(src)

if(TERREATE_BUILD_TEST)
  add_subdirectory(test)
endif()
