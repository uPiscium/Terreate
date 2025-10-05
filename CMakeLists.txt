project(Terreate)
option(TERREATE_DEBUG_BUILD "Build with debug flags" OFF)
option(TERREATE_BUILD_EXAMPLE "Build examples" ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(freetype)
include(sdl3)
include(sdl3_image)

if(TERREATE_BUILD_EXAMPLE)
  add_subdirectory(example)
endif()

