project(TerreateVulkanModule)
message(STATUS "Configuring ${PROJECT_NAME}...")

add_library(
  ${PROJECT_NAME} STATIC
  debugger.cpp
  device.cpp
  instance.cpp
  queue.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ../../include)
target_include_directories(${PROJECT_NAME} PUBLIC ../../deps)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

message(STATUS "${PROJECT_NAME} configured.")
