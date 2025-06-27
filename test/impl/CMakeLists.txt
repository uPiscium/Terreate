cmake_minimum_required(VERSION 3.20)
project(TerreateTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} test.cpp)

target_link_libraries(${PROJECT_NAME} Terreate)
target_include_directories(${PROJECT_NAME} PUBLIC ../include)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                                 ${CMAKE_BINARY_DIR}/bin)
