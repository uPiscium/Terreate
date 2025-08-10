cmake_minimum_required(VERSION 3.20)
project(Terreate)
option(TERREATE_DEBUG_BUILD "Build with debug flags" OFF)
option(TERREATE_BUILD_TEST "Build tests" ON)

add_subdirectory(src)

if(TERREATE_BUILD_TEST)
  add_subdirectory(example)
endif()

if(TERREATE_DEBUG_BUILD)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
endif()
