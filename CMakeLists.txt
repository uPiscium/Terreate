cmake_minimum_required(VERSION 3.25)
project(terreate LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(project_lib src/lib.cpp)
target_include_directories(project_lib PUBLIC include)

add_executable(project_app src/main.cpp)
target_link_libraries(project_app PRIVATE project_lib)

enable_testing()
add_executable(project_tests tests/test_smoke.cpp)
target_link_libraries(project_tests PRIVATE project_lib)
add_test(NAME smoke COMMAND project_tests)
