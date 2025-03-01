cmake_minimum_required(VERSION 3.20)
project(Terreate)
option(TERREATE_BUILD_TEST "Build test" ON)

add_subdirectory(impls)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fsanitize=address")

if(TERREATE_BUILD_TEST)
  add_subdirectory(tests/impls)
endif()
