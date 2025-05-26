cmake_minimum_required(VERSION 3.20)
include(FetchContent)
project(TRutil LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

message(STATUS "C++ standard: " ${CMAKE_CXX_STANDARD})
message(STATUS "Component: " ${PROJECT_NAME})

add_library(
  ${PROJECT_NAME} STATIC
  uuid.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ../../deps)
target_include_directories(${PROJECT_NAME} PUBLIC ../../include)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
                             LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
message(STATUS "Component: ${PROJECT_NAME} configured.")
