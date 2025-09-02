cmake_minimum_required(VERSION 3.20)
project(Terreate)
option(TERREATE_DEBUG_BUILD "Build with debug flags" OFF)
option(TERREATE_BUILD_TEST "Build tests" ON)

include(FetchContent)

message(STATUS "Cloning Freetype...")
FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG VER-2-13-3
)
message(STATUS "Freetype cloned.")
message(STATUS "Configuring Freetype.")
FetchContent_MakeAvailable(freetype)
message(STATUS "Freetype configured.")

message(STATUS "Cloning SDL3...")
include(FetchContent)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.16
)
message(STATUS "SDL3 cloned.")
message(STATUS "Configuring SDL3.")
FetchContent_MakeAvailable(SDL3)
message(STATUS "SDL3 configured.")

if(TERREATE_DEBUG_BUILD)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")
  add_compile_definitions(TERREATE_DEBUG_BUILD)
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
endif()

# add_subdirectory(src)

if(TERREATE_BUILD_TEST)
  add_subdirectory(example)
endif()

