cmake_minimum_required(VERSION 3.20)
project(Terreate)
option(TERREATE_DEBUG_BUILD "Build with debug flags" OFF)
option(TERREATE_BUILD_EXAMPLE "Build examples" ON)

include(FetchContent)

message(STATUS "[Terreate] Setting up Freetype...")
message(STATUS "[Freetype] cloning...")
FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG VER-2-13-3
  GIT_PROGRESS TRUE
)
message(STATUS "[Freetype] cloned.")
message(STATUS "[Freetype] configuring...")
FetchContent_MakeAvailable(freetype)
message(STATUS "[Freetype] configured.")

message(STATUS "[Terreate] Setting up SDL3 components...")
message(STATUS "[SDL3] Setting up...")
set(SDL_SHARED OFF)
set(SDL_INTERFACE ON)
set(SDL_STATIC ON)
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.24
  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3)
set(SDL3_DIR "${sdl3_BINARY_DIR}")
message(STATUS "[SDL3] Ready.")

message(STATUS "[SDL3_image] Setting up...")
set(BUILD_SHARED_LIBS OFF)
FetchContent_Declare(
  SDL3_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG release-3.2.4
  OVERRIDE_FIND_PACKAGE
)
FetchContent_MakeAvailable(SDL3_image)
message(STATUS "[SDL3_image] Ready.")
# FetchContent_MakeAvailable(SDL3 SDL3_image)

if(TERREATE_DEBUG_BUILD)
  message(STATUS "[Terreate] Building with debug flags.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")
  add_compile_definitions(TERREATE_DEBUG_BUILD)
else()
  message(STATUS "[Terreate] Building with release flags.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
endif()

if(TERREATE_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

