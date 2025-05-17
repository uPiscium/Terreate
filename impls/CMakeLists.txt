cmake_minimum_required(VERSION 3.20)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "C++ standard : " ${CMAKE_CXX_STANDARD})

add_library(
  ${PROJECT_NAME} STATIC
  core/api.cpp
  core/context.cpp
  core/instance.cpp
  util/debugger.cpp
  uuid.cpp
)

find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw Vulkan::Vulkan)
target_include_directories(${PROJECT_NAME} PUBLIC ../deps/)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
