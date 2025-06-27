cmake_minimum_required(VERSION 3.20)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "C++ standard : " ${CMAKE_CXX_STANDARD})

add_subdirectory(module/common)
add_subdirectory(module/opengl)

add_library(
  ${PROJECT_NAME} STATIC
  module/opengl.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_include_directories(${PROJECT_NAME} PUBLIC ../deps)
target_link_libraries(${PROJECT_NAME} TerreateCommonComponent TerreateGraphicComponent)
