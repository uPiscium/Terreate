cmake_minimum_required(VERSION 3.20)
project(Terreate)
option(TERREATE_DEBUG_BUILD "Build with debug flags" OFF)
option(TERREATE_BUILD_TEST "Build test" ON)

add_subdirectory(impls)

if(TERREATE_DEBUG_BUILD)
  message(STATUS "Building with debug flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -fsanitize=address -DTERREATE_DEBUG_BUILD")
else()
  message(STATUS "Building with release flags")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall")
endif()

if(TERREATE_BUILD_TEST)
  add_subdirectory(tests/impls)
endif()
