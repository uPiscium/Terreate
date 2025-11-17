project(TerreateExample)
message(STATUS "Configuring ${PROJECT_NAME}...")

include(options)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
)

set_build_mode()

target_include_directories(${PROJECT_NAME} PUBLIC ./include)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)
target_include_directories(${PROJECT_NAME} PUBLIC ../deps)

target_link_libraries(${PROJECT_NAME} PRIVATE Terreate)
setup_binary_dir(${PROJECT_NAME})

message(STATUS "${PROJECT_NAME} configured.")
