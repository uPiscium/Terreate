project(AudioComponent)
message(STATUS "Configuring ${PROJECT_NAME}...")

message(STATUS "Cloning OpenAL-Soft...")
set(ALSOFT_DLOPEN OFF)
set(ALSOFT_UTILS OFF)
set(ALSOFT_EXAMPLES OFF)
FetchContent_Declare(
  openal_soft
  GIT_REPOSITORY https://github.com/kcat/openal-soft.git
  TAG v1.23.1)
FetchContent_MakeAvailable(openal_soft)
set_target_properties(
  OpenAL PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
message(STATUS "OpenAL-Soft cloned.")

add_library(${PROJECT_NAME} STATIC ALhelper.cpp buffer.cpp
                                     context.cpp source.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC CoreComponent)
target_include_directories(${PROJECT_NAME} PUBLIC ../../include/deps)
target_include_directories(${PROJECT_NAME} PUBLIC ../../include)
set_target_properties(
  ${PROJECT_NAME} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
message(STATUS "${PROJECT_NAME} configured.")
