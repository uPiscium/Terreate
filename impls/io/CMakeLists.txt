project(IOComponent)
message(STATUS "Configuring ${PROJECT_NAME}...")

add_library(${PROJECT_NAME} STATIC
  format/gltf.cpp
  format/json.cpp
  animation.cpp
  image.cpp
)
target_link_libraries(${PROJECT_NAME} PUBLIC CoreComponent)
target_include_directories(${PROJECT_NAME} PUBLIC ../../includes/Deps)
target_include_directories(${PROJECT_NAME} PUBLIC ../../includes)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
message(STATUS "${PROJECT_NAME} configured.")
