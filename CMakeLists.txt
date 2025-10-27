cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(Terreate)
option(TERREATE_DEBUG_BUILD "Build with debug flags" OFF)
option(TERREATE_BUILD_EXAMPLE "Build examples" OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(TERREATE_DEBUG_BUILD)
  message(STATUS "[Terreate] Building with debug flags.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer")
  add_compile_definitions(TERREATE_DEBUG_BUILD)
else()
  message(STATUS "[Terreate] Building with release flags.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
endif()

include(options)
include(freetype)
include(sdl3)

add_subdirectory(src)

if(TERREATE_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

