project(TerreateTest)

include(utils)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
)

configure_target(
  ${PROJECT_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  "TerreateCore;TerreateGraphics"
)

# target_include_directories(
#   ${PROJECT_NAME} PRIVATE
#   ${CMAKE_SOURCE_DIR}/include
# )
# target_link_libraries(
#   TerreateTest PRIVATE
#   TerreateCore
#   TerreateGraphics
# )
