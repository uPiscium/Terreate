cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(TERREATE_DEBUG_BUILD)
  message(STATUS "[Terreate] Building with debug flags.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra")
  add_compile_definitions(TERREATE_DEBUG_BUILD)
else()
  message(STATUS "[Terreate] Building with release flags.")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
endif()

