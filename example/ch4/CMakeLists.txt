project(TerreateExampleCh4)
message(STATUS "Configuring ${PROJECT_NAME}...")

include(options)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
)

set_build_mode()
setup_include(${PROJECT_NAME})
setup_link(${PROJECT_NAME})
setup_binary_dir(${PROJECT_NAME})

message(STATUS "${PROJECT_NAME} configured.")
