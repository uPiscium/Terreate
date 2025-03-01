cmake_minimum_required(VERSION 3.20)
project(TerreateTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_CORE_TESTS "Build the core tests" OFF)
option(BUILD_AUDIO_TESTS "Build the audio tests" OFF)
option(BUILD_GRAPHICS_TESTS "Build the graphics tests" ON)
option(BUILD_IO_TESTS "Build the io tests" ON)

add_executable(${PROJECT_NAME} TerreateTest.cpp audiotest.cpp coretest.cpp graphictest.cpp iotest.cpp main.cpp)

if(BUILD_CORE_TESTS)
  add_compile_definitions(BUILD_CORE_TESTS)
endif()
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
if(BUILD_IO_TESTS)
  add_compile_definitions(BUILD_IO_TESTS)
endif()
target_link_libraries(${PROJECT_NAME} Terreate)
target_include_directories(${PROJECT_NAME} PUBLIC ../includes)
target_include_directories(${PROJECT_NAME} PUBLIC ../includes/Deps)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                 ${CMAKE_BINARY_DIR}/bin)
