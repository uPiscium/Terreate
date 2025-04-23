cmake_minimum_required(VERSION 3.20)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "C++ standard : " ${CMAKE_CXX_STANDARD})

# add_subdirectory(audio)

add_library(${PROJECT_NAME} STATIC executor.cpp uuid.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ../include/deps/)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
