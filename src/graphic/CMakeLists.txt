project(TerreateGraphics)

include(utils)

setup_build_flags()

add_library(
  ${PROJECT_NAME} STATIC
  dummy.cpp
)

find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

configure_target(
  ${PROJECT_NAME}
  ${CMAKE_SOURCE_DIR}/include
  "glm::glm;spdlog::spdlog;SDL3::SDL3"
)

# target_include_directories(
#   ${PROJECT_NAME} PUBLIC
#   ${CMAKE_SOURCE_DIR}/include
# )
# target_link_libraries(
#   ${PROJECT_NAME} PUBLIC
#   glm::glm
#   spdlog::spdlog
#   SDL3::SDL3
# )
