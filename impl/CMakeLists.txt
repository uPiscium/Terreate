cmake_minimum_required(VERSION 3.20)
include(FetchContent)

if(TERREATE_DEBUG_BUILD)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "C++ standard : " ${CMAKE_CXX_STANDARD})

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

message(STATUS "Cloning SDL...")
include(FetchContent)
set(SDL_SHARED OFF)
set(SDL_STATIC ON)
FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-3.2.16
)
message(STATUS "SDL cloned.")
message(STATUS "Configuring SDL.")
FetchContent_MakeAvailable(SDL3)
message(STATUS "SDL configured.")

add_subdirectory(module/common)
add_subdirectory(module/opengl)
add_subdirectory(module/sdl)

add_subdirectory(module/core)

add_library(
  ${PROJECT_NAME} STATIC
  dummy.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_include_directories(${PROJECT_NAME} PUBLIC ../deps)
target_link_libraries(
  ${PROJECT_NAME} PUBLIC
  TerreateCommonModule
  TerreateCoreModule
  TerreateOpenGLModule
  TerreateSDLModule
)
