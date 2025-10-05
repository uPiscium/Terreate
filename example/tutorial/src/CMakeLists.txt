include(options)

add_executable(${PROJECT_NAME} main.cpp)

# target_link_libraries(${PROJECT_NAME} Terreate)
find_package(Vulkan REQUIRED)
target_link_libraries(
  ${PROJECT_NAME} PRIVATE
  Vulkan::Vulkan
  SDL3::SDL3
  freetype
)

target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_include_directories(${PROJECT_NAME} PUBLIC ../../../deps)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                 ${CMAKE_BINARY_DIR}/bin)
