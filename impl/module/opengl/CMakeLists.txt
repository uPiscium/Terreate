cmake_minimum_required(VERSION 3.20)
project(TerreateGraphicComponent)
message(STATUS "Configuring ${PROJECT_NAME}...")

if(TERREATE_DEBUG_BUILD)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
endif()

message(STATUS "Cloning GLFW...")
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4)
FetchContent_MakeAvailable(glfw)
message(STATUS "GLFW cloned.")

message(STATUS "Cloning Freetype...")
message(STATUS "This may take a while.")
FetchContent_Declare(
  freetype
  GIT_REPOSITORY https://github.com/freetype/freetype.git
  GIT_TAG VER-2-13-3)
FetchContent_MakeAvailable(freetype)
message(STATUS "Freetype cloned.")

add_library(
  ${PROJECT_NAME} STATIC
  buffer.cpp
  compute.cpp
  converter.cpp
  cursor.cpp
  font.cpp
  gl.cpp
  icon.cpp
  joystick.cpp
  mouse.cpp
  screen.cpp
  shader.cpp
  text.cpp
  texture.cpp
  window.cpp
  winhelper.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC TerreateCommonComponent)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC freetype)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../include)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../deps)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
                             LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
message(STATUS "${PROJECT_NAME} configured.")
