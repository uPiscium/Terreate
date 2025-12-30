function(setup_build_flags)
  if(TERREATE_DEBUG_BUILD)
    message(STATUS "[Terreate] Building with debug flags.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer")
    add_compile_definitions(TERREATE_DEBUG_BUILD)
  else()
    message(STATUS "[Terreate] Building with release flags.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wextra")
  endif()
endfunction()

function(configure_target target include_dir libraries)
  target_include_directories(${target} PRIVATE ${include_dir})
  target_link_libraries(${target} PUBLIC ${libraries})
  set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
  )
  set_target_properties(
    ${target} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  )
endfunction()
