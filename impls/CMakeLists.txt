cmake_minimum_required(VERSION 3.20)
include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
message(STATUS "C++ standard : " ${CMAKE_CXX_STANDARD})

add_subdirectory(core)
add_subdirectory(audio)
add_subdirectory(graphic)
add_subdirectory(io)

add_library(${PROJECT_NAME} STATIC context.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ../includes/Deps/)
target_include_directories(${PROJECT_NAME} PUBLIC ../includes)
target_link_libraries(${PROJECT_NAME} CoreComponent AudioComponent GraphicComponent)
