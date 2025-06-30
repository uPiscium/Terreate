cmake_minimum_required(VERSION 3.20)
project(TerreateCoreModule)
message(STATUS "Configuring ${PROJECT_NAME}...")

if(TERREATE_DEBUG_BUILD)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
endif()

add_library(
  ${PROJECT_NAME} STATIC
  context.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC TerreateCommonModule TerreateGraphicModule)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../include)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../deps)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
                             LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
message(STATUS "${PROJECT_NAME} configured.")
