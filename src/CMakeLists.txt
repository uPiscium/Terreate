add_subdirectory(common)
add_subdirectory(core)

add_library(
  ${PROJECT_NAME} STATIC
  Terreate.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ../../include)
target_include_directories(${PROJECT_NAME} PUBLIC ../../deps)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

find_package(Vulkan REQUIRED)

target_link_libraries(
  Terreate PRIVATE
  Vulkan::Vulkan
  SDL3::SDL3
  TerreateCommon
  TerreateCoreSDL
  TerreateCoreVulkan
)
