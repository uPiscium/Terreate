cmake_minimum_required(VERSION 3.20)
include(FetchContent)
project(TRcore LANGUAGES CXX)

if(TERREATE_DEBUG_BUILD)
  message(STATUS "Building with debug flags")
  add_compile_options(-g -O0 -Wall -fsanitize=address)
  add_link_options(-fsanitize=address)
  add_compile_definitions(TERREATE_DEBUG_BUILD)
else()
  message(STATUS "Building with release flags")
  add_compile_options(-O3 -Wall)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "C++ standard: " ${CMAKE_CXX_STANDARD})
message(STATUS "Component: " ${PROJECT_NAME})

add_library(
  ${PROJECT_NAME} STATIC
  command.cpp
  context.cpp
  debugger.cpp
  device.cpp
  framebuffer.cpp
  pipeline.cpp
  queue.cpp
  swapchain.cpp
  sync.cpp
  window.cpp
  winevent.cpp
)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC freetype)
target_include_directories(${PROJECT_NAME} PUBLIC ../../deps)
target_include_directories(${PROJECT_NAME} PUBLIC ../../include)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
                             LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
message(STATUS "Component: ${PROJECT_NAME} configured.")
