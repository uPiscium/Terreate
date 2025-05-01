cmake_minimum_required(VERSION 3.20)
project(TerreateTest CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_AUDIO_TESTS "Build the audio tests" OFF)
option(BUILD_GRAPHICS_TESTS "Build the graphics tests" ON)

add_executable(${PROJECT_NAME} main.cpp api.cpp resource.cpp)

if(BUILD_AUDIO_TESTS)
  add_compile_definitions(BUILD_AUDIO_TESTS)
  if(WIN32)
    add_custom_command(
      TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:OpenAL>/OpenAL32.dll
              $<TARGET_FILE_DIR:${PROJECT_NAME}>)
  else()
    add_custom_command(
      TARGET ${PROJECT_NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE_DIR:OpenAL>/libopenal.so
              $<TARGET_FILE_DIR:${PROJECT_NAME}>/)
  endif()
endif()
if(BUILD_GRAPHICS_TESTS)
  add_compile_definitions(BUILD_GRAPHIC_TESTS)
endif()

find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} Terreate glfw Vulkan::Vulkan)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_include_directories(${PROJECT_NAME} PUBLIC ../include/deps)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                 ${CMAKE_BINARY_DIR}/bin)
