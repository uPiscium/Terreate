project(TerreateCoreSDL)

message(STATUS "Configuring ${PROJECT_NAME}...")
add_library(
  ${PROJECT_NAME} STATIC
  audio.cpp
  camera.cpp
  event.cpp
  gamepad.cpp
  icon.cpp
  joystick.cpp
  mouse.cpp
  registry.cpp
  window.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ../../../include)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../deps)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

message(STATUS "Configured ${PROJECT_NAME}.")
