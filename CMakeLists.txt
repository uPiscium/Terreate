cmake_minimum_required(VERSION 3.30)
project(Terreate)
option(TERREATE_DEBUG_BUILD "Build with debug flags" OFF)
option(TERREATE_BUILD_EXAMPLE "Build examples" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(options)
include(freetype)
include(sdl3)

if(TERREATE_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

